
#include "GlobalResources.h"

namespace resources
{
	ion::io::ResourceHandle<ion::render::Texture> Textures::sLoadingScreenBg;
	ion::io::ResourceHandle<ion::render::Texture> Textures::sMainMenuBg;
	ion::io::ResourceHandle<ion::render::Texture> Textures::sDemoLevelBg;

	void Load(ion::io::ResourceManager& resourceManager)
	{
		ion::io::XML xml;
		if(xml.Load("../scripts/Game.xml"))
		{
			ion::io::XML* texturesNode = xml.FindChild("Textures");
			if(texturesNode)
			{
				std::string filename;

				if(texturesNode->GetAttribute("loadingScreenBg", filename))
					Textures::sLoadingScreenBg = resourceManager.GetResource<ion::render::Texture>(filename);
				if(texturesNode->GetAttribute("mainMenuBg", filename))
					Textures::sMainMenuBg = resourceManager.GetResource<ion::render::Texture>(filename);
				if(texturesNode->GetAttribute("demoLevelBg", filename))
					Textures::sDemoLevelBg = resourceManager.GetResource<ion::render::Texture>(filename);
			}
		}
	}
}