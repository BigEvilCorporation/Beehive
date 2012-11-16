///////////////////////////////////////////////////
// File:		Texture.cpp
// Date:		19th December 2011
// Authors:		Matt Phillips
// Description:	Texture map
///////////////////////////////////////////////////s

#include "Texture.h"

#if !defined ION_PLUGIN
#include <Ogre/OgreResourceGroupManager.h>
#include <Ogre/OgreImage.h>
#include <Ogre/OgreDataStream.h>
#include <fstream>
#include <sstream>
#endif

namespace ion
{
	namespace renderer
	{
		int Texture::sTextureIndex = 0;

		bool Texture::Load(std::string filename)
		{
			bool result = false;

			//Open file
			std::ifstream file(filename.c_str(),  std::ios::binary | std::ios::in);

			if(file.is_open())
			{
				//Find extension
				std::string::size_type extIndex = filename.find_last_of('.');
				if(extIndex != std::string::npos)
				{
					//Get extension
					Ogre::String extension = filename.substr(extIndex + 1);

					//Create data stream
					Ogre::DataStreamPtr stream(new Ogre::FileStreamDataStream(filename, &file, false));

					//Read image
					Ogre::Image image;
					image.load(stream, extension);

					//Create name
					std::stringstream nameStream;
					nameStream << filename << sTextureIndex++;
					mOgreTextureName = nameStream.str();

					//Convert to Ogre texture
					mOgreTexture = Ogre::TextureManager::getSingleton().loadImage(mOgreTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, image, Ogre::TEX_TYPE_2D, 0, 1.0f);
					
					result = true;
				}

				file.close();
			}

			return result;
		}

		#if !defined ION_PLUGIN
		const std::string& Texture::GetOgreTextureName() const
		{
			return mOgreTextureName;
		}
		#endif
	}
}