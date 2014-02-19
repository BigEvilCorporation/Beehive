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
#include <ion/io/XML.h>
#include <ion/input/Keyboard.h>
#include <ion/input/Mouse.h>
#include <ion/input/Gamepad.h>
#include <ion/renderer/Renderer.h>
#include <ion/renderer/Colour.h>
#include <ion/renderer/Shader.h>
#include <ion/renderer/Camera.h>
#include <ion/renderer/Primitive.h>
#include <ion/renderer/Texture.h>
#include <ion/renderer/Sprite.h>
#include <ion/renderer/Window.h>
#include <ion/gamekit/StateManager.h>

#include <sstream>

#include "StateLoadingScreenGlobal.h"
#include "StateMainMenu.h"

class Sumatoes : public ion::framework::Application
{
public:
	Sumatoes();
	~Sumatoes();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	///////////////////////////////////////////////////
	// Engine
	///////////////////////////////////////////////////

	ion::io::FileSystem* mFileSystem;
	ion::io::ResourceManager* mResourceManager;

	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;
	
	ion::render::Renderer* mRenderer;
	ion::render::Camera* mCamera;

	ion::gamekit::StateManager mStateManager;

	u64 mStartTicks;
	u32 mFrameCount;

	const int mScreenWidth;
	const int mScreenHeight;

	std::stringstream mWindowTitle;

	///////////////////////////////////////////////////
	// Game
	///////////////////////////////////////////////////

	//Game states
	ion::gamekit::State* mStateLoadingScreenGlobal;
	ion::gamekit::State* mStateMainMenu;

	ion::render::Sprite* mSprite;
	float mSpriteTimer;
	float mSpriteSpeed;
};