///////////////////////////////////////////////////
// File:		Material.cpp
// Date:		13th December 2011
// Authors:		Matt Phillips
// Description:	Material class and file loader
///////////////////////////////////////////////////

#include "renderer/Material.h"

#include "core/debug/Debug.h"

namespace ion
{
	namespace render
	{
		Material::Material()
		{
			//Set default lighting and shadows
			SetLightingEnabled(true);
			SetLightingMode(Phong);
			SetBlendMode(Additive);
			SetReceiveShadows(true);
		}

		Material::~Material()
		{
		}

		void Material::Bind(const Matrix4& worldMtx, const Matrix4& viewMtx, const Matrix4& projectionMtx)
		{
			ApplyShaderParams(worldMtx, viewMtx, projectionMtx);

			if(mVertexShader)
			{
				mVertexShader->Bind();
			}

			if(mPixelShader)
			{
				mPixelShader->Bind();
			}
		}

		void Material::Unbind()
		{
			if(mVertexShader)
			{
				mVertexShader->Unbind();
			}

			if(mPixelShader)
			{
				mPixelShader->Unbind();
			}
		}

		void Material::SetVertexShader(Shader* vertexShader)
		{
			mVertexShader = vertexShader;

			if(vertexShader)
			{
				mVertexShaderParams.mMatrices.mWorld = vertexShader->CreateParamHndl<Matrix4>("gWorldMatrix");
				mVertexShaderParams.mMatrices.mWorldViewProjection = vertexShader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				mVertexShaderParams.mColours.mAmbient = vertexShader->CreateParamHndl<Colour>("gAmbientColour");
				mVertexShaderParams.mColours.mDiffuse = vertexShader->CreateParamHndl<Colour>("gDiffuseColour");
				mVertexShaderParams.mColours.mSpecular = vertexShader->CreateParamHndl<Colour>("gSpecularColour");
				mVertexShaderParams.mColours.mEmissive = vertexShader->CreateParamHndl<Colour>("gEmissiveColour");
				mVertexShaderParams.mTextures.mDiffuseMap = vertexShader->CreateParamHndl<Texture>("gDiffuseTexture");
				mVertexShaderParams.mTextures.mNormalMap = vertexShader->CreateParamHndl<Texture>("gNormalTexture");
				mVertexShaderParams.mTextures.mOpacityMap = vertexShader->CreateParamHndl<Texture>("gOpacityTexture");
				mVertexShaderParams.mTextures.mSpecularMap = vertexShader->CreateParamHndl<Texture>("gSpecularTexture");
			}
		}

		void Material::SetPixelShader(Shader* pixelShader)
		{
			mPixelShader = pixelShader;

			if(pixelShader)
			{
				mPixelShaderParams.mMatrices.mWorld = pixelShader->CreateParamHndl<Matrix4>("gWorldMatrix");
				mPixelShaderParams.mMatrices.mWorldViewProjection = pixelShader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				mPixelShaderParams.mColours.mAmbient = pixelShader->CreateParamHndl<Colour>("gAmbientColour");
				mPixelShaderParams.mColours.mDiffuse = pixelShader->CreateParamHndl<Colour>("gDiffuseColour");
				mPixelShaderParams.mColours.mSpecular = pixelShader->CreateParamHndl<Colour>("gSpecularColour");
				mPixelShaderParams.mColours.mEmissive = pixelShader->CreateParamHndl<Colour>("gEmissiveColour");
				mPixelShaderParams.mTextures.mDiffuseMap = pixelShader->CreateParamHndl<Texture>("gDiffuseTexture");
				mPixelShaderParams.mTextures.mNormalMap = pixelShader->CreateParamHndl<Texture>("gNormalTexture");
				mPixelShaderParams.mTextures.mOpacityMap = pixelShader->CreateParamHndl<Texture>("gOpacityTexture");
				mPixelShaderParams.mTextures.mSpecularMap = pixelShader->CreateParamHndl<Texture>("gSpecularTexture");
			}
		}

		void Material::RegisterSerialiseType(io::Archive& archive)
		{
			archive.RegisterPointerType<Material>();
		}

		void Material::Serialise(io::Archive& archive)
		{
			//Colours
			archive.Serialise(mAmbientColour);
			archive.Serialise(mDiffuseColour);
			archive.Serialise(mSpecularColour);
			archive.Serialise(mEmissiveColour);

			//Textures
			archive.Serialise(mDiffuseMaps);
			archive.Serialise(mNormalMap);
			archive.Serialise(mSpecularMap);
			archive.Serialise(mOpacityMap);

			//TODO
			//Shaders
			//archive.Serialise(mVertexShader);
			//archive.Serialise(mPixelShader);

			//Params
			archive.Serialise(mLightingEnabled);
			archive.Serialise(mReceiveShadows);
			archive.Serialise((u32&)mLightingMode);
			archive.Serialise((u32&)mBlendMode);

			if(archive.GetDirection() == io::Archive::In)
			{
				//SetVertexShader(mVertexShader);
				//SetPixelShader(mPixelShader);
			}
		}

		void Material::SetBlendMode(BlendMode blendMode)
		{
			mBlendMode = blendMode;
		}

		Material::BlendMode Material::GetBlendMode() const
		{
			return mBlendMode;
		}

		void Material::SetAmbientColour(const Colour& ambient)
		{
			mAmbientColour = ambient;
		}

		void Material::SetDiffuseColour(const Colour& diffuse)
		{
			mDiffuseColour = diffuse;
		}

		void Material::SetSpecularColour(const Colour& specular)
		{
			mSpecularColour = specular;
		}

		void Material::SetEmissiveColour(const Colour& emissive)
		{
			mEmissiveColour = emissive;
		}

		const Colour& Material::GetAmbientColour() const
		{
			return mAmbientColour;
		}

		const Colour& Material::GetDiffuseColour() const
		{
			return mDiffuseColour;
		}

		const Colour& Material::GetSpecularColour() const
		{
			return mSpecularColour;
		}

		const Colour& Material::GetEmissiveColour() const
		{
			return mEmissiveColour;
		}

		void Material::AddDiffuseMap(Texture* diffuse)
		{
			mDiffuseMaps.push_back(diffuse);
		}

		void Material::SetNormalMap(Texture* normal)
		{
			mNormalMap = normal;
		}

		void Material::SetSpecularMap(Texture* specular)
		{
			mSpecularMap = specular;
		}

		void Material::SetOpacityMap(Texture* opacity)
		{
			mOpacityMap = opacity;
		}

		Texture* Material::GetDiffuseMap(int diffuseMapIdx) const
		{
			Texture* texture = NULL;

			if(diffuseMapIdx < (int)mDiffuseMaps.size())
			{
				texture = mDiffuseMaps[diffuseMapIdx];
			}

			return texture;
		}

		Texture* Material::GetNormalMap() const
		{
			return mNormalMap;
		}

		Texture* Material::GetSpecularMap() const
		{
			return mSpecularMap;
		}

		Texture* Material::GetOpacityMap() const
		{
			return mOpacityMap;
		}

		int Material::GetNumDiffuseMaps() const
		{
			return mDiffuseMaps.size();
		}

		void Material::SetLightingEnabled(bool lighting)
		{
			mLightingEnabled = lighting;
		}

		bool Material::GetLightingEnabled() const
		{
			return mLightingEnabled;
		}

		void Material::SetLightingMode(LightingMode mode)
		{
			mLightingMode = mode;
		}

		Material::LightingMode Material::GetLightingMode() const
		{
			return mLightingMode;
		}

		void Material::SetReceiveShadows(bool shadows)
		{
			mReceiveShadows = shadows;
		}

		bool Material::GetReceiveShadows() const
		{
			return mReceiveShadows;
		}

		void Material::SetDepthTest(bool enabled)
		{

		}

		void Material::SetDepthWrite(bool enabled)
		{

		}

		void Material::SetCullMode(CullMode cullMode)
		{

		}

		void Material::ApplyShaderParams(const Matrix4& worldMtx, const Matrix4& viewMtx, const Matrix4& projectionMtx)
		{
			Matrix4 worldViewProjMtx = worldMtx * viewMtx * projectionMtx;

			mVertexShaderParams.mMatrices.mWorld.SetValue(worldMtx);
			mVertexShaderParams.mMatrices.mWorldViewProjection.SetValue(worldViewProjMtx);
			mVertexShaderParams.mColours.mAmbient.SetValue(mAmbientColour);
			mVertexShaderParams.mColours.mDiffuse.SetValue(mDiffuseColour);
			mVertexShaderParams.mColours.mSpecular.SetValue(mSpecularColour);
			mVertexShaderParams.mColours.mEmissive.SetValue(mEmissiveColour);

			if(mDiffuseMaps.size() > 0)
			{
				mVertexShaderParams.mTextures.mDiffuseMap.SetValue(*mDiffuseMaps[0]);
			}

			if(mNormalMap)
			{
				mVertexShaderParams.mTextures.mNormalMap.SetValue(*mNormalMap);
			}

			if(mSpecularMap)
			{
				mVertexShaderParams.mTextures.mSpecularMap.SetValue(*mSpecularMap);
			}

			if(mOpacityMap)
			{
				mVertexShaderParams.mTextures.mOpacityMap.SetValue(*mOpacityMap);
			}

			mPixelShaderParams.mMatrices.mWorld.SetValue(worldMtx);
			mPixelShaderParams.mMatrices.mWorldViewProjection.SetValue(worldViewProjMtx);
			mPixelShaderParams.mColours.mAmbient.SetValue(mAmbientColour);
			mPixelShaderParams.mColours.mDiffuse.SetValue(mDiffuseColour);
			mPixelShaderParams.mColours.mSpecular.SetValue(mSpecularColour);
			mPixelShaderParams.mColours.mEmissive.SetValue(mEmissiveColour);

			if(mDiffuseMaps.size() > 0)
			{
				mPixelShaderParams.mTextures.mDiffuseMap.SetValue(*mDiffuseMaps[0]);
			}

			if(mNormalMap)
			{
				mPixelShaderParams.mTextures.mNormalMap.SetValue(*mNormalMap);
			}

			if(mSpecularMap)
			{
				mPixelShaderParams.mTextures.mSpecularMap.SetValue(*mSpecularMap);
			}

			if(mOpacityMap)
			{
				mPixelShaderParams.mTextures.mOpacityMap.SetValue(*mOpacityMap);
			}
		}
	}
}