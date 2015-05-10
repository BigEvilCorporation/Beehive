///////////////////////////////////////////////////
// File:		ShaderCgGL.h
// Date:		11th December 2013
// Authors:		Matt Phillips
// Description:	Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Shader.h"

#include <GL/glew.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

#include <Windows.h>
#include <WinDef.h>
#include <Wingdi.h>

namespace ion
{
	namespace render
	{
		class ShaderManagerCgGL : public ShaderManager
		{
		public:
			ShaderManagerCgGL();
			virtual ~ShaderManagerCgGL();

			static CGcontext sCgContext;
			static int sContextRefCount;

			static CGprofile sCgProfileVertex;
			static CGprofile sCgProfilePixel;

		private:
			static void ErrorCallback(CGcontext context, CGerror error, void* appdata);
		};

		class ShaderCgGL : public Shader
		{
		public:
			ShaderCgGL();
			virtual ~ShaderCgGL();

			virtual bool Load(const std::string& filename);

			//Bind/unbind
			virtual void Bind();
			virtual void Unbind();

		protected:

			class ShaderParamDelegateCg : public ShaderParamDelegate
			{
			public:
				ShaderParamDelegateCg(CGparameter cgParam);

				virtual void Set(const int& value);
				virtual void Set(const float& value);
				virtual void Set(const Vector2& value);
				virtual void Set(const Vector3& value);
				virtual void Set(const Colour& value);
				virtual void Set(const Matrix4& value);
				virtual void Set(const Texture& value);

				CGparameter mCgParam;
			};

			virtual ShaderParamDelegate* CreateShaderParamDelegate(const std::string& paramName);

			CGprogram mCgProgram;
			bool mCgProgramLoaded;
		};
	}
}