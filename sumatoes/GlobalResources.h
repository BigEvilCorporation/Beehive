#pragma once

#include <ion/io/ResourceManager.h>
#include <ion/io/ResourceHandle.h>
#include <ion/io/XML.h>
#include <ion/renderer/Texture.h>

namespace resources
{
	struct Textures
	{
		static ion::io::ResourceHandle<ion::render::Texture> sLoadingScreenBg;
		static ion::io::ResourceHandle<ion::render::Texture> sMainMenuBg;
		static ion::io::ResourceHandle<ion::render::Texture> sDemoLevelBg;
	};

	void Load(ion::io::ResourceManager& resourceManager);
}