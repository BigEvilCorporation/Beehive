///////////////////////////////////////////////////
// File:		Shader.h
// Date:		8th December 2013
// Authors:		Matt Phillips
// Description:	Shader base class
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"
#include "renderer/colour.h"

#include <string>
#include <map>

namespace ion
{
	namespace render
	{
		//Forward declaration
		class Texture;

		class Shader
		{
		protected:
			class ShaderParamDelegate;

		public:
			enum ProgramType { Vertex, Fragment };

			template <typename T> class ParamHndl
			{
			public:
				ParamHndl();

				bool IsValid() const;

				void SetValue(const T& value);

			protected:
				ShaderParamDelegate* mParamDelegate;

				friend class Shader;
			};

			static Shader* Create();
			virtual ~Shader();

			//Load shader from file
			virtual bool Load(const std::string& filename, const std::string& entryPoint, ProgramType programType) = 0;

			//Get handle to a shader parameter
			template <typename T> ParamHndl<T> CreateParamHndl(const std::string& name);

			//Bind/unbind
			virtual void Bind() = 0;
			virtual void Unbind() = 0;

		protected:

			class ShaderParamDelegate
			{
			public:
				virtual void Set(const int& value) = 0;
				virtual void Set(const float& value) = 0;
				virtual void Set(const Vector2& value) = 0;
				virtual void Set(const Vector3& value) = 0;
				virtual void Set(const Colour& value) = 0;
				virtual void Set(const Matrix4& value) = 0;
				virtual void Set(const Texture& value) = 0;
			};

			Shader();

			virtual ShaderParamDelegate* CreateShaderParamDelegate(const std::string& paramName) = 0;
		};

		template <typename T> Shader::ParamHndl<T> Shader::CreateParamHndl(const std::string& name)
		{
			ParamHndl<T> handle;
			handle.mParamDelegate = CreateShaderParamDelegate(name);
			return handle;
		}

		template <typename T> Shader::ParamHndl<T>::ParamHndl()
		{
			mParamDelegate = NULL;
		}

		template <typename T> bool Shader::ParamHndl<T>::IsValid() const
		{
			return mParamDelegate != NULL;
		}

		template <typename T> void Shader::ParamHndl<T>::SetValue(const T& value)
		{
			if(IsValid())
			{
				mParamDelegate->Set(value);
			}
		}
	}
}