///////////////////////////////////////////////////
// File:		ShaderCgGL.cpp
// Date:		11th December 2013
// Authors:		Matt Phillips
// Description:	Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "core/Debug.h"
#include "io/file.h"
#include "renderer/opengl/ShaderCgGL.h"
#include "renderer/opengl/TextureOpenGL.h"

namespace ion
{
	namespace render
	{
		CGcontext ShaderCgGL::sCgContext = 0;
		int ShaderCgGL::sContextRefCount = 0;

		Shader* Shader::Create()
		{
			return new ShaderCgGL();
		}

		ShaderCgGL::ShaderCgGL()
		{
			if(!sContextRefCount)
			{
				sCgContext = cgCreateContext();

				//Case sensitive semantics
				cgSetSemanticCasePolicy(CG_UNCHANGED_CASE_POLICY);

				//Set managed texture params (automatically binds/unbinds textures)
				cgGLSetManageTextureParameters(sCgContext, true);
			}

			sContextRefCount++;
		}

		ShaderCgGL::~ShaderCgGL()
		{
			cgDestroyProgram(mCgProgram);

			sContextRefCount--;

			if(!sContextRefCount)
			{
				cgDestroyContext(sCgContext);
			}
		}

		bool ShaderCgGL::Load(const std::string& filename, const std::string& entryPoint, ProgramType programType)
		{
			bool result = false;

			//Open file
			io::File file(filename, io::File::OpenRead);

			if(file.IsOpen())
			{
				//Load file contents
				int fileSize = file.GetSize();
				char* programText = new char[fileSize + 1];
				file.Read(programText, fileSize);
				programText[fileSize] = 0;

				if(programType == Vertex)
				{
					mCgProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
				}
				else if (programType == Fragment)
				{
					mCgProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
				}

				cgGLSetOptimalOptions(mCgProfile);

				//Compile program
				mCgProgram = cgCreateProgram(sCgContext, CG_SOURCE, programText, mCgProfile, entryPoint.c_str(), NULL);

				//Done with file text
				delete programText;

				//Done with file
				file.Close();

				if(CheckCgError())
				{
					//Load Cg program
					cgGLLoadProgram(mCgProgram);

					if(CheckCgError())
					{
						result = true;
					}
				}
			}

			return result;
		}

		void ShaderCgGL::Bind()
		{
			cgGLEnableProfile(mCgProfile);
			cgGLBindProgram(mCgProgram);
			CheckCgError();
		}

		void ShaderCgGL::Unbind()
		{
			cgGLUnbindProgram(mCgProfile);
			cgGLDisableProfile(mCgProfile);
			CheckCgError();
		}

		Shader::ShaderParamDelegate* ShaderCgGL::CreateShaderParamDelegate(const std::string& paramName)
		{
			ShaderParamDelegateCg* paramDelegate = NULL;

			CGparameter param = cgGetNamedParameter(mCgProgram, paramName.c_str());
			if(param)
			{
				paramDelegate = new ShaderParamDelegateCg(param);
			}

			return paramDelegate;
		}

		bool ShaderCgGL::CheckCgError()
		{
			CGerror error = cgGetError();

			if(error)
			{
				std::string errorStr = cgGetErrorString(error);

				if(error == CG_COMPILER_ERROR)
				{
					errorStr += " : ";
					errorStr += cgGetLastListing(sCgContext);
				}

				debug::Error(errorStr.c_str());
			}
			
			return error == CG_NO_ERROR;
		}

		ShaderCgGL::ShaderParamDelegateCg::ShaderParamDelegateCg(CGparameter cgParam)
		{
			mCgParam = cgParam;
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const int& value)
		{
			cgGLSetParameter1f(mCgParam, (float)value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const float& value)
		{
			cgGLSetParameter1f(mCgParam, value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector2& value)
		{
			cgGLSetParameter2fv(mCgParam, (float*)&value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector3& value)
		{
			cgGLSetParameter3fv(mCgParam, (float*)&value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Colour& value)
		{
			cgGLSetParameter4fv(mCgParam, (float*)&value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Matrix4& value)
		{
			cgSetMatrixParameterfc(mCgParam, (float*)&value);
			CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Texture& value)
		{
			cgGLSetTextureParameter(mCgParam, ((TextureOpenGL&)value).GetTextureId());
			CheckCgError();
		}
	}
}