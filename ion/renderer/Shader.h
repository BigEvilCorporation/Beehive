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
#include "io/Archive.h"
#include "renderer/colour.h"

#include <string>
#include <map>

namespace ion
{
	namespace render
	{
		//Forward declaration
		class Texture;

		class ShaderManager
		{
		public:
			static ShaderManager* Create();
			virtual ~ShaderManager() {}

		protected:
			ShaderManager() {}
		};

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
				ParamHndl(const ParamHndl<T>& rhs);
				ParamHndl(ShaderParamDelegate* paramDelegate);
				~ParamHndl();

				bool IsValid() const;
				void SetValue(const T& value);
				ParamHndl& operator = (const ParamHndl<T>& rhs);

			private:
				void Reference();
				void Release();

				ShaderParamDelegate* m_paramDelegate;
			};

			static Shader* Create();
			virtual ~Shader();

			//Load from file
			virtual bool Load(const std::string& filename) = 0;

			//Set entry point, program type and program filename
			void SetProgram(const std::string programFilename, const std::string& entryPoint, ProgramType programtype);

			//Get handle to a shader parameter
			template <typename T> ParamHndl<T> CreateParamHndl(const std::string& name);

			//Bind/unbind
			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			//Serialise
			static void RegisterSerialiseType(io::Archive& archive);
			void Serialise(io::Archive& archive);

		protected:

			class ShaderParamDelegate
			{
			public:
				ShaderParamDelegate() : m_refCount(0) {}

				virtual void Set(const int& value) = 0;
				virtual void Set(const float& value) = 0;
				virtual void Set(const Vector2& value) = 0;
				virtual void Set(const Vector3& value) = 0;
				virtual void Set(const Colour& value) = 0;
				virtual void Set(const Matrix4& value) = 0;
				virtual void Set(const Texture& value) = 0;
			protected:
				u32 m_refCount;
				friend class Shader;
			};

			Shader();
			virtual ShaderParamDelegate* CreateShaderParamDelegate(const std::string& paramName) = 0;

			std::string mProgramFilename;
			std::string mEntryPoint;
			ProgramType mProgramType;
		};

		template <typename T> Shader::ParamHndl<T> Shader::CreateParamHndl(const std::string& name)
		{
			return ParamHndl<T>(CreateShaderParamDelegate(name));
		}

		template <typename T> Shader::ParamHndl<T>::ParamHndl()
		{
			m_paramDelegate = NULL;
		}

		template <typename T> Shader::ParamHndl<T>::ParamHndl(const ParamHndl<T>& rhs)
		{
			m_paramDelegate = rhs.m_paramDelegate;
			Reference();
		}

		template <typename T> Shader::ParamHndl<T>::ParamHndl(ShaderParamDelegate* paramDelegate)
		{
			m_paramDelegate = paramDelegate;
			Reference();
		}
		
		template <typename T> Shader::ParamHndl<T>::~ParamHndl()
		{
			Release();
		}

		template <typename T> void Shader::ParamHndl<T>::Reference()
		{
			if(m_paramDelegate)
				m_paramDelegate->m_refCount++;
		}

		template <typename T> void Shader::ParamHndl<T>::Release()
		{
			if(m_paramDelegate)
			{
				ion::debug::Assert(m_paramDelegate->m_refCount > 0, "Bad ref count");
				m_paramDelegate->m_refCount--;
				if(!m_paramDelegate->m_refCount)
				{
					delete m_paramDelegate;
					m_paramDelegate = NULL;
				}
			}
		}

		template <typename T> bool Shader::ParamHndl<T>::IsValid() const
		{
			return m_paramDelegate != NULL;
		}

		template <typename T> void Shader::ParamHndl<T>::SetValue(const T& value)
		{
			if(IsValid())
			{
				m_paramDelegate->Set(value);
			}
		}

		template <typename T> Shader::ParamHndl<T>& Shader::ParamHndl<T>::operator = (const ParamHndl& rhs)
		{
			Release();
			m_paramDelegate = rhs.m_paramDelegate;
			Reference();
			return *this;
		}
	}
}