#include <ion/maths/Vector.h>
#include <ion/io/ResourceManager.h>
#include <ion/renderer/Sprite.h>
#include <ion/renderer/Renderer.h>

#include <string>

class Level
{
public:
	Level();
	~Level();

	void Load(const std::string& backgroundTexture, const ion::Vector2& screenSize, ion::io::ResourceManager& resourceManager);

	void Render(ion::render::Renderer& renderer, ion::render::Camera& camera);

protected:
	ion::Vector2 mScreenSize;
	ion::render::Sprite* mBackground;
};