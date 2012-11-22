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

	mLightSin = 0.0f;
	mLightCos = 0.0f;

	mFrameCount = 0;
}

RenderTest::~RenderTest()
{
}

bool RenderTest::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create filesystem
	mFileSystem = new ion::io::FileSystem();

	//Create renderer, scene, camera and viewport
	mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 1024, 768, false);
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

	//Get default file device
	ion::io::FileDevice* defaultFileDevice = mFileSystem->GetDefaultFileDevice();

	//Get current directory
	std::string currentDirectory = defaultFileDevice->GetDirectory();

	//Read directory
	std::vector<ion::io::FileDevice::DirectoryItem> directoryItems;
	defaultFileDevice->ReadDirectory(currentDirectory, directoryItems);

	//mScene->Load("scenes\\testscene.ion.scene");

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.1f, 0.1f, 0.1f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Set default shadow cast distance (must be done before adding lights)
	mScene->SetShadowFarDistance(100.0f);

	//Add post effects
	mPostEffectBloom = new ion::renderer::PostEffectBloom;
	mPostEffectBloom->AssignToViewport(*mViewport);

	//Set default camera position and direction
	mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 10.0f));
	mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create material
	mMaterial = new ion::renderer::Material();

	//Load texture
	mTexture = new ion::renderer::Texture();
	mTexture->Load("textures\\placeholderWhite512.png");
	mMaterial->AddDiffuseMap(mTexture);

	mMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetSpecularColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetEmissiveColour(ion::Colour(1.0f, 0.0f, 0.0f, 1.0f));
	mMaterial->SetLightingEnabled(true);
	mMaterial->SetReceiveShadows(true);

	mQuad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mQuad->AddQuad(mMaterial, 10.0f, 5.0f, ion::renderer::Primitive::xz, ion::Vector3());
	mQuad->SetCastShadows(false);
	mQuadNode = new ion::renderer::SceneNode(*mScene);
	mQuadNode->Attach(*mQuad);
	mQuadNode->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));

	ion::renderer::Primitive* quad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	quad->AddQuad(mMaterial, 15.0f, 10.0f, ion::renderer::Primitive::xz, ion::Vector3());
	quad->SetCastShadows(false);
	ion::renderer::SceneNode* sceneNode = new ion::renderer::SceneNode(*mScene);
	sceneNode->Attach(*quad);
	sceneNode->SetPosition(ion::Vector3(-1.0f, -1.0f, -1.0f));

	mCube = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mCube->AddBox(mMaterial, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
	mCube->SetCastShadows(true);
	mCubeNode = new ion::renderer::SceneNode(*mScene);
	mCubeNode->Attach(*mCube);
	mCubeNode->SetPosition(ion::Vector3(-2.0f, 0.5f, 0.0f));

	mSphere = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mSphere->AddSphere(mMaterial, 1.0f, 32, 32);
	mSphere->SetCastShadows(true);
	mSphereNode = new ion::renderer::SceneNode(*mScene);
	mSphereNode->Attach(*mSphere);
	mSphereNode->SetPosition(ion::Vector3(2.0f, 0.5f, 0.0f));

	//mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 5.0f));
	//mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//mCamera->SetDrawMode(ion::renderer::Camera::Wireframe);

	//mMesh = new ion::renderer::Mesh;
	//mMesh->Load("meshes\\testmesh3.ion.mesh");

	//mMeshNode = new ion::renderer::SceneNode(*mScene);
	//mMeshNode->Attach(*mMesh);

	//mPointLight2 = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);

	//mDirectionalLights[0] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[1] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[2] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);
	//mDirectionalLights[3] = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene);

	mPointLight[0] = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	mPointLight[0]->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mPointLight[0]->SetSpecular(ion::ColourRGB(1.0f, 1.0f, 1.0f));
	mPointLight[0]->SetAttenuation(50.0f);

	mPointLightNode[0] = new ion::renderer::SceneNode(*mScene);
	mPointLightNode[0]->Attach(*mPointLight[0]);
	mPointLightNode[0]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));

	mPointLight[1] = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	mPointLight[1]->SetDiffuse(ion::ColourRGB(0.8f, 0.0f, 0.0f));
	mPointLight[1]->SetSpecular(ion::ColourRGB(1.0f, 0.0f, 0.0f));
	mPointLight[1]->SetAttenuation(50.0f);

	mPointLight[2] = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	mPointLight[2]->SetDiffuse(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mPointLight[2]->SetSpecular(ion::ColourRGB(0.0f, 1.0f, 0.0f));
	mPointLight[2]->SetAttenuation(50.0f);

	mPointLight[3] = new ion::renderer::Light(ion::renderer::Light::Point, *mScene);
	mPointLight[3]->SetDiffuse(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mPointLight[3]->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 1.0f));
	mPointLight[3]->SetAttenuation(50.0f);

	mPointLightNode[1] = new ion::renderer::SceneNode(*mScene);
	mPointLightNode[1]->Attach(*mPointLight[1]);
	mPointLightNode[1]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));

	mPointLightNode[2] = new ion::renderer::SceneNode(*mScene);
	mPointLightNode[2]->Attach(*mPointLight[2]);
	mPointLightNode[2]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));

	mPointLightNode[3] = new ion::renderer::SceneNode(*mScene);
	mPointLightNode[3]->Attach(*mPointLight[3]);
	mPointLightNode[3]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));

	mPointLightDebugSphereMaterial = new ion::renderer::Material();
	mPointLightDebugSphereMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetSpecularColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetLightingEnabled(false);
	mPointLightDebugSphereMaterial->SetReceiveShadows(false);

	mPointLightDebugSphere[0] = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mPointLightDebugSphere[0]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);
	mPointLightNode[0]->Attach(*mPointLightDebugSphere[0]);

	mPointLightDebugSphere[1] = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mPointLightDebugSphere[1]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);
	mPointLightNode[1]->Attach(*mPointLightDebugSphere[1]);

	mPointLightDebugSphere[2] = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mPointLightDebugSphere[2]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);
	mPointLightNode[2]->Attach(*mPointLightDebugSphere[2]);

	mPointLightDebugSphere[3] = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mPointLightDebugSphere[3]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);
	mPointLightNode[3]->Attach(*mPointLightDebugSphere[3]);

	/*
	mPointLight2->SetPosition(ion::Vector3(-100.0f, 0.0f, -100.0f));
	mPointLight2->SetDiffuse(ion::ColourRGB(0.0f, 0.7f, 0.0f));
	mPointLight2->SetSpecular(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mPointLight2->SetAttenuation(50.0f, 10.0f, 0.5f);
	*/

	/*
	mSpotLight = new ion::renderer::Light(ion::renderer::Light::Spot, *mScene);
	mSpotLight->SetDiffuse(ion::ColourRGB(0.7f, 0.0f, 0.0f));
	mSpotLight->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mSpotLight->SetAttenuation(10.0f);
	mSpotLight->SetSpotRange(10.0f, 45.0f);
	mSpotLight->CastShadows(true);

	mSpotLightNode = new ion::renderer::SceneNode(*mScene);
	mSpotLightNode->Attach(*mSpotLight);
	mSpotLightNode->SetPosition(ion::Vector3(1.0f, 2.0f, 1.0f));
	mSpotLightNode->SetLookAt(ion::Vector3(0.0f, 0.0f, 0.0f));
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

	mLightSin += 0.5f * deltaTime;
	mLightCos += 0.5f * deltaTime;
	mPointLightNode[0]->SetPosition(ion::Vector3(sin(mLightSin) * 2.0f, 1.5f, cos(mLightCos) * 2.0f));
	mPointLightNode[1]->SetPosition(ion::Vector3(sin(mLightSin + 0.3f) * 2.0f, cos(mLightCos + 0.3f) * 2.0f, cos(mLightCos + 0.3f) * 2.0f));
	mPointLightNode[2]->SetPosition(ion::Vector3(sin(mLightSin + 0.6f) * 3.0f, cos(mLightCos + 0.6f) * 2.0f, 1.0f));
	mPointLightNode[3]->SetPosition(ion::Vector3(cos(mLightCos + 0.9f) * 4.0f, 2.0f, sin(mLightSin + 0.9f) * 2.0f));

	//Update renderer
	mRenderer->Update(deltaTime);

	//Update FPS display
	if(mFrameCount++ % 100 == 0)
	{
		//Get 100-frame end time and diff
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
		mRenderer->SetWindowTitle(text.str().c_str());

		//Reset timer
		mStartTicks = ion::time::GetSystemTicks();
	}

	return !exit;
}

void RenderTest::Render()
{
}