#include "Level.h"

Level::Level()
{
	mBackground = NULL;
}

Level::~Level()
{
	if(mBackground)
		delete mBackground;
}

void Level::Load(const std::string& backgroundTexture, const ion::Vector2& screenSize, ion::io::ResourceManager& resourceManager)
{
	mScreenSize = screenSize;
	mBackground = new ion::render::Sprite(ion::render::Sprite::Render2D, 1.0f, 1.0f, 1, 1, backgroundTexture, resourceManager);
}

void Level::Render(ion::render::Renderer& renderer, ion::render::Camera& camera)
{
	mBackground->Render(renderer, camera);
}