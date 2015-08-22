///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#pragma once

#include <ion/maths/Vector.h>
#include <ion/io/Serialise.h>
#include <vector>
#include <sstream>
#include <string>

typedef u32 GameObjectTypeId;
typedef u32 GameObjectId;

static const GameObjectTypeId InvalidGameObjectTypeId = 0;
static const GameObjectTypeId InvalidGameObjectId = 0;

class GameObjectType
{
public:
	enum VariableSize
	{
		eSizeByte = 1,
		eSizeWord = 2,
		eSizeLong = 4
	};

	struct Variable
	{
		Variable()
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

	GameObjectType(GameObjectTypeId id);

	GameObjectTypeId GetId() const { return m_id; }

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	const ion::Vector2i& GetDimensions() const { return m_dimensions; }
	void SetDimensions(const ion::Vector2i& dimensions) { m_dimensions = dimensions; }

	Variable& AddVariable();
	void RemoveVariable(Variable& variable);
	Variable* GetVariable(u32 index);
	const std::vector<Variable>& GetVariables() const { return m_variables; }

	void Serialise(ion::io::Archive& archive);

private:
	GameObjectTypeId m_id;
	std::string m_name;
	std::vector<Variable> m_variables;
	ion::Vector2i m_dimensions;
};

class GameObject
{
public:
	GameObject(GameObjectId objectId, GameObjectTypeId typeId);

	GameObjectId GetId() const { return m_objectId; }
	GameObjectTypeId GetTypeId() const { return m_typeId; }

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream, GameObjectType& objectType) const;

private:
	GameObjectId m_objectId;
	GameObjectTypeId m_typeId;
	ion::Vector2i m_position;
};