///////////////////////////////////////////////////
// File:		Texture.h
// Date:		19th December 2011
// Authors:		Matt Phillips
// Description:	Texture map
///////////////////////////////////////////////////

#pragma once

#if defined ION_OGRE
#include <Ogre/OgreTextureManager.h>
#endif

namespace ion
{
	namespace renderer
	{
		class Texture
		{
		public:
			bool Load(std::string filename);

			#if defined ION_OGRE
			const std::string& GetOgreTextureName() const;
			#endif

		private:
		#if defined ION_OGRE
			Ogre::TexturePtr mOgreTexture;
			std::string mOgreTextureName;
			static int sTextureIndex;
		#endif
		};
	}
}