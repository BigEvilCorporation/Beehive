///////////////////////////////////////////////////
// File:		Shader.cpp
// Date:		8th December 2013
// Authors:		Matt Phillips
// Description:	Shader base class
///////////////////////////////////////////////////

#include "renderer/Shader.h"

namespace ion
{
	namespace render
	{
		Shader::Shader()
		{

		}

		Shader::~Shader()
		{

		}

		void Shader::SetProgram(const std::string programFilename, const std::string& entryPoint, ProgramType programtype)
		{
			m_programFilename = programFilename;
			m_entryPoint = entryPoint;
			m_programType = programtype;
		}

		void Shader::Serialise(io::Archive& archive)
		{
			archive.Serialise(m_programFilename);
			archive.Serialise(m_entryPoint);
			archive.Serialise((u32&)m_programType);

			if(archive.GetDirection() == io::Archive::eIn)
			{
				if(archive.GetResourceManager())
				{
					Load(archive.GetResourceManager()->GetResourceDirectory<Shader>());
				}
			}
		}
	}
}