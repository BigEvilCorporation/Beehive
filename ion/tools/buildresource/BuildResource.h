///////////////////////////////////////////////////
// File:		BuildResource.h
// Date:		15th February 2014
// Authors:		Matt Phillips
// Description:	Resource build tool
///////////////////////////////////////////////////

#pragma once

#include "maths/Matrix.h"
#include "maths/Vector.h"
#include "io/File.h"
#include "renderer/Texture.h"
#include "renderer/Shader.h"

#include <string>

namespace ion
{
	namespace build
	{
		void BuildTexture(const std::string& name, const std::string& textureFile);
		void BuildShader(const std::string& name, const std::string& vertexProgram, const std::string& fragmentProgram, const std::string& entrypoint);
	}
}