///////////////////////////////////////////////////
// File:		Material.h
// Date:		13th December 2011
// Authors:		Matt Phillips
// Description:	Material class and file loader
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "renderer/Colour.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "io/Archive.h"
#include "io/ResourceHandle.h"

#include <string>
#include <vector>

namespace ion
{
	namespace render
	{
		class Material
		{
		public:

			enum BlendMode
			{
				Additive,
				Modulative,
				Replace,
				TransparentAlpha,
				TransparentColour
			};

			enum LightingMode
			{
				Flat,
				Gouraud,
				Phong
			};

			enum CullMode
			{
				None,
				Clockwise,
				CounterClockwise
			};

			enum ColourType
			{
				Ambient,
				Diffuse,
				Specular,
				Emissive
			};

			Material();
			~Material();

			//Bind/unbind
			void Bind(const Matrix4& worldMtx, const Matrix4& viewMtx, const Matrix4& projectionMtx);
			void Unbind();

			//Shaders
			void SetVertexShader(const io::ResourceHandle<Shader>& shader);
			void SetPixelShader(const io::ResourceHandle<Shader>& shader);

			//Colour
			void SetAmbientColour(const Colour& ambient);
			void SetDiffuseColour(const Colour& diffuse);
			void SetSpecularColour(const Colour& specular);
			void SetEmissiveColour(const Colour& emissive);

			const Colour& GetAmbientColour() const;
			const Colour& GetDiffuseColour() const;
			const Colour& GetSpecularColour() const;
			const Colour& GetEmissiveColour() const;

			//Texture maps
			void AddDiffuseMap(const io::ResourceHandle<Texture>& diffuse);
			void SetNormalMap(const io::ResourceHandle<Texture>& normal);
			void SetSpecularMap(const io::ResourceHandle<Texture>& specular);
			void SetOpacityMap(const io::ResourceHandle<Texture>& opacity);

			const io::ResourceHandle<Texture>* GetDiffuseMap(int diffuseMapIdx) const;
			const io::ResourceHandle<Texture>& GetNormalMap() const;
			const io::ResourceHandle<Texture>& GetSpecularMap() const;
			const io::ResourceHandle<Texture>& GetOpacityMap() const;

			int GetNumDiffuseMaps() const;

			//Lighting and shadows
			void SetLightingEnabled(bool lighting);
			void SetLightingMode(LightingMode mode);
			void SetReceiveShadows(bool shadows);

			bool GetLightingEnabled() const;
			LightingMode GetLightingMode() const;
			bool GetReceiveShadows() const;

			//Blend mode
			void SetBlendMode(BlendMode blendMode);
			BlendMode GetBlendMode() const;

			//Depth and culling
			void SetDepthTest(bool enabled);
			void SetDepthWrite(bool enabled);
			void SetCullMode(CullMode cullMode);

			//Serialisation
			static void RegisterSerialiseType(io::Archive& archive);
			void Serialise(io::Archive& archive);

		protected:

			void ApplyShaderParams(const Matrix4& worldMtx, const Matrix4& viewMtx, const Matrix4& projectionMtx);

			Colour mAmbientColour;
			Colour mDiffuseColour;
			Colour mSpecularColour;
			Colour mEmissiveColour;

			std::vector<io::ResourceHandle<Texture>> mDiffuseMaps;
			io::ResourceHandle<Texture> mNormalMap;
			io::ResourceHandle<Texture> mSpecularMap;
			io::ResourceHandle<Texture> mOpacityMap;

			io::ResourceHandle<Shader> mVertexShader;
			io::ResourceHandle<Shader> mPixelShader;

			struct ShaderParams
			{
				struct MatrixParams
				{
					Shader::ParamHndl<Matrix4> mWorld;
					Shader::ParamHndl<Matrix4> mWorldViewProjection;
				} mMatrices;

				struct ColourParams
				{
					Shader::ParamHndl<Colour> mAmbient;
					Shader::ParamHndl<Colour> mDiffuse;
					Shader::ParamHndl<Colour> mSpecular;
					Shader::ParamHndl<Colour> mEmissive;
				} mColours;

				struct TextureParams
				{
					Shader::ParamHndl<Texture> mDiffuseMap;
					Shader::ParamHndl<Texture> mNormalMap;
					Shader::ParamHndl<Texture> mSpecularMap;
					Shader::ParamHndl<Texture> mOpacityMap;
				} mTextures;
			};

			ShaderParams mVertexShaderParams;
			ShaderParams mPixelShaderParams;

			bool mLightingEnabled;
			bool mReceiveShadows;
			LightingMode mLightingMode;

			BlendMode mBlendMode;
		};
	}
}