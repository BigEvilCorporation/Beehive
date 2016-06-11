///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Material.cpp
// Date:		13th December 2011
// Authors:		Matt Phillips
// Description:	Material class and file loader
///////////////////////////////////////////////////

#include "renderer/Material.h"
#include "core/debug/Debug.h"

#include <algorithm>

namespace ion
{
	namespace render
	{
		Material::Material()
		{
			//Set default lighting and shadows
			SetLightingEnabled(true);
			SetLightingMode(ePhong);
			SetBlendMode(eAdditive);
			SetReceiveShadows(true);
		}

		Material::~Material()
		{
		}

		void Material::Bind(const Matrix4& worldMtx, const Matrix4& viewMtx, const Matrix4& projectionMtx)
		{
			ApplyShaderParams(worldMtx, viewMtx, projectionMtx);

			if(m_vertexShader)
			{
				m_vertexShader->Bind();
			}

			if(m_pixelShader)
			{
				m_pixelShader->Bind();
			}
		}

		void Material::Unbind()
		{
			if(m_vertexShader)
			{
				m_vertexShader->Unbind();
			}

			if(m_pixelShader)
			{
				m_pixelShader->Unbind();
			}
		}

		void Material::SetVertexShader(Shader* vertexShader)
		{
			m_vertexShader = vertexShader;

			if(vertexShader)
			{
				m_vertexShaderParams.m_matrices.m_world = vertexShader->CreateParamHndl<Matrix4>("gWorldMatrix");
				m_vertexShaderParams.m_matrices.m_worldViewProjection = vertexShader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				m_vertexShaderParams.m_colours.m_ambient = vertexShader->CreateParamHndl<Colour>("gAmbientColour");
				m_vertexShaderParams.m_colours.m_diffuse = vertexShader->CreateParamHndl<Colour>("gDiffuseColour");
				m_vertexShaderParams.m_colours.m_specular = vertexShader->CreateParamHndl<Colour>("gSpecularColour");
				m_vertexShaderParams.m_colours.m_emissive = vertexShader->CreateParamHndl<Colour>("gEmissiveColour");
				m_vertexShaderParams.m_textures.m_diffuseMap = vertexShader->CreateParamHndl<Texture>("gDiffuseTexture");
				m_vertexShaderParams.m_textures.m_normalMap = vertexShader->CreateParamHndl<Texture>("gNormalTexture");
				m_vertexShaderParams.m_textures.m_opacityMap = vertexShader->CreateParamHndl<Texture>("gOpacityTexture");
				m_vertexShaderParams.m_textures.m_specularMap = vertexShader->CreateParamHndl<Texture>("gSpecularTexture");
			}
		}

		void Material::SetPixelShader(Shader* pixelShader)
		{
			m_pixelShader = pixelShader;

			if(pixelShader)
			{
				m_pixelShaderParams.m_matrices.m_world = pixelShader->CreateParamHndl<Matrix4>("gWorldMatrix");
				m_pixelShaderParams.m_matrices.m_worldViewProjection = pixelShader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				m_pixelShaderParams.m_colours.m_ambient = pixelShader->CreateParamHndl<Colour>("gAmbientColour");
				m_pixelShaderParams.m_colours.m_diffuse = pixelShader->CreateParamHndl<Colour>("gDiffuseColour");
				m_pixelShaderParams.m_colours.m_specular = pixelShader->CreateParamHndl<Colour>("gSpecularColour");
				m_pixelShaderParams.m_colours.m_emissive = pixelShader->CreateParamHndl<Colour>("gEmissiveColour");
				m_pixelShaderParams.m_textures.m_diffuseMap = pixelShader->CreateParamHndl<Texture>("gDiffuseTexture");
				m_pixelShaderParams.m_textures.m_normalMap = pixelShader->CreateParamHndl<Texture>("gNormalTexture");
				m_pixelShaderParams.m_textures.m_opacityMap = pixelShader->CreateParamHndl<Texture>("gOpacityTexture");
				m_pixelShaderParams.m_textures.m_specularMap = pixelShader->CreateParamHndl<Texture>("gSpecularTexture");
			}
		}

		void Material::RegisterSerialiseType(io::Archive& archive)
		{
			archive.RegisterPointerType<Material>();
		}

		void Material::Serialise(io::Archive& archive)
		{
			//Colours
			archive.Serialise(m_ambientColour);
			archive.Serialise(m_diffuseColour);
			archive.Serialise(m_specularColour);
			archive.Serialise(m_emissiveColour);

			//Textures
			archive.Serialise(m_diffuseMaps);
			archive.Serialise(m_normalMap);
			archive.Serialise(m_specularMap);
			archive.Serialise(m_opacityMap);

			//TODO
			//Shaders
			//archive.Serialise(mVertexShader);
			//archive.Serialise(mPixelShader);

			//Params
			archive.Serialise(m_lightingEnabled);
			archive.Serialise(m_receiveShadows);
			archive.Serialise((u32&)m_lightingMode);
			archive.Serialise((u32&)m_blendMode);

			if(archive.GetDirection() == io::Archive::eIn)
			{
				//SetVertexShader(mVertexShader);
				//SetPixelShader(mPixelShader);
			}
		}

		void Material::SetBlendMode(BlendMode blendMode)
		{
			m_blendMode = blendMode;
		}

		Material::BlendMode Material::GetBlendMode() const
		{
			return m_blendMode;
		}

		void Material::SetAmbientColour(const Colour& ambient)
		{
			m_ambientColour = ambient;
		}

		void Material::SetDiffuseColour(const Colour& diffuse)
		{
			m_diffuseColour = diffuse;
		}

		void Material::SetSpecularColour(const Colour& specular)
		{
			m_specularColour = specular;
		}

		void Material::SetEmissiveColour(const Colour& emissive)
		{
			m_emissiveColour = emissive;
		}

		const Colour& Material::GetAmbientColour() const
		{
			return m_ambientColour;
		}

		const Colour& Material::GetDiffuseColour() const
		{
			return m_diffuseColour;
		}

		const Colour& Material::GetSpecularColour() const
		{
			return m_specularColour;
		}

		const Colour& Material::GetEmissiveColour() const
		{
			return m_emissiveColour;
		}

		void Material::AddDiffuseMap(Texture* diffuse)
		{
			m_diffuseMaps.push_back(diffuse);
		}

		void Material::SetDiffuseMap(Texture* diffuse, int diffuseMapIdx)
		{
			m_diffuseMaps.insert(m_diffuseMaps.begin() + diffuseMapIdx, diffuse);
		}

		void Material::SetNormalMap(Texture* normal)
		{
			m_normalMap = normal;
		}

		void Material::SetSpecularMap(Texture* specular)
		{
			m_specularMap = specular;
		}

		void Material::SetOpacityMap(Texture* opacity)
		{
			m_opacityMap = opacity;
		}

		Texture* Material::GetDiffuseMap(int diffuseMapIdx) const
		{
			Texture* texture = NULL;

			if(diffuseMapIdx < (int)m_diffuseMaps.size())
			{
				texture = m_diffuseMaps[diffuseMapIdx];
			}

			return texture;
		}

		Texture* Material::GetNormalMap() const
		{
			return m_normalMap;
		}

		Texture* Material::GetSpecularMap() const
		{
			return m_specularMap;
		}

		Texture* Material::GetOpacityMap() const
		{
			return m_opacityMap;
		}

		int Material::GetNumDiffuseMaps() const
		{
			return m_diffuseMaps.size();
		}

		void Material::SetLightingEnabled(bool lighting)
		{
			m_lightingEnabled = lighting;
		}

		bool Material::GetLightingEnabled() const
		{
			return m_lightingEnabled;
		}

		void Material::SetLightingMode(LightingMode mode)
		{
			m_lightingMode = mode;
		}

		Material::LightingMode Material::GetLightingMode() const
		{
			return m_lightingMode;
		}

		void Material::SetReceiveShadows(bool shadows)
		{
			m_receiveShadows = shadows;
		}

		bool Material::GetReceiveShadows() const
		{
			return m_receiveShadows;
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

			m_vertexShaderParams.m_matrices.m_world.SetValue(worldMtx);
			m_vertexShaderParams.m_matrices.m_worldViewProjection.SetValue(worldViewProjMtx);
			m_vertexShaderParams.m_colours.m_ambient.SetValue(m_ambientColour);
			m_vertexShaderParams.m_colours.m_diffuse.SetValue(m_diffuseColour);
			m_vertexShaderParams.m_colours.m_specular.SetValue(m_specularColour);
			m_vertexShaderParams.m_colours.m_emissive.SetValue(m_emissiveColour);

			if(m_diffuseMaps.size() > 0)
			{
				m_vertexShaderParams.m_textures.m_diffuseMap.SetValue(*m_diffuseMaps[0]);
			}

			if(m_normalMap)
			{
				m_vertexShaderParams.m_textures.m_normalMap.SetValue(*m_normalMap);
			}

			if(m_specularMap)
			{
				m_vertexShaderParams.m_textures.m_specularMap.SetValue(*m_specularMap);
			}

			if(m_opacityMap)
			{
				m_vertexShaderParams.m_textures.m_opacityMap.SetValue(*m_opacityMap);
			}

			m_pixelShaderParams.m_matrices.m_world.SetValue(worldMtx);
			m_pixelShaderParams.m_matrices.m_worldViewProjection.SetValue(worldViewProjMtx);
			m_pixelShaderParams.m_colours.m_ambient.SetValue(m_ambientColour);
			m_pixelShaderParams.m_colours.m_diffuse.SetValue(m_diffuseColour);
			m_pixelShaderParams.m_colours.m_specular.SetValue(m_specularColour);
			m_pixelShaderParams.m_colours.m_emissive.SetValue(m_emissiveColour);

			if(m_diffuseMaps.size() > 0)
			{
				m_pixelShaderParams.m_textures.m_diffuseMap.SetValue(*m_diffuseMaps[0]);
			}

			if(m_normalMap)
			{
				m_pixelShaderParams.m_textures.m_normalMap.SetValue(*m_normalMap);
			}

			if(m_specularMap)
			{
				m_pixelShaderParams.m_textures.m_specularMap.SetValue(*m_specularMap);
			}

			if(m_opacityMap)
			{
				m_pixelShaderParams.m_textures.m_opacityMap.SetValue(*m_opacityMap);
			}
		}
	}
}