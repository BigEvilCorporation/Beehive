///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObject.h"
#include <algorithm>
#include <iomanip>

GameObjectType::GameObjectType()
{
	m_id = InvalidGameObjectTypeId;
}

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
	archive.Serialise(m_id, "id");
	archive.Serialise(m_name, "name");
	archive.Serialise(m_variables, "variables");
	archive.Serialise(m_dimensions, "dimensions");
}

GameObject::GameObject()
{
	m_objectId = InvalidGameObjectId;
	m_typeId = InvalidGameObjectTypeId;
}

GameObject::GameObject(GameObjectId objectId, GameObjectTypeId typeId, const ion::Vector2i& position)
{
	m_objectId = objectId;
	m_typeId = typeId;
	m_position = position;
}

void GameObject::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_objectId, "objectId");
	archive.Serialise(m_typeId, "typeId");
	archive.Serialise(m_position, "position");
}

void GameObject::Export(std::stringstream& stream, const GameObjectType& objectType) const
{
	stream << objectType.GetName().c_str() << "_" << (u32)m_objectId << ":" << std::endl;
	stream << '\t' << "jsr " << objectType.GetName() << "Init" << std::endl;

	const std::vector<GameObjectType::Variable>& variables = objectType.GetVariables();

	for(int i = 0; i < variables.size(); i++)
	{
		//"move.[s] #[value], [name](a0)"

		stream << '\t';

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

		std::string valueString = variables[i].m_value;
		ParseValueTokens(valueString);

		stream << "#" << valueString << ", " << variables[i].m_name << "(a0)" << std::endl;
	}
}

void GameObject::ParseValueTokens(std::string& valueString) const
{
	const int tileWidth = 8;
	const int tileHeight = 8;
	const int screenToWorldSpaceShift = 8;
	const int spriteBorderX = 128;
	const int spriteBorderY = 128;

	const std::string worldPosXString("&WORLDPOSX");

	std::string::size_type worldPosXStart = valueString.find(worldPosXString);
	if(worldPosXStart != std::string::npos)
	{
		u32 worldPosX = ((m_position.x * tileWidth) + spriteBorderX) << screenToWorldSpaceShift;

		std::stringstream hexStream;
		hexStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << worldPosX;

		valueString.replace(worldPosXStart, worldPosXString.size(), hexStream.str());
	}

	const std::string worldPosYString("&WORLDPOSY");

	std::string::size_type worldPosYStart = valueString.find(worldPosYString);
	if(worldPosYStart != std::string::npos)
	{
		u32 worldPosY = ((m_position.y * tileHeight) + spriteBorderY) << screenToWorldSpaceShift;

		std::stringstream hexStream;
		hexStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << worldPosY;

		valueString.replace(worldPosYStart, worldPosYString.size(), hexStream.str());
	}
}