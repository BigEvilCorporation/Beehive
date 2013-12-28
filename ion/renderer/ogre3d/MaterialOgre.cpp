///////////////////////////////////////////////////
// File:		MaterialOgre.cpp
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d material implementation
///////////////////////////////////////////////////

#include "MaterialOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			MaterialOgre::MaterialOgre()
			{
				static int materialIndex = 0;
				std::stringstream name;
				name << "Material_" << materialIndex++;
				mOgreMaterialName = name.str();
				mOgreMaterial = Ogre::MaterialManager::getSingleton().create(mOgreMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
				mOgreMaterial->load();
				mOgreTechnique = mOgreMaterial->getTechnique(0);
				mOgrePass = mOgreTechnique->getPass(0);
			}

			MaterialOgre::~MaterialOgre()
			{
			}

			void MaterialOgre::SetBlendMode(BlendMode blendMode)
			{
				Material::SetBlendMode(blendMode);

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
			}

			void MaterialOgre::SetAmbientColour(const Colour& ambient)
			{
				Material::SetAmbientColour(ambient);
				mOgrePass->setAmbient(ambient.r, ambient.g, ambient.b);
			}

			void MaterialOgre::SetDiffuseColour(const Colour& diffuse)
			{
				Material::SetDiffuseColour(diffuse);
				mOgrePass->setDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			}

			void MaterialOgre::SetSpecularColour(const Colour& specular)
			{
				Material::SetSpecularColour(specular);

				mOgrePass->setSpecular(specular.r, specular.g, specular.b, 1.0f);
				mOgrePass->setShininess(specular.a);
			}

			void MaterialOgre::SetEmissiveColour(const Colour& emissive)
			{
				Material::SetEmissiveColour(emissive);
				mOgrePass->setSelfIllumination(emissive.r, emissive.g, emissive.b);
			}

			void MaterialOgre::AddDiffuseMap(Texture* diffuse)
			{
				Material::AddDiffuseMap(diffuse);

				Ogre::TextureUnitState* textureState = mOgrePass->createTextureUnitState(((MaterialOgre*)diffuse)->mOgreTextureName);
				textureState->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
				textureState->setTextureAnisotropy(8);
			}

			void MaterialOgre::SetNormalMap(Texture* normal)
			{
				Material::SetNormalMap(normal);
			}

			void MaterialOgre::SetSpecularMap(Texture* specular)
			{
				Material::SetSpecularMap(specular);
			}

			void MaterialOgre::SetOpacityMap(Texture* opacity)
			{
				Material::SetOpacityMap(opacity);
			}

			void MaterialOgre::AssignVertexColour(ColourType colourType)
			{
				Material::AssignVertexColour(colourType);

				int ogreColourType = Ogre::TVC_AMBIENT;

				switch(colourType)
				{
				case Ambient:
					ogreColourType = Ogre::TVC_AMBIENT;
					break;
				case Diffuse:
					ogreColourType = Ogre::TVC_DIFFUSE;
					break;
				case Specular:
					ogreColourType = Ogre::TVC_SPECULAR;
					break;
				case Emissive:
					ogreColourType = Ogre::TVC_EMISSIVE;
					break;
				}

				mOgrePass->setVertexColourTracking(ogreColourType);
			}

			void MaterialOgre::SetLightingEnabled(bool lighting)
			{
				Material::SetLightingEnabled(lighting);
				mOgreMaterial->setLightingEnabled(lighting);
			}

			void MaterialOgre::SetLightingMode(LightingMode mode)
			{
				Material::SetLightingMode(mode);

				Ogre::ShadeOptions ogreShadeMode = Ogre::SO_FLAT ;

				switch(mode)
				{
				case Flat:
					ogreShadeMode = Ogre::SO_FLAT;
					break;
				case Gouraud:
					ogreShadeMode = Ogre::SO_GOURAUD;
					break;
				case Phong:
					ogreShadeMode = Ogre::SO_PHONG;
					break;
				};

				mOgreMaterial->setShadingMode(ogreShadeMode);
			}

			void MaterialOgre::SetReceiveShadows(bool shadows)
			{
				Material::SetReceiveShadows(shadows);
				mOgreMaterial->setReceiveShadows(shadows);
			}

			void MaterialOgre::SetDepthTest(bool enabled)
			{
				Material::SetDepthTest(enabled);
				mOgrePass->setDepthCheckEnabled(enabled);
			}

			void MaterialOgre::SetDepthWrite(bool enabled)
			{
				Material::SetDepthWrite(enabled);
				mOgrePass->setDepthWriteEnabled(enabled);
			}

			void MaterialOgre::SetCullMode(CullMode cullMode)
			{
				Material::SetCullMode(cullMode);

				Ogre::CullingMode ogreCullMode = Ogre::CULL_NONE;

				switch(cullMode)
				{
					case None:
						ogreCullMode = Ogre::CULL_NONE;
						break;
					case Clockwise:
						ogreCullMode = Ogre::CULL_CLOCKWISE;
						break;
					case CounterClockwise:
						ogreCullMode = Ogre::CULL_ANTICLOCKWISE;
						break;
				}

				mOgrePass->setCullingMode(ogreCullMode);
			}
		}
	}
}