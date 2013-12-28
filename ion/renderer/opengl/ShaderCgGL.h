///////////////////////////////////////////////////
// File:		ShaderCgGL.h
// Date:		11th December 2013
// Authors:		Matt Phillips
// Description:	Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Shader.h"

#include <Cg\cg.h>
#include <Cg\cgGL.h>

namespace ion
{
	namespace render
	{
		class ShaderCgGL : public Shader
		{
		public:
			ShaderCgGL();
			virtual ~ShaderCgGL();

			//Load shader from file
			virtual bool Load(const std::string& filename, const std::string& entryPoint, ProgramType programType);

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

			static bool CheckCgError();

			static CGcontext sCgContext;
			static int sContextRefCount;
			CGprogram mCgProgram;
			CGprofile mCgProfile;
		};
	}
}