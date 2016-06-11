///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		ShaderCgGL.cpp
// Date:		11th December 2013
// Authors:		Matt Phillips
// Description:	Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "core/debug/Debug.h"
#include "io/File.h"
#include "io/Archive.h"
#include "renderer/opengl/ShaderCgGL.h"
#include "renderer/opengl/TextureOpenGL.h"
#include "renderer/opengl/RendererOpenGL.h"

namespace ion
{
	namespace render
	{
		CGcontext ShaderManagerCgGL::sCgContext = 0;
		int ShaderManagerCgGL::sContextRefCount = 0;
		CGprofile ShaderManagerCgGL::sCgProfileVertex;
		CGprofile ShaderManagerCgGL::sCgProfilePixel;

		ShaderManager* ShaderManager::Create()
		{
			return new ShaderManagerCgGL();
		}

		ShaderManagerCgGL::ShaderManagerCgGL()
		{
			if(!sContextRefCount)
			{
				//Set error handler
				cgSetErrorHandler(ErrorCallback, NULL);

				sCgContext = cgCreateContext();

				//Case sensitive semantics
				cgSetSemanticCasePolicy(CG_UNCHANGED_CASE_POLICY);

				//Set managed texture params (automatically binds/unbinds textures)
				cgGLSetManageTextureParameters(sCgContext, true);

				//Get profiles for current GL context
				sCgProfileVertex = cgGLGetLatestProfile(CG_GL_VERTEX);
				sCgProfilePixel = cgGLGetLatestProfile(CG_GL_FRAGMENT);

				//Set optimal options for current GL context
				cgGLSetOptimalOptions(sCgProfileVertex);
				cgGLSetOptimalOptions(sCgProfilePixel);
			}

			sContextRefCount++;
		}

		ShaderManagerCgGL::~ShaderManagerCgGL()
		{
			sContextRefCount--;

			if(!sContextRefCount)
			{
				cgDestroyContext(sCgContext);
			}
		}

		void ShaderManagerCgGL::ErrorCallback(CGcontext context, CGerror error, void* appdata)
		{
			std::string errorStr = cgGetErrorString(error);

			if(error == CG_COMPILER_ERROR)
			{
				errorStr += " : ";
				errorStr += cgGetLastListing(sCgContext);
			}

			debug::Error(errorStr.c_str());
		}

		Shader* Shader::Create()
		{
			return new ShaderCgGL();
		}

		void Shader::RegisterSerialiseType(io::Archive& archive)
		{
			archive.RegisterPointerTypeStrict<Shader, ShaderCgGL>();
		}

		ShaderCgGL::ShaderCgGL()
		{
			m_cgProgram = 0;
			m_cgProgramLoaded = false;
		}

		ShaderCgGL::~ShaderCgGL()
		{
			if(m_cgProgram)
			{
				cgDestroyProgram(m_cgProgram);
			}
		}

		bool ShaderCgGL::Load(const std::string& filename)
		{
			//Open shader file
			io::File shaderFile(filename, io::File::eOpenRead);
			if(shaderFile.IsOpen())
			{
				//Serialise
				io::Archive archiveIn(shaderFile, io::Archive::eIn, NULL);
				archiveIn.Serialise(*this);

				//Done with shader file
				shaderFile.Close();

				//Load program file
				std::string shaderDirectory = filename.substr(0, filename.find_first_of('/'));
				std::string programFilename = shaderDirectory + "/programs/cg/" + m_programFilename;
				io::File programFile(programFilename, io::File::eOpenRead);

				if(programFile.IsOpen())
				{
					//Load file contents
					u64 fileSize = programFile.GetSize();
					char* programText = new char[(int)fileSize + 1];
					programFile.Read(programText, fileSize);
					programText[fileSize] = 0;

					//Done with file
					programFile.Close();

					CGprofile cgProfile;
					if(m_programType == eVertex)
						cgProfile = ShaderManagerCgGL::sCgProfileVertex;
					else if(m_programType == eFragment)
						cgProfile = ShaderManagerCgGL::sCgProfilePixel;

					//Compile program
					m_cgProgram = cgCreateProgram(ShaderManagerCgGL::sCgContext, CG_SOURCE, programText, cgProfile, m_entryPoint.c_str(), NULL);

					//Done with file text
					delete programText;

					return true;
				}
			}

			return false;
		}

		void ShaderCgGL::Bind()
		{
			CGprofile cgProfile;
			if(m_programType == eVertex)
				cgProfile = ShaderManagerCgGL::sCgProfileVertex;
			else if(m_programType == eFragment)
				cgProfile = ShaderManagerCgGL::sCgProfilePixel;

			if(!m_cgProgramLoaded)
			{
				cgGLLoadProgram(m_cgProgram);
				m_cgProgramLoaded = true;
			}

			cgGLEnableProfile(cgProfile);
			cgGLBindProgram(m_cgProgram);
		}

		void ShaderCgGL::Unbind()
		{
			CGprofile cgProfile;
			if(m_programType == eVertex)
				cgProfile = ShaderManagerCgGL::sCgProfileVertex;
			else if(m_programType == eFragment)
				cgProfile = ShaderManagerCgGL::sCgProfilePixel;

			cgGLUnbindProgram(cgProfile);
			cgGLDisableProfile(cgProfile);
		}

		Shader::ShaderParamDelegate* ShaderCgGL::CreateShaderParamDelegate(const std::string& paramName)
		{
			ShaderParamDelegateCg* paramDelegate = NULL;

			CGparameter param = cgGetNamedParameter(m_cgProgram, paramName.c_str());
			if(param)
			{
				paramDelegate = new ShaderParamDelegateCg(param);
			}

			return paramDelegate;
		}

		ShaderCgGL::ShaderParamDelegateCg::ShaderParamDelegateCg(CGparameter cgParam)
		{
			m_cgParam = cgParam;
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const int& value)
		{
			cgGLSetParameter1f(m_cgParam, (float)value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const float& value)
		{
			cgGLSetParameter1f(m_cgParam, value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector2& value)
		{
			cgGLSetParameter2fv(m_cgParam, (float*)&value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector3& value)
		{
			cgGLSetParameter3fv(m_cgParam, (float*)&value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Colour& value)
		{
			cgGLSetParameter4fv(m_cgParam, (float*)&value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Matrix4& value)
		{
			cgSetMatrixParameterfc(m_cgParam, (float*)&value);
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Texture& value)
		{
			cgGLSetTextureParameter(m_cgParam, ((TextureOpenGL&)value).GetTextureId());
		}
	}
}