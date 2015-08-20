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

class GameObjectType
{
public:
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

	GameObjectType();

	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() const { return m_name; }

	Variable& AddVariable();
	void RemoveVariable(Variable& variable);
	const std::vector<Variable>& GetVariables() const { return m_variables; }

	void Serialise(ion::io::Archive& archive);
	void Export(std::stringstream& stream) const;

private:
	std::string m_name;
	std::vector<Variable> m_variables;
	ion::Vector2i m_dimensions;
};