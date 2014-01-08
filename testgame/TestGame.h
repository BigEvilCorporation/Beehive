#include "core/bootstrap/Application.h"
#include "core/Version.h"
#include "core/debug/Debug.h"
#include "core/thread/Thread.h"
#include "core/time/Time.h"
#include "io/fileSystem.h"
#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "io/Resource.h"
#include "io/ResourceManager.h"
#include "io/File.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/Gamepad.h"
#include "renderer/Renderer.h"
#include "renderer/Colour.h"
#include "renderer/Shader.h"
//#include "renderer/Scene.h"
//#include "renderer/SceneNode.h"
#include "renderer/Camera.h"
//#include "renderer/Viewport.h"
#include "renderer/Primitive.h"
//#include "renderer/Mesh.h"
//#include "renderer/Light.h"
//#include "renderer/Material.h"
#include "renderer/Texture.h"
#include "renderer/Sprite.h"
//#include "renderer/PostEffect.h"
//#include "renderer/PostEffectBloom.h"

#include "Ship.h"

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

	ion::io::ResourceHandle<ion::render::Shader> mVertexShader;
	ion::io::ResourceHandle<ion::render::Shader> mPixelShader;
	ion::io::ResourceHandle<ion::render::Texture> mTexture;

	float mMouseSensitivity;

	const float mSceneCylinderRadius;
	const float mSceneCylinderHeight;
	const float mCameraDistance;

	u64 mStartTicks;
	u32 mFrameCount;
};