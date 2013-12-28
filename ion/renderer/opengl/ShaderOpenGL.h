///////////////////////////////////////////////////
// File:		ShaderOpenGL.h
// Date:		8th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL/Cg shader implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Shader.h"
namespace ion
{
	namespace renderer
	{
		class ShaderOpenGL : public Shader
		{
		public:
			template <typename T> class ParamHndlOpenGL : public Shader::ParamHndl<T>
			{
			public:
				virtual ~ParamHndlOpenGL() {}

				void SetValue(const T& value);
				T GetValue();

			protected:
			};

			ShaderOpenGL();
			virtual ~ShaderOpenGL();

			//Load shader from file
			virtual bool Load(const std::string& filename, const std::string& entryPoint, ProgramType programType);

			//Create handle to a shader parameter
			template <typename T> virtual ParamHndl<T>* CreateParamHndl(const std::string& name);

		protected:
		};
	}
}