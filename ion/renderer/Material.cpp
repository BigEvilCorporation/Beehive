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
			if(mVertexShader)
			{
				mVertexShader->Bind();
			}

			if(mPixelShader)
			{
				mPixelShader->Bind();
			}

			ApplyShaderParams(worldMtx, viewMtx, projectionMtx);
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

		void Material::SetVertexShader(const io::ResourceHandle<Shader>& shader)
		{
			mVertexShader = shader;

			if(shader)
			{
				mVertexShaderParams.mMatrices.mWorld = shader->CreateParamHndl<Matrix4>("gWorldMatrix");
				mVertexShaderParams.mMatrices.mWorldViewProjection = shader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				mVertexShaderParams.mColours.mAmbient = shader->CreateParamHndl<Colour>("gAmbientColour");
				mVertexShaderParams.mColours.mDiffuse = shader->CreateParamHndl<Colour>("gDiffuseColour");
				mVertexShaderParams.mColours.mSpecular = shader->CreateParamHndl<Colour>("gSpecularColour");
				mVertexShaderParams.mColours.mEmissive = shader->CreateParamHndl<Colour>("gEmissiveColour");
				mVertexShaderParams.mTextures.mDiffuseMap = shader->CreateParamHndl<Texture>("gDiffuseTexture");
				mVertexShaderParams.mTextures.mNormalMap = shader->CreateParamHndl<Texture>("gNormalTexture");
				mVertexShaderParams.mTextures.mOpacityMap = shader->CreateParamHndl<Texture>("gOpacityTexture");
				mVertexShaderParams.mTextures.mSpecularMap = shader->CreateParamHndl<Texture>("gSpecularTexture");
			}
		}

		void Material::SetPixelShader(const io::ResourceHandle<Shader>& shader)
		{
			mPixelShader = shader;

			if(shader)
			{
				mPixelShaderParams.mMatrices.mWorld = shader->CreateParamHndl<Matrix4>("gWorldMatrix");
				mPixelShaderParams.mMatrices.mWorldViewProjection = shader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				mPixelShaderParams.mColours.mAmbient = shader->CreateParamHndl<Colour>("gAmbientColour");
				mPixelShaderParams.mColours.mDiffuse = shader->CreateParamHndl<Colour>("gDiffuseColour");
				mPixelShaderParams.mColours.mSpecular = shader->CreateParamHndl<Colour>("gSpecularColour");
				mPixelShaderParams.mColours.mEmissive = shader->CreateParamHndl<Colour>("gEmissiveColour");
				mPixelShaderParams.mTextures.mDiffuseMap = shader->CreateParamHndl<Texture>("gDiffuseTexture");
				mPixelShaderParams.mTextures.mNormalMap = shader->CreateParamHndl<Texture>("gNormalTexture");
				mPixelShaderParams.mTextures.mOpacityMap = shader->CreateParamHndl<Texture>("gOpacityTexture");
				mPixelShaderParams.mTextures.mSpecularMap = shader->CreateParamHndl<Texture>("gSpecularTexture");
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

			//Shaders
			archive.Serialise(mVertexShader);
			archive.Serialise(mPixelShader);

			//Params
			archive.Serialise(mLightingEnabled);
			archive.Serialise(mReceiveShadows);
			archive.Serialise((u32&)mLightingMode);
			archive.Serialise((u32&)mBlendMode);

			if(archive.GetDirection() == io::Archive::In)
			{
				SetVertexShader(mVertexShader);
				SetPixelShader(mPixelShader);
			}
		}

		/*
		bool Material::Load(std::string filename)
		{
			//Open file for reading
			io::BinaryFile file(filename, ion::io::File::OpenRead);

			if(file.IsOpen())
			{
				//Check header
				if(stricmp(sFileType, file.GetFileType().c_str()) != 0)
				{
					ion::debug::Error("Material::Load() - Not an ion::Material file");
				}

				//Check version
				if(file.GetFileVersion() < sMinFileVersion)
				{
					ion::debug::Error("Material::Load() - Unsupported file version");
				}

				for(io::BinaryFile::Chunk dataChunk = file.GetTrunk().Begin(), dataEnd = file.GetTrunk().End(); dataChunk != dataEnd; dataChunk = dataChunk.Next())
				{
					switch(dataChunk.GetID())
					{
						case ChunkId_BlendMode:
						{
							int blendMode;
							dataChunk.ReadData(&blendMode, sizeof(int));
							SetBlendMode((BlendMode)blendMode);
						}

						case ChunkId_ColourAmbient:
						{
							Colour ambient;
							dataChunk.ReadData(&ambient, sizeof(Colour));
							SetAmbientColour(ambient);
						}

						case ChunkId_ColourDiffuse:
						{
							Colour diffuse;
							dataChunk.ReadData(&diffuse, sizeof(Colour));
							SetDiffuseColour(diffuse);
						}

						case ChunkId_ColourSpecular:
						{
							Colour specular;
							dataChunk.ReadData(&specular, sizeof(Colour));
							SetSpecularColour(specular);
						}

						case ChunkId_ColourEmissive:
						{
							Colour emissive;
							dataChunk.ReadData(&emissive, sizeof(Colour));
							SetEmissiveColour(emissive);
						}

						case ChunkId_MapDiffuse:
						{
							char name[1024] = {0};
							dataChunk.ReadData(&name, dataChunk.GetNumItems());
						}

						case ChunkId_MapNormal:
						{
							char name[1024] = {0};
							dataChunk.ReadData(&name, dataChunk.GetNumItems());
						}

						case ChunkId_MapSpecular:
						{
							char name[1024] = {0};
							dataChunk.ReadData(&name, dataChunk.GetNumItems());
						}

						case ChunkId_MapOpacity:
						{
							char name[1024] = {0};
							dataChunk.ReadData(&name, dataChunk.GetNumItems());
						}
					}
				}

				return true;
			}

			return false;
		}

		u64 Material::Save(std::string filename)
		{
			u64 fileSize = 0;

			//Open file for writing
			io::BinaryFile file(filename, ion::io::File::OpenWrite);

			if(file.IsOpen())
			{
				//Setup header
				file.SetFileType(sFileType);
				file.SetFileVersion(sCurrentFileVersion);

				//Set trunk node id
				file.GetTrunk().SetID(ChunkId_Root);

				//Create blend mode chunk
				io::BinaryFile::Chunk blendModeChunk;
				blendModeChunk.SetID(ChunkId_BlendMode);
				blendModeChunk.SetData(&mBlendMode, sizeof(int), 1);
				file.GetTrunk().AddChild(blendModeChunk);

				//Create colour chunks
				io::BinaryFile::Chunk colourAmbientChunk;
				colourAmbientChunk.SetID(ChunkId_ColourAmbient);
				colourAmbientChunk.SetData(&mAmbientColour, sizeof(Colour), 1);
				file.GetTrunk().AddChild(colourAmbientChunk);

				io::BinaryFile::Chunk colourDiffuseChunk;
				colourDiffuseChunk.SetID(ChunkId_ColourDiffuse);
				colourDiffuseChunk.SetData(&mDiffuseColour, sizeof(Colour), 1);
				file.GetTrunk().AddChild(colourDiffuseChunk);

				io::BinaryFile::Chunk colourSpecularChunk;
				colourSpecularChunk.SetID(ChunkId_ColourSpecular);
				colourSpecularChunk.SetData(&mSpecularColour, sizeof(Colour), 1);
				file.GetTrunk().AddChild(colourSpecularChunk);

				io::BinaryFile::Chunk colourEmissiveChunk;
				colourEmissiveChunk.SetID(ChunkId_ColourEmissive);
				colourEmissiveChunk.SetData(&mEmissiveColour, sizeof(Colour), 1);
				file.GetTrunk().AddChild(colourEmissiveChunk);

				//Create texture chunks
				std::string dummyName = "TEXTURENAME";

				for(int i = 0; i < GetNumDiffuseMaps(); i++)
				{
					io::BinaryFile::Chunk textureDiffuseChunk;
					textureDiffuseChunk.SetID(ChunkId_MapDiffuse);
					textureDiffuseChunk.SetData((void*)dummyName.c_str(), dummyName.size(), dummyName.size());
					file.GetTrunk().AddChild(textureDiffuseChunk);
				}
				
				io::BinaryFile::Chunk textureNormalChunk;
				textureNormalChunk.SetID(ChunkId_MapNormal);
				textureNormalChunk.SetData((void*)dummyName.c_str(), dummyName.size(), dummyName.size());
				file.GetTrunk().AddChild(textureNormalChunk);

				io::BinaryFile::Chunk textureSpecularChunk;
				textureSpecularChunk.SetID(ChunkId_MapSpecular);
				textureSpecularChunk.SetData((void*)dummyName.c_str(), dummyName.size(), dummyName.size());
				file.GetTrunk().AddChild(textureSpecularChunk);

				io::BinaryFile::Chunk textureOpacityChunk;
				textureOpacityChunk.SetID(ChunkId_MapOpacity);
				textureOpacityChunk.SetData((void*)dummyName.c_str(), dummyName.size(), dummyName.size());
				file.GetTrunk().AddChild(textureOpacityChunk);

				//Get total filesize
				fileSize = file.GetTrunk().GetChunkSize() + sizeof(io::BinaryFile::FileHeader);

				//Write file
				file.Write();

				//Close file
				file.Close();
			}

			return fileSize;
		}
		*/

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

		void Material::AddDiffuseMap(const io::ResourceHandle<Texture>& diffuse)
		{
			mDiffuseMaps.push_back(diffuse);
		}

		void Material::SetNormalMap(const io::ResourceHandle<Texture>& normal)
		{
			mNormalMap = normal;
		}

		void Material::SetSpecularMap(const io::ResourceHandle<Texture>& specular)
		{
			mSpecularMap = specular;
		}

		void Material::SetOpacityMap(const io::ResourceHandle<Texture>& opacity)
		{
			mOpacityMap = opacity;
		}

		const io::ResourceHandle<Texture>* Material::GetDiffuseMap(int diffuseMapIdx) const
		{
			const io::ResourceHandle<Texture>* textureHndl = NULL;

			if(diffuseMapIdx < (int)mDiffuseMaps.size())
			{
				textureHndl = &mDiffuseMaps[diffuseMapIdx];
			}

			return textureHndl;
		}

		const io::ResourceHandle<Texture>& Material::GetNormalMap() const
		{
			return mNormalMap;
		}

		const io::ResourceHandle<Texture>& Material::GetSpecularMap() const
		{
			return mSpecularMap;
		}

		const io::ResourceHandle<Texture>& Material::GetOpacityMap() const
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