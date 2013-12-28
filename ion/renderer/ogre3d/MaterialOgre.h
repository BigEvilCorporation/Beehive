///////////////////////////////////////////////////
// File:		MaterialOgre.h
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d material implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreMaterial.h>
#include <Ogre/OgreMaterialManager.h>
#include <Ogre/OgreTechnique.h>
#include <Ogre/OgrePass.h>

#include "renderer/Material.h"

#include <string>

namespace ion
{
	//Forward declaration
	class ColourRGB;

	namespace renderer
	{
		namespace ogre
		{
			class MaterialOgre : public Material
			{
			public:
				MaterialOgre();
				~MaterialOgre();

				virtual void SetBlendMode(BlendMode blendMode);
				virtual void SetAmbientColour(const Colour& ambient);
				virtual void SetDiffuseColour(const Colour& diffuse);
				virtual void SetSpecularColour(const Colour& specular);
				virtual void SetEmissiveColour(const Colour& emissive);
				virtual void AddDiffuseMap(Texture* diffuse);
				virtual void SetNormalMap(Texture* normal);
				virtual void SetSpecularMap(Texture* specular);
				virtual void SetOpacityMap(Texture* opacity);
				virtual void AssignVertexColour(ColourType colourType);
				virtual void SetLightingEnabled(bool lighting);
				virtual void SetLightingMode(LightingMode mode);
				virtual void SetReceiveShadows(bool shadows);
				virtual void SetDepthTest(bool enabled);
				virtual void SetDepthWrite(bool enabled);
				virtual void SetCullMode(CullMode cullMode);

				Ogre::MaterialPtr mOgreMaterial;
				Ogre::Technique* mOgreTechnique;
				Ogre::Pass* mOgrePass;
				std::string mOgreMaterialName;
			};
		}

		typedef ogre::MaterialOgre MaterialImpl;
	}
}