///////////////////////////////////////////////////
// File:		ShaderCgGL.cpp
// Date:		11th December 2013
// Authors:		Matt Phillips
// Description:	Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "core/debug/Debug.h"
#include "io/file.h"
#include "renderer/opengl/ShaderCgGL.h"
#include "renderer/opengl/TextureOpenGL.h"
#include "renderer/opengl/RendererOpenGL.h"

namespace ion
{
	namespace render
	{
		CGcontext ShaderManagerCgGL::sCgContext = 0;
		int ShaderManagerCgGL::sContextRefCount = 0;

		ShaderManager* ShaderManager::Create()
		{
			return new ShaderManagerCgGL();
		}

		ShaderManagerCgGL::ShaderManagerCgGL()
		{
			if(!sContextRefCount)
			{
				RendererOpenGL::LockGLContext();

				sCgContext = cgCreateContext();

				//Case sensitive semantics
				cgSetSemanticCasePolicy(CG_UNCHANGED_CASE_POLICY);

				//Set managed texture params (automatically binds/unbinds textures)
				cgGLSetManageTextureParameters(sCgContext, true);

				RendererOpenGL::UnlockGLContext();
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

		bool ShaderManagerCgGL::CheckCgError()
		{
			RendererOpenGL::LockGLContext();
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

			RendererOpenGL::UnlockGLContext();

			return error == CG_NO_ERROR;
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
			mCgProgram = 0;
			mCgProgramLoaded = false;
		}

		ShaderCgGL::~ShaderCgGL()
		{
			if(mCgProgram)
			{
				RendererOpenGL::LockGLContext();
				cgDestroyProgram(mCgProgram);
				RendererOpenGL::UnlockGLContext();
			}
		}

		bool ShaderCgGL::Load(const std::string& shaderDirectory)
		{
			//Open file
			std::string path = shaderDirectory + "/programs/" + mProgramFilename;
			io::File file(path, io::File::OpenRead);

			bool result = false;

			if(file.IsOpen())
			{
				//Load file contents
				u64 fileSize = file.GetSize();
				char* programText = new char[(int)fileSize + 1];
				file.Read(programText, fileSize);
				programText[fileSize] = 0;

				//Done with file
				file.Close();

				//OpenGL thread safety
				RendererOpenGL::LockGLContext();

				if(mProgramType == Vertex)
				{
					mCgProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
				}
				else if (mProgramType == Fragment)
				{
					mCgProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
				}

				cgGLSetOptimalOptions(mCgProfile);

				//Compile program
				mCgProgram = cgCreateProgram(ShaderManagerCgGL::sCgContext, CG_SOURCE, programText, mCgProfile, mEntryPoint.c_str(), NULL);

				result = ShaderManagerCgGL::CheckCgError();

				//OpenGL thread safety
				RendererOpenGL::UnlockGLContext();

				//Done with file text
				delete programText;
			}

			return result;
		}

		void ShaderCgGL::Bind()
		{
			RendererOpenGL::LockGLContext();

			if(!mCgProgramLoaded)
			{
				cgGLLoadProgram(mCgProgram);
				mCgProgramLoaded = true;
			}

			cgGLEnableProfile(mCgProfile);
			cgGLBindProgram(mCgProgram);
			ShaderManagerCgGL::CheckCgError();

			RendererOpenGL::UnlockGLContext();
		}

		void ShaderCgGL::Unbind()
		{
			RendererOpenGL::LockGLContext();
			cgGLUnbindProgram(mCgProfile);
			cgGLDisableProfile(mCgProfile);
			ShaderManagerCgGL::CheckCgError();
			RendererOpenGL::UnlockGLContext();
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

		ShaderCgGL::ShaderParamDelegateCg::ShaderParamDelegateCg(CGparameter cgParam)
		{
			mCgParam = cgParam;
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const int& value)
		{
			cgGLSetParameter1f(mCgParam, (float)value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const float& value)
		{
			cgGLSetParameter1f(mCgParam, value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector2& value)
		{
			cgGLSetParameter2fv(mCgParam, (float*)&value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Vector3& value)
		{
			cgGLSetParameter3fv(mCgParam, (float*)&value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Colour& value)
		{
			cgGLSetParameter4fv(mCgParam, (float*)&value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Matrix4& value)
		{
			cgSetMatrixParameterfc(mCgParam, (float*)&value);
			ShaderManagerCgGL::CheckCgError();
		}

		void ShaderCgGL::ShaderParamDelegateCg::Set(const Texture& value)
		{
			cgGLSetTextureParameter(mCgParam, ((TextureOpenGL&)value).GetTextureId());
			ShaderManagerCgGL::CheckCgError();
		}
	}
}