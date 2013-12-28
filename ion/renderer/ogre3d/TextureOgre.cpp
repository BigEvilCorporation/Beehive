///////////////////////////////////////////////////
// File:		TextureOgre.cpp
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d texture implementation
///////////////////////////////////////////////////

#pragma once

#include "TextureOgre.h"

#include <Ogre/OgreResourceGroupManager.h>
#include <Ogre/OgreImage.h>
#include <Ogre/OgreDataStream.h>
#include <fstream>
#include <sstream>

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			TextureOgre::TextureOgre()
			{
			}

			TextureOgre::~TextureOgre()
			{
			}

			bool TextureOgre::Load(std::string filename)
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
						static int textureIndex = 0;
						std::stringstream nameStream;
						nameStream << filename << textureIndex++;
						mOgreTextureName = nameStream.str();

						//Convert to Ogre texture
						mOgreTexture = Ogre::TextureManager::getSingleton().loadImage(mOgreTextureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, image, Ogre::TEX_TYPE_2D, 0, 1.0f);
					
						result = true;
					}

					file.close();
				}

				return result;
			}
		}
	}
}