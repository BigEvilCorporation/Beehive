///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObject.h"

GameObjectType::GameObjectType(u32 id)
{
	m_id = id;
	m_name = "GameObj_";
	m_dimensions.x = 16;
	m_dimensions.y = 16;
}

GameObjectType::Variable& GameObjectType::AddVariable()
{
	m_variables.push_back(Variable());
	return m_variables[m_variables.size() - 1];
}

void GameObjectType::RemoveVariable(GameObjectType::Variable& variable)
{
	for(int i = 0; i < m_variables.size(); i++)
	{
		if(m_variables[i].m_name == variable.m_name)
		{
			std::swap(variable, m_variables.back());
			m_variables.pop_back();
			return;
		}
	}
}

GameObjectType::Variable* GameObjectType::GetVariable(u32 index)
{
	ion::debug::Assert(index < m_variables.size(), "GameObjectType::GetVariable() - out of range");
	return &m_variables[index];
}

void GameObjectType::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_variables, "variables");
	archive.Serialise(m_dimensions, "dimensions");
}

GameObject::GameObject(GameObjectId objectId, GameObjectTypeId typeId)
{
	m_objectId = objectId;
	m_typeId = typeId;
}

void GameObject::Serialise(ion::io::Archive& archive)
{

}

void GameObject::Export(std::stringstream& stream, GameObjectType& objectType) const
{
	stream << objectType.GetName().c_str() << "_" << (u32)m_objectId << ":" << std::endl;

	const std::vector<GameObjectType::Variable>& variables = objectType.GetVariables();

	for(int i = 0; i < variables.size(); i++)
	{
		//"move.[s] #[value], [name](a0)"

		switch(variables[i].m_size)
		{
		case GameObjectType::eSizeByte:
			stream << "move.b ";
			break;
		case GameObjectType::eSizeWord:
			stream << "move.w ";
			break;
		case GameObjectType::eSizeLong:
			stream << "move.l ";
			break;
		}

		stream << "#" << variables[i].m_value.c_str() << ", " << variables[i].m_name.c_str() << "(a0)" << std::endl;
	}
}