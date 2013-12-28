///////////////////////////////////////////////////
// File:		TextureOgre.h
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d texture implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreTextureManager.h>

#include "renderer/Texture.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class TextureOgre : public Texture
			{
			public:
				TextureOgre();
				~TextureOgre();

				virtual bool Load(std::string filename);

				Ogre::TexturePtr mOgreTexture;
				std::string mOgreTextureName;
			};
		}

		typedef ogre::TextureOgre TextureImpl;
	}
}