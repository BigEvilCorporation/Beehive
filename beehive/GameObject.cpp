///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#include "GameObject.h"
#include <algorithm>
#include <iomanip>

GameObjectType::GameObjectType()
{
	m_id = InvalidGameObjectTypeId;
	m_previewSpriteSheet = InvalidSpriteSheetId;
}

GameObjectType::GameObjectType(u32 id)
{
	m_id = id;
	m_name = "GameObj_";
	m_previewSpriteSheet = InvalidSpriteSheetId;
	m_dimensions.x = 16;
	m_dimensions.y = 16;
}

GameObjectVariable& GameObjectType::AddVariable()
{
	m_variables.push_back(GameObjectVariable());
	return m_variables[m_variables.size() - 1];
}

void GameObjectType::RemoveVariable(GameObjectVariable& variable)
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

GameObjectVariable* GameObjectType::GetVariable(u32 index)
{
	ion::debug::Assert(index < m_variables.size(), "GameObjectType::GetVariable() - out of range");
	return &m_variables[index];
}

void GameObjectType::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_id, "id");
	archive.Serialise(m_name, "name");
	archive.Serialise(m_previewSpriteSheet, "previewSpriteSheet");
	archive.Serialise(m_variables, "variables");
	archive.Serialise(m_dimensions, "dimensions");
}

GameObject::GameObject()
{
	m_objectId = InvalidGameObjectId;
	m_typeId = InvalidGameObjectTypeId;
}

GameObject::GameObject(GameObjectId objectId, const GameObject& rhs)
{
	m_objectId = objectId;
	m_typeId = rhs.m_typeId;
	m_position = rhs.m_position;
	m_name = rhs.m_name;
	m_spriteAnimId = rhs.m_spriteAnimId;
	m_spriteAnimFrame = rhs.m_spriteAnimFrame;
	m_animDrawOffset = rhs.m_animDrawOffset;
	m_variables = rhs.m_variables;
}

GameObject::GameObject(GameObjectId objectId, GameObjectTypeId typeId, const ion::Vector2i& position)
{
	m_objectId = objectId;
	m_typeId = typeId;
	m_position = position;

	m_spriteAnimId = 0;
	m_spriteAnimFrame = 0;
}

GameObjectVariable& GameObject::AddVariable()
{
	m_variables.push_back(GameObjectVariable());
	return m_variables[m_variables.size() - 1];
}

void GameObject::RemoveVariable(GameObjectVariable& variable)
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

GameObjectVariable* GameObject::GetVariable(u32 index)
{
	ion::debug::Assert(index < m_variables.size(), "GameObject::GetVariable() - out of range");
	return &m_variables[index];
}

void GameObject::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_objectId, "objectId");
	archive.Serialise(m_typeId, "typeId");
	archive.Serialise(m_name, "name");
	archive.Serialise(m_position, "position");
	archive.Serialise(m_variables, "variables");
}

void GameObject::Export(std::stringstream& stream, const GameObjectType& gameObjectType) const
{
	stream << '\t' << "jsr " << gameObjectType.GetName() << "Init" << std::endl;

	const std::vector<GameObjectVariable>& templateVariables = gameObjectType.GetVariables();

	for(int i = 0; i < templateVariables.size(); i++)
	{
		//"move.[s] #[value], [name](a0)"

		stream << '\t';

		switch(templateVariables[i].m_size)
		{
		case eSizeByte:
			stream << "move.b ";
			break;
		case eSizeWord:
			stream << "move.w ";
			break;
		case eSizeLong:
			stream << "move.l ";
			break;
		}

		std::string valueString = templateVariables[i].m_value;
		ParseValueTokens(valueString);

		stream << "#" << valueString << ", " << templateVariables[i].m_name << "(a0)" << std::endl;
	}

	const std::vector<GameObjectVariable>& instanceVariables = GetVariables();

	for(int i = 0; i < instanceVariables.size(); i++)
	{
		//"move.[s] #[value], [name](a0)"

		stream << '\t';

		switch(instanceVariables[i].m_size)
		{
		case eSizeByte:
			stream << "move.b ";
			break;
		case eSizeWord:
			stream << "move.w ";
			break;
		case eSizeLong:
			stream << "move.l ";
			break;
		}

		std::string valueString = instanceVariables[i].m_value;
		ParseValueTokens(valueString);

		stream << "#" << valueString << ", " << instanceVariables[i].m_name << "(a0)" << std::endl;
	}
}

void GameObject::ParseValueTokens(std::string& valueString) const
{
	const int screenToWorldSpaceShift = 8;
	const int spriteSheetBorderX = 128;
	const int spriteSheetBorderY = 128;

	const std::string worldPosXString("&WORLDPOSX");

	std::string::size_type worldPosXStart = valueString.find(worldPosXString);
	if(worldPosXStart != std::string::npos)
	{
		u32 worldPosX = (m_position.x + spriteSheetBorderX) << screenToWorldSpaceShift;

		std::stringstream hexStream;
		hexStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << worldPosX;

		valueString.replace(worldPosXStart, worldPosXString.size(), hexStream.str());
	}

	const std::string worldPosYString("&WORLDPOSY");

	std::string::size_type worldPosYStart = valueString.find(worldPosYString);
	if(worldPosYStart != std::string::npos)
	{
		u32 worldPosY = (m_position.y + spriteSheetBorderY) << screenToWorldSpaceShift;

		std::stringstream hexStream;
		hexStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << worldPosY;

		valueString.replace(worldPosYStart, worldPosYString.size(), hexStream.str());
	}
}