#include "RenderTest.h"
#include "core/Version.h"
#include "core/Debug.h"
#include "core/Thread.h"
#include "core/Colour.h"
#include "core/Time.h"

#include "core/BinaryFile.h"
#include "renderer/Mesh.h"

#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreEntity.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>

#include <sstream>

RenderTest::RenderTest()
	: ion::framework::Application("RenderTest")
{
	mRenderer = NULL;

	mKeyboard = NULL;
	mMouse = NULL;
	mGamepad = NULL;

	mCameraPitch = 0.0f;
	mCameraYaw = 0.0f;
	mCameraSpeed = 10.0f;
	mMouseSensitivity = 0.005f;

	mFrameCount = 0;

	mFileBrowser = NULL;
}

RenderTest::~RenderTest()
{
}

void RenderTest::OnEvent(const ion::gui::Event& event, const ion::gui::Event::Params& params)
{
	if(event == ion::gui::Button::sOnPressedEvent)
	{
		//Button pressed UI event, params will be a standard Widget event
		//TODO: Button should typedef this even if it's unchanged for clarity
		const ion::gui::Widget::EventParams& widgetParams = (const ion::gui::Widget::EventParams&)params;

		if(widgetParams.mWidget == mUIButton)
		{
			mUIStaticText->SetText("Button pressed");

			if(!mFileBrowser)
			{
				//Create file browser
				mFileBrowser = new ion::gui::FileBrowser(*mUIScheme, "File Browser", *mFileSystem->GetDefaultFileDevice(), "\\", NULL);

				//Subscribe to events
				ion::gui::FileBrowser::sBrowserClosedEvent.Subscribe(*this);

				//Add to root UI node
				mUIWindow->AddChild(*mFileBrowser);
			}

			//Show
			mFileBrowser->Show();

			mUISystem->SetActiveRoot(mUIRoot);
		}
	}
	else if(event == ion::gui::FileBrowser::sBrowserClosedEvent)
	{
		//File browser params
		ion::gui::FileBrowser::Params& fileBrowserParams = (ion::gui::FileBrowser::Params&)params;

		if(fileBrowserParams.mWidget == mFileBrowser)
		{
			if(fileBrowserParams.mResult == ion::gui::FileBrowser::Ok)
			{
				//Get filename
				mUIStaticText->SetText(fileBrowserParams.mFullPath);
			}
			else
			{
				mUIStaticText->SetText("File browser cancelled");
			}
		}
	}
	else if(event == ion::gui::ListBox::sOnItemSelectedEvent)
	{
		//Listbox selection UI event, params will be a ListBox event
		const ion::gui::ListBox::Params& listBoxParams = (const ion::gui::ListBox::Params&)params;

		if(listBoxParams.mWidget == mUIListBox)
		{
			std::stringstream text;
			text << "Selected items: ";

			//Iterate through all selected items
			for(std::list<ion::gui::ListBox::Item*>::const_iterator it = listBoxParams.mItems.begin(), end = listBoxParams.mItems.end(); it != end; it++)
			{
				text << (*it)->GetId() << ", ";
			}

			mUIStaticText->SetText(text.str());
		}
	}
	else if(event == ion::gui::Slider::sOnValueChangedEvent)
	{
		//Slider value change event, params will be a slider event
		const ion::gui::Slider::Params& sliderParams = (const ion::gui::Slider::Params&)params;

		if(sliderParams.mWidget == mUISlider)
		{
			std::stringstream text;
			text << "Slider value: " << sliderParams.mValue;
			mUIStaticText->SetText(text.str());
		}
	}
}

bool RenderTest::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create filesystem
	mFileSystem = new ion::io::FileSystem();

	//Create renderer, scene, camera and viewport
	mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 640, 480, false);
	mScene = new ion::renderer::Scene();
	mCamera = new ion::renderer::Camera(*mScene);
	mViewport = new ion::renderer::Viewport(*mRenderer, *mCamera);

	//Create input handlers
	mKeyboard = new ion::input::Keyboard();
	mMouse = new ion::input::Mouse();
	mGamepad = new ion::input::Gamepad();

	//Set input cooperative level with renderer window
	mKeyboard->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Keyboard::Exclusive);
	mMouse->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Mouse::Exclusive);

	//Create UI system
	mUISystem = new ion::gui::GUISystem(*mRenderer);

	//Load UI scheme
	mUIScheme = new ion::gui::Scheme("WindowsLook");

	//Load font
	mUISystem->LoadFont("DejaVuSans-10");

	//Create root widget
	mUIRoot = new ion::gui::Root();

	//Create window
	mUIWindow = new ion::gui::Window("Window", *mUIScheme);
	mUIWindow->SetPosition(ion::Vector2(0.1f, 0.1f));
	mUIWindow->SetSize(ion::Vector2(0.8f, 0.8f));

	//Create button
	mUIButton = new ion::gui::Button("Button", *mUIScheme);
	mUIButton->SetPosition(ion::Vector2(0.1f, 0.1f));
	mUIButton->SetSize(ion::Vector2(0.1f, 0.06f));

	//Create static text
	mUIStaticText = new ion::gui::StaticText("Static text", *mUIScheme);
	mUIStaticText->SetPosition(ion::Vector2(0.1f, 0.2f));
	mUIStaticText->SetSize(ion::Vector2(0.4f, 0.06f));

	//Create editable text
	mUITextBox = new ion::gui::TextBox(*mUIScheme);
	mUITextBox->SetPosition(ion::Vector2(0.10f, 0.3f));
	mUITextBox->SetSize(ion::Vector2(0.3f, 0.06f));

	//Create slider
	mUISlider = new ion::gui::Slider(*mUIScheme, 10.0f, 5.0f, 1.0f);
	mUISlider->SetPosition(ion::Vector2(0.1f, 0.4f));
	mUISlider->SetSize(ion::Vector2(0.6f, 0.1f));

	//Create list box
	mUIListBox = new ion::gui::ListBox(*mUIScheme);
	mUIListBox->SetPosition(ion::Vector2(0.1f, 0.5f));
	mUIListBox->SetSize(ion::Vector2(0.6f, 0.3f));

	//Get default file device
	ion::io::FileDevice* defaultFileDevice = mFileSystem->GetDefaultFileDevice();

	//Get current directory
	std::string currentDirectory = defaultFileDevice->GetDirectory();

	//Read directory
	std::vector<ion::io::FileDevice::DirectoryItem> directoryItems;
	defaultFileDevice->ReadDirectory(currentDirectory, directoryItems);

	//Add listbox items
	for(unsigned int i = 0; i < directoryItems.size(); i++)
	{
		//Generate name
		std::stringstream name;

		if(directoryItems[i].mFileType == ion::io::FileDevice::Directory)
		{
			name << "<" << directoryItems[i].mFilename << ">";
		}
		else
		{
			name << directoryItems[i].mFilename;
		}

		//Create new item
		ion::gui::ListBox::Item* item = new ion::gui::ListBox::Item(name.str(), i);

		//Push to local list for destruction later
		mListBoxItems.push_back(item);

		//Add to listbox
		mUIListBox->AddItem(*item);
	}

	//Create static text for FPS counter
	mUIFPSText = new ion::gui::StaticText("FPS", *mUIScheme);
	mUIFPSText->SetPosition(ion::Vector2(0.55f, 0.1f));
	mUIFPSText->SetSize(ion::Vector2(0.15f, 0.06f));

	//Add widgets to window
	mUIWindow->AddChild(*mUIButton);
	mUIWindow->AddChild(*mUIStaticText);
	mUIWindow->AddChild(*mUITextBox);
	mUIWindow->AddChild(*mUISlider);
	mUIWindow->AddChild(*mUIListBox);
	mUIWindow->AddChild(*mUIFPSText);

	/*
	//Create file browser
	mFileBrowser = new ion::gui::FileBrowser(*mUIScheme, "File Browser", *mFileSystem->GetDefaultFileDevice(), currentDirectory, NULL);

	//Subscribe to events
	ion::gui::FileBrowser::sBrowserClosedEvent.Subscribe(*this);

	//Add to root UI node
	mUIRoot->AddChild(*mFileBrowser);

	//Show
	mFileBrowser->Show();
	*/

	//Add window to root node
	mUIRoot->AddChild(*mUIWindow);

	//Set root (and its tree of elements) as the currently active (i.e. visible, receiving input) set
	//mUISystem->SetActiveRoot(mUIRoot);

	//Subscribe to UI events
	ion::gui::Button::sOnPressedEvent.Subscribe(*this);
	ion::gui::ListBox::sOnItemSelectedEvent.Subscribe(*this);
	ion::gui::Slider::sOnValueChangedEvent.Subscribe(*this);

	//Hide the OS's mouse cursor, since the UI system has its own
	mMouse->ShowCursor(false);

	//mScene->Load("scenes\\testscene.ion.scene");

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.1f, 0.1f, 0.1f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Set default camera position and direction
	mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 10.0f));
	mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create material
	mMaterial = new ion::renderer::Material();
	mMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetDiffuseColour(ion::Colour(0.8f, 0.0f, 0.0f, 1.0f));
	mMaterial->SetSpecularColour(ion::Colour(1.0f, 0.0f, 0.0f, 1.0f));

	//Load texture
	mTexture = new ion::renderer::Texture();
	mTexture->Load("textures\\placeholder256.png");
	mMaterial->AddDiffuseMap(mTexture);

	mCube = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mCube->Begin(mMaterial, ion::renderer::Primitive::Triangle);
	mCube->AddBox(ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
	mCube->End();
	mCubeNode = new ion::renderer::SceneNode(*mScene);
	mCubeNode->Attach(*mCube);

	mQuad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mQuad->Begin(NULL, ion::renderer::Primitive::Triangle);
	mQuad->AddQuad(10.0f, 5.0f, ion::renderer::Primitive::xz, ion::Vector3());
	mQuad->End();
	mQuadNode = new ion::renderer::SceneNode(*mScene);
	mQuadNode->Attach(*mQuad);
	mQuadNode->SetPosition(ion::Vector3(0.0f, -0.5f, 0.0f));

	//mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 5.0f));
	//mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//mCamera->SetDrawMode(ion::renderer::Camera::Wireframe);

	//mMesh = new ion::renderer::Mesh;
	//mMesh->Load("meshes\\testmesh3.ion.mesh");

	//mMeshNode = new ion::renderer::SceneNode(*mScene);
	//mMeshNode->Attach(*mMesh);

	mPointLight = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	//mPointLight2 = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	//mSpotLight = new ion::renderer::Light(ion::renderer::Light::Spot, *mScene);

	//mDirectionalLights[0] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[1] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[2] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[3] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);

	mPointLight->SetPosition(ion::Vector3(2.0f, 2.0f, 2.0f));
	mPointLight->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mPointLight->SetSpecular(ion::ColourRGB(1.0f, 1.0f, 1.0f));
	mPointLight->SetAttenuation(100.0f);

	/*
	mPointLight2->SetPosition(ion::Vector3(-100.0f, 0.0f, -100.0f));
	mPointLight2->SetDiffuse(ion::ColourRGB(0.0f, 0.7f, 0.0f));
	mPointLight2->SetSpecular(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mPointLight2->SetAttenuation(50.0f, 10.0f, 0.5f);

	mSpotLight->SetPosition(ion::Vector3(-100.0f, 0.0f, 100.0f));
	mSpotLight->SetDirection(ion::Vector3(1.0f, 0.0f, 0.0f));
	mSpotLight->SetDiffuse(ion::ColourRGB(0.0f, 0.0f, 0.7f));
	mSpotLight->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mSpotLight->SetAttenuation(50.0f, 10.0f, 0.5f);
	mSpotLight->CastShadows(true);
	*/

	//mDirectionalLights[0]->SetPosition(ion::Vector3(5.0f, 0.0f, 5.0f));
	//mDirectionalLights[0]->SetDirection(ion::Vector3(1.0f, 0.0f, 1.0f));
	//mDirectionalLights[0]->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	//mDirectionalLights[0]->SetSpecular(ion::ColourRGB(0.9f, 0.9f, 0.9f));
	//mDirectionalLights[0]->SetAttenuation(50.0f, 10.0f, 0.5f);

	/*
	mDirectionalLights[1]->SetPosition(ion::Vector3(-100.0f, 0.0f, -100.0f));
	mDirectionalLights[1]->SetDirection(ion::Vector3(1.0f, 0.0f, 1.0f));
	mDirectionalLights[1]->SetDiffuse(ion::ColourRGB(0.0f, 0.7f, 0.0f));
	mDirectionalLights[1]->SetSpecular(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mDirectionalLights[1]->SetAttenuation(50.0f, 10.0f, 0.5f);

	mDirectionalLights[2]->SetPosition(ion::Vector3(100.0f, 0.0f, -100.0f));
	mDirectionalLights[2]->SetDirection(ion::Vector3(-1.0f, 0.0f, 1.0f));
	mDirectionalLights[2]->SetDiffuse(ion::ColourRGB(0.0f, 0.0f, 0.7f));
	mDirectionalLights[2]->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mDirectionalLights[2]->SetAttenuation(50.0f, 10.0f, 0.5f);

	mDirectionalLights[3]->SetPosition(ion::Vector3(-100.0f, 0.0f, 100.0f));
	mDirectionalLights[3]->SetDirection(ion::Vector3(1.0f, 0.0f, -1.0f));
	mDirectionalLights[3]->SetDiffuse(ion::ColourRGB(0.7f, 0.7f, 0.7f));
	mDirectionalLights[3]->SetSpecular(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mDirectionalLights[3]->SetAttenuation(50.0f, 10.0f, 0.5f);
	*/

	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();
	
	return true;
}

void RenderTest::Shutdown()
{
	if(mKeyboard)
		delete mKeyboard;

	if(mMouse)
		delete mMouse;

	if(mGamepad)
		delete mGamepad;

	if(mViewport)
		delete mViewport;

	if(mCamera)
		delete mCamera;

	if(mScene)
		delete mScene;

	if(mRenderer)
		delete mRenderer;
}

bool RenderTest::Update(float deltaTime)
{
	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();
	mUISystem->Update(mKeyboard, mMouse, mGamepad);

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE);
	if(!exit)
		exit = mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

	//Create camera move vector from WASD state
	ion::Vector3 cameraMoveVector;
	if(mKeyboard->KeyDown(DIK_W))
		cameraMoveVector.z -= mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_S))
		cameraMoveVector.z += mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_A))
		cameraMoveVector.x -= mCameraSpeed * deltaTime;
	if(mKeyboard->KeyDown(DIK_D))
		cameraMoveVector.x += mCameraSpeed * deltaTime;

	//Get mouse deltas
	float mouseDeltaX = (float)mMouse->GetDeltaX();
	float mouseDeltaY = (float)mMouse->GetDeltaY();

	//Move, pitch and yaw camera
	mCamera->Move(cameraMoveVector);
	mCamera->Pitch(-mouseDeltaY * mMouseSensitivity);
	mCamera->Yaw(-mouseDeltaX * mMouseSensitivity);

	//Update renderer
	mRenderer->Update(deltaTime);

	//Measure the time of 100 frames and average it for FPS count
	mFrameCount++;
	if(mFrameCount % 100 == 0)
	{
		//Get 1000-frame end time and diff
		u64 endTicks = ion::time::GetSystemTicks();
		u64 diffTicks = endTicks - mStartTicks;

		//Calc frame time and frames per secod
		float frameTime = (float)ion::time::TicksToSeconds(diffTicks) / 100.0f;
		float framesPerSecond = 1.0f / frameTime;

		//Set window title
		std::stringstream text;
		text.setf(std::ios::fixed, std::ios::floatfield);
		text.precision(2);
		text << "FPS: " << framesPerSecond;

		mUIFPSText->SetText(text.str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void RenderTest::Render()
{
}