///////////////////////////////////////////////////
// File:		BuildResource.h
// Date:		15th February 2014
// Authors:		Matt Phillips
// Description:	Resource build tool
///////////////////////////////////////////////////

#pragma once

#include "tools/buildresource/BuildResource.h"
#include "core/containers/Queue.h"
#include "core/debug/Debug.h"

#include <string>
#include <vector>
#include <locale>
#include <algorithm>

const int sMaxTokens = 32;

bool CheckToken(const ion::Queue<std::string, sMaxTokens>& tokens, const std::string& error)
{
	if(tokens.IsEmpty())
	{
		std::string errorMsg = "Error: ";
		errorMsg += error;
		ion::debug::Error(errorMsg.c_str());
	}

	return !tokens.IsEmpty();
}

int main(int numargs, char** args)
{
	ion::Queue<std::string, sMaxTokens> tokens;

	for(int i = 1; i < numargs; i++)
	{
		tokens.Push(std::string(args[i]));
	}

	if(CheckToken(tokens, "Missing resource type"))
	{
		std::locale locale;
		std::string resourceType = tokens.Pop();
		std::transform(resourceType.begin(), resourceType.end(), resourceType.begin(), ::tolower);

		if(!std::strcmp(resourceType.c_str(), "texture"))
		{
			if(CheckToken(tokens, "Missing texture name"))
			{
				const std::string& name = tokens.Pop();

				if(CheckToken(tokens, "Missing texture filename"))
				{
					const std::string& textureFile = tokens.Pop();
					ion::build::BuildTexture(name, textureFile);
				}
			}
		}
	}

	ion::debug::Log("Done.");

	return 0;
}

namespace ion
{
	namespace build
	{
		void BuildTexture(const std::string& name, const std::string& textureFile)
		{
			std::string filename = name;
			filename += ".ion.texture";

			ion::io::File file(filename, ion::io::File::OpenWrite);
			if(file.IsOpen())
			{
				ion::render::Texture* texture = ion::render::Texture::Create();
				texture->SetImageFilename(textureFile);
				ion::io::Archive archive(file, ion::io::Archive::Out);
				ion::render::Texture::RegisterSerialiseType(archive);
				archive.Serialise(texture);
				file.Close();

				std::string message = "Written: ";
				message += filename;
				debug::Log(message.c_str());
			}
		}

		void BuildShader(const std::string& name, const std::string& vertexProgram, const std::string& fragmentProgram, const std::string& entrypoint)
		{
		}
	}
}