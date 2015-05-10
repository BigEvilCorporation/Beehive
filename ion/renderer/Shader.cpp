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
			mProgramFilename = programFilename;
			mEntryPoint = entryPoint;
			mProgramType = programtype;
		}

		void Shader::Serialise(io::Archive& archive)
		{
			archive.Serialise(mProgramFilename);
			archive.Serialise(mEntryPoint);
			archive.Serialise((u32&)mProgramType);

			if(archive.GetDirection() == io::Archive::In)
			{
				if(archive.GetResourceManager())
				{
					Load(archive.GetResourceManager()->GetResourceDirectory<Shader>());
				}
			}
		}
	}
}