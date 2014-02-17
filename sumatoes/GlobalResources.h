#pragma once

#include <ion/io/ResourceManager.h>
#include <ion/io/ResourceHandle.h>
#include <ion/io/XML.h>
#include <ion/renderer/Texture.h>

#include <string>
#include <vector>

namespace resources
{
	struct Textures
	{
		static ion::io::ResourceHandle<ion::render::Texture> sLoadingScreenGlobalBg;
		static ion::io::ResourceHandle<ion::render::Texture> sMainMenuBg;
		static ion::io::ResourceHandle<ion::render::Texture> sDemoLevelBg;
	};

	struct Levels
	{
		static std::vector<std::string> sLevelNames;
		static std::string sDemoLevel;
		static std::string sDemoPlayer1;
		static std::string sDemoPlayer2;
		static float sDemoTimeout;
	};

	void Load(ion::io::ResourceManager& resourceManager);
}