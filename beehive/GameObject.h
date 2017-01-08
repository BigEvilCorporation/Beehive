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

#pragma once

#include <ion/maths/Vector.h>
#include <ion/io/Serialise.h>
#include <vector>
#include <sstream>
#include <string>

#include "SpriteSheet.h"

#include "Types.h"

enum GameObjectVariableSize
{
	eSizeByte = 1,
	eSizeWord = 2,
	eSizeLong = 4
};

struct GameObjectVariable
{
	GameObjectVariable()
	{
		m_name = "GameObj_";
		m_value = "0x0";
		m_size = 1;
	}

	void Serialise(ion::io::Archive& archive)
	{
		archive.Serialise(m_name, "name");
		archive.Serialise(m_value, "value");
		archive.Serialise(m_size, "size");
	}

	std::string m_name;
	std::string m_value;
	u8 m_size;
};

class GameObjectType
{
public:
	GameObjectType();
	GameObjectType(GameObjectTypeId id);

	GameObjectTypeId GetId() const { return m_id; }

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	void SetPreviewSpriteSheet(SpriteSheetId spriteSheetId) { m_previewSpriteSheet = spriteSheetId;  }
	SpriteSheetId GetPreviewSpriteSheet() const { return m_previewSpriteSheet; }

	const ion::Vector2i& GetDimensions() const { return m_dimensions; }
	void SetDimensions(const ion::Vector2i& dimensions) { m_dimensions = dimensions; }

	GameObjectVariable& AddVariable();
	void RemoveVariable(GameObjectVariable& variable);
	GameObjectVariable* GetVariable(u32 index);
	const std::vector<GameObjectVariable>& GetVariables() const { return m_variables; }

	void Serialise(ion::io::Archive& archive);

private:
	GameObjectTypeId m_id;
	std::string m_name;
	SpriteSheetId m_previewSpriteSheet;
	std::vector<GameObjectVariable> m_variables;
	ion::Vector2i m_dimensions;
};

class GameObject
{
public:
	GameObject();
	GameObject(GameObjectId objectId, const GameObject& rhs);
	GameObject(GameObjectId objectId, GameObjectTypeId typeId, const ion::Vector2i& position);

	GameObjectId GetId() const { return m_objectId; }
	GameObjectTypeId GetTypeId() const { return m_typeId; }

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	const ion::Vector2i& GetPosition() const { return m_position; }
	void SetPosition(const ion::Vector2i& position) { m_position = position; }

	//Animation
	//TODO: Poll from current anim+track during rendering, don't store here
	const ion::Vector2i& GetAnimDrawOffset() const { return m_animDrawOffset; }
	void SetAnimDrawOffset(const ion::Vector2i& offset) { m_animDrawOffset = offset; }

	SpriteAnimId GetSpriteAnim() const { return m_spriteAnimId; }
	void SetSpriteAnim(SpriteAnimId animId) { m_spriteAnimId = animId; }

	u32 GetSpriteAnimFrame() const { return m_spriteAnimFrame; }
	void SetSpriteAnimFrame(u32 frame) { m_spriteAnimFrame = frame; }

	GameObjectVariable& AddVariable();
	void RemoveVariable(GameObjectVariable& variable);
	GameObjectVariable* GetVariable(u32 index);
	const std::vector<GameObjectVariable>& GetVariables() const { return m_variables; }

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream, const GameObjectType& gameObjectType) const;

private:
	void ParseValueTokens(std::string& valueString) const;

	GameObjectId m_objectId;
	GameObjectTypeId m_typeId;
	std::string m_name;
	ion::Vector2i m_position;
	std::vector<GameObjectVariable> m_variables;

	//Animation
	//TODO: Poll from current anim+track during rendering, don't store here
	ion::Vector2i m_animDrawOffset;
	SpriteAnimId m_spriteAnimId;
	u32 m_spriteAnimFrame;
};