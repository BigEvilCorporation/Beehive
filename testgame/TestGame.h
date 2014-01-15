#include <ion/core/bootstrap/Application.h>
#include <ion/core/Version.h>
#include <ion/core/debug/Debug.h>
#include <ion/core/thread/Thread.h>
#include <ion/core/time/Time.h>
#include <ion/io/fileSystem.h>
#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>
#include <ion/io/Resource.h>
#include <ion/io/ResourceManager.h>
#include <ion/io/File.h>
#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Colour.h>
#include <ion/renderer/Shader.h>
//#include <ion/renderer/Scene.h>
//#include <ion/renderer/SceneNode.h>
#include <ion/renderer/Camera.h>
//#include <ion/renderer/Viewport.h>
#include <ion/renderer/Primitive.h>
//#include <ion/renderer/Mesh.h>
//#include <ion/renderer/Light.h>
//#include <ion/renderer/Material.h>
#include <ion/renderer/Texture.h>
#include <ion/renderer/Sprite.h>
//#include <ion/renderer/PostEffect.h>
//#include <ion/renderer/PostEffectBloom.h>
#include <ion/renderer/Window.h>

#include "Ship.h"
#include "Utils.h"

class TestGame : public ion::framework::Application
{
public:
	TestGame();
	~TestGame();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	ion::io::FileSystem* mFileSystem;
	ion::io::ResourceManager* mResourceManager;

	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;
	
	ion::render::Renderer* mRenderer;
	ion::render::Camera* mCamera;

	Ship* mPlayer;

	ion::render::Box* mBoxPrimitive;

	float mMouseSensitivity;

	const float mSceneCylinderRadius;
	const float mSceneCylinderHeight;
	const float mCameraDistance;

	u64 mStartTicks;
	u32 mFrameCount;
};