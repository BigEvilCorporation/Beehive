
#include "GlobalResources.h"

namespace resources
{
	ion::io::ResourceHandle<ion::render::Texture> Textures::sLoadingScreenGlobalBg;
	ion::io::ResourceHandle<ion::render::Texture> Textures::sMainMenuBg;
	ion::io::ResourceHandle<ion::render::Texture> Textures::sDemoLevelBg;

	std::vector<std::string> Levels::sLevelNames;
	std::string Levels::sDemoLevel;
	std::string Levels::sDemoPlayer1;
	std::string Levels::sDemoPlayer2;
	float Levels::sDemoTimeout = 5.0f;

	void Load(ion::io::ResourceManager& resourceManager)
	{
		ion::io::XML xml;
		if(xml.Load("../scripts/Game.xml"))
		{
			ion::io::XML* texturesNode = xml.FindChild("Textures");
			if(texturesNode)
			{
				std::string filename;

				if(texturesNode->GetAttribute("loadingScreenGlobalBg", filename))
					Textures::sLoadingScreenGlobalBg = resourceManager.GetResource<ion::render::Texture>(filename);
				if(texturesNode->GetAttribute("mainMenuBg", filename))
					Textures::sMainMenuBg = resourceManager.GetResource<ion::render::Texture>(filename);
				if(texturesNode->GetAttribute("demoLevelBg", filename))
					Textures::sDemoLevelBg = resourceManager.GetResource<ion::render::Texture>(filename);
			}

			ion::io::XML* demoNode = xml.FindChild("Demo");
			if(demoNode)
			{
				demoNode->GetAttribute("level", Levels::sDemoLevel);
				demoNode->GetAttribute("player1", Levels::sDemoPlayer1);
				demoNode->GetAttribute("player2", Levels::sDemoPlayer2);
				demoNode->GetAttribute("timeout", Levels::sDemoTimeout);
			}
		}
	}
}