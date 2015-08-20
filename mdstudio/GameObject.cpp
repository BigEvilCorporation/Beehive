///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "GameObject.h"

GameObjectType::GameObjectType()
{
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

void GameObjectType::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_variables, "variables");
	archive.Serialise(m_dimensions, "dimensions");
}

void GameObjectType::Export(std::stringstream& stream) const
{

}
