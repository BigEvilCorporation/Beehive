///////////////////////////////////////////////////
// File:		Material.cpp
// Date:		13th December 2011
// Authors:		Matt Phillips
// Description:	Material class and file loader
///////////////////////////////////////////////////

#include "Material.h"
#include "Texture.h"

#include "../Core/Debug.h"

namespace ion
{
	namespace renderer
	{
		//Minimum file version supported by Material::Load()
		const int Material::sMinFileVersion = 1;

		//File version output by Material::Save()
		const int Material::sCurrentFileVersion = 1;

		//Material file header type
		const char* Material::sFileType = "ion::material";

		//For unique name generation
		int Material::sMaterialIndex = 0;

		//Chunk IDs
		const u32 Material::sChunkIds[NumChunkIds] = 
		{
			Hash("MaterialRoot"),
			Hash("BlendMode"),
			Hash("ColourAmbient"),
			Hash("ColourDiffuse"),
			Hash("ColourSpecular"),
			Hash("ColourEmissive"),
			Hash("MapDiffuse"),
			Hash("MapNormal"),
			Hash("MapSpecular"),
			Hash("MapOpacity")
		};

		Material::Material()
		{
			#if !defined ION_PLUGIN
			std::stringstream name;
			name << "Material_" << sMaterialIndex++;
			mOgreMaterialName = name.str();
			mOgreMaterial = Ogre::MaterialManager::getSingleton().create(mOgreMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mOgreTechnique = mOgreMaterial->getTechnique(0);
			mOgrePass = mOgreTechnique->getPass(0);
			#endif
		}

		Material::~Material()
		{
		}

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

		void Material::SetBlendMode(BlendMode blendMode)
		{

			#if !defined ION_PLUGIN
			Ogre::SceneBlendType ogreBlendType = Ogre::SBT_REPLACE;

			switch(blendMode)
			{
			case Additive:
				ogreBlendType = Ogre::SBT_ADD;
				break;

			case Modulative:
				ogreBlendType = Ogre::SBT_MODULATE;
				break;

			case Replace:
				ogreBlendType = Ogre::SBT_REPLACE;
				break;

			case TransparentAlpha:
				ogreBlendType = Ogre::SBT_TRANSPARENT_ALPHA;
				break;

			case TransparentColour:
				ogreBlendType = Ogre::SBT_TRANSPARENT_COLOUR;
				break;
			};

			mOgreMaterial->getTechnique(0)->getPass(0)->setSceneBlending(ogreBlendType);
			#endif

			mBlendMode = blendMode;
		}

		Material::BlendMode Material::GetBlendMode() const
		{
			return mBlendMode;
		}

		void Material::SetAmbientColour(const Colour& ambient)
		{
			#if !defined ION_PLUGIN
			mOgreMaterial->getTechnique(0)->getPass(0)->setAmbient(ambient.r, ambient.g, ambient.b);
			#endif

			mAmbientColour = ambient;
		}

		void Material::SetDiffuseColour(const Colour& diffuse)
		{
			#if !defined ION_PLUGIN
			mOgreMaterial->getTechnique(0)->getPass(0)->setDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			#endif

			mDiffuseColour = diffuse;
		}

		void Material::SetSpecularColour(const Colour& specular)
		{
			#if !defined ION_PLUGIN
			mOgreMaterial->getTechnique(0)->getPass(0)->setSpecular(specular.r, specular.g, specular.b, 1.0f);
			mOgreMaterial->getTechnique(0)->getPass(0)->setShininess(specular.a);
			#endif

			mSpecularColour = specular;
		}

		void Material::SetEmissiveColour(const Colour& emissive)
		{
			#if !defined ION_PLUGIN
			mOgreMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(emissive.r, emissive.g, emissive.b);
			#endif

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

			#if !defined ION_PLUGIN
			Ogre::TextureUnitState* textureState = mOgrePass->createTextureUnitState(diffuse->GetOgreTextureName());
			#endif
		}

		void Material::SetNormalMap(Texture* normal)
		{
			mNormalMap = normal;

			#if !defined ION_PLUGIN
			//Ogre::TextureUnitState* textureState = mOgrePass->createTextureUnitState(diffuse->GetOgreTextureName());
			//textureState->setContentType(Ogre::TextureUnitState::Conte
			#endif
		}

		void Material::SetSpecularMap(Texture* specular)
		{
			mSpecularMap = specular;
		}

		void Material::SetOpacityMap(Texture* opacity)
		{
			mOpacityMap = opacity;
		}

		const Texture* Material::GetDiffuseMap(int diffuseMapIdx) const
		{
			return mDiffuseMaps[diffuseMapIdx];
		}

		const Texture* Material::GetNormalMap() const
		{
			return mNormalMap;
		}

		const Texture* Material::GetSpecularMap() const
		{
			return mSpecularMap;
		}

		const Texture* Material::GetOpacityMap() const
		{
			return mOpacityMap;
		}

		int Material::GetNumDiffuseMaps() const
		{
			return mDiffuseMaps.size();
		}
	}
}