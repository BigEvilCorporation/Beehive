#include "core/Application.h"
#include "core/FileSystem.h"
#include "core/maths/Vector.h"
#include "core/maths/Matrix.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/Gamepad.h"
#include "renderer/Renderer.h"
#include "renderer/Scene.h"
#include "renderer/SceneNode.h"
#include "renderer/Camera.h"
#include "renderer/Viewport.h"
#include "renderer/Primitive.h"
#include "renderer/Mesh.h"
#include "renderer/Light.h"
#include "renderer/Material.h"
#include "renderer/Texture.h"
#include "gui/GUIsystem.h"
#include "gui/Scheme.h"
#include "gui/Root.h"
#include "gui/EventListener.h"
#include "gui/Window.h"
#include "gui/Button.h"
#include "gui/StaticText.h"
#include "gui/TextBox.h"
#include "gui/Slider.h"
#include "gui/ListBox.h"
#include "gui/CommonDlg/FileBrowser.h"

class RenderTest : public ion::framework::Application, ion::gui::EventListener
{
public:
	RenderTest();
	~RenderTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	//Callback for UI events
	void OnEvent(const ion::gui::Event& event, const ion::gui::Event::Params& params);

	ion::io::FileSystem* mFileSystem;
	
	ion::renderer::Renderer* mRenderer;
	ion::renderer::Scene* mScene;
	ion::renderer::Camera* mCamera;
	ion::renderer::Viewport* mViewport;
	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;

	ion::gui::GUISystem* mUISystem;
	ion::gui::Scheme* mUIScheme;
	ion::gui::Root* mUIRoot;
	ion::gui::Window* mUIWindow;
	ion::gui::Button* mUIButton;
	ion::gui::StaticText* mUIStaticText;
	ion::gui::TextBox* mUITextBox;
	ion::gui::Slider* mUISlider;
	ion::gui::ListBox* mUIListBox;

	ion::gui::StaticText* mUIFPSText;

	ion::gui::FileBrowser* mFileBrowser;

	ion::renderer::Primitive* mCube;
	ion::renderer::SceneNode* mCubeNode;

	ion::renderer::Primitive* mQuad;
	ion::renderer::SceneNode* mQuadNode;

	ion::renderer::Mesh* mMesh;
	ion::renderer::SceneNode* mMeshNode;

	ion::renderer::Light* mPointLight;
	ion::renderer::Light* mPointLight2;
	ion::renderer::Light* mSpotLight;
	ion::renderer::Light* mDirectionalLights[4];

	ion::renderer::Material* mMaterial;
	ion::renderer::Texture* mTexture;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	std::vector<ion::gui::ListBox::Item*> mListBoxItems;

	u64 mStartTicks;
	u32 mFrameCount;
};