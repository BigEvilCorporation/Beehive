#include "RenderTest.h"

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

	//Create renderer, scene, camera and viewport
	mRenderer = ion::render::Renderer::Create(windowTitle.str(), 1280, 720, false);

	//Create camera
	mCamera = new ion::render::Camera();

	/*
	mScene = new ion::render::Scene();
	mCameraNode = new ion::render::SceneNode();
	mScene->AddSceneNode(*mCameraNode);
	mCameraNode->Attach(*mCamera);
	mViewport = new ion::render::Viewport(*mRenderer, *mCamera);
	*/

	//Create input handlers
	mKeyboard = new ion::input::Keyboard();
	mMouse = new ion::input::Mouse();
	mGamepad = new ion::input::Gamepad();

	mQuad = new ion::render::Quad(NULL, ion::render::Quad::xz, ion::Vector2(0.5f, 0.5f), ion::Vector3(0.0f, 0.0f, 0.0f));
	mBox = new ion::render::Box(NULL, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3(0.0f, 0.0f, 0.0f));
	mSphere = new ion::render::Sphere(NULL, 1.0f, 32, 32);

	//Create and load shaders
	mShaderDefaultV = ion::render::Shader::Create();
	mShaderDefaultF = ion::render::Shader::Create();
	mShaderDefaultV->Load("../shaders/default.cgfx", "VertexProgram", ion::render::Shader::Vertex);
	mShaderDefaultF->Load("../shaders/default.cgfx", "FragmentProgram", ion::render::Shader::Fragment);

	//Get shader params
	mShaderParamWorldMtx = mShaderDefaultV->CreateParamHndl<ion::Matrix4>("gWorldMatrix");
	mShaderParamWorldViewProjMtx = mShaderDefaultV->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");
	mShaderParamTexture = mShaderDefaultF->CreateParamHndl<ion::render::Texture>("gDiffuseTexture");

	//Create and load texture
	mTexture = ion::render::Texture::Create();
	mTexture->Load("../textures/placeholder256.png");

	//Create sprite
	mSprite = new ion::render::Sprite(0.5f, 0.3f, 2, 2, mTexture);
	mSprite->SetPosition(ion::Vector3(1.0f, 1.0f, 1.0f));

	//Create sprite animation
	mSpriteAnim = new ion::render::SpriteAnimation(*mSprite);
	mSpriteAnimTrack = new ion::render::AnimationTrackFloat();
	mSpriteAnimTrack->AddKeyframe(ion::render::Keyframe<float>(0.0f, 0.0f));
	mSpriteAnimTrack->AddKeyframe(ion::render::Keyframe<float>(1.0f, 1.0f));
	mSpriteAnimTrack->AddKeyframe(ion::render::Keyframe<float>(2.0f, 2.0f));
	mSpriteAnimTrack->AddKeyframe(ion::render::Keyframe<float>(3.0f, 3.0f));
	mSpriteAnimTrack->AddKeyframe(ion::render::Keyframe<float>(4.0f, 3.0f));
	mSpriteAnim->SetAnimationTrack(*mSpriteAnimTrack);
	mSpriteAnim->SetLength(4.0f);
	mSpriteAnim->SetPlaybackSpeed(2.0f);
	mSpriteAnim->SetState(ion::render::Animation::Playing);

	//Set input cooperative level with renderer window
	mKeyboard->SetCooperativeWindow(ion::input::Keyboard::Exclusive);
	mMouse->SetCooperativeWindow(ion::input::Mouse::Exclusive);

	/*

	//Create and open scene file stream for reading
	ion::io::File file("..\\scenes\\TestScene1.ion.scene", ion::io::File::OpenRead);

	if(file.IsOpen())
	{
		//Create archive for serialising in
		ion::io::Archive archiveIn(file, ion::io::Archive::In, ion::render::Scene::sSerialiseVersion);

		//Serialise
		archiveIn.Serialise(*mScene);

		//Close file
		file.Close();
	}

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.1f, 0.1f, 0.1f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Set default shadow cast distance (must be done before adding lights)
	mScene->SetShadowFarDistance(100.0f);

	//Set default camera position and direction
	mCameraNode->SetPosition(ion::Vector3(0.0f, 0.0f, 10.0f));
	mCameraNode->SetLookAt(ion::Vector3(0.0f, 0.0f, 0.0f));
	*/

	/*
	//Create material
	mMaterial = new ion::render::Material();

	//Load texture
	mTexture = new ion::render::Texture();
	mTexture->Load("textures\\placeholderWhite512.png");
	mMaterial->AddDiffuseMap(mTexture);

	mMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetSpecularColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetEmissiveColour(ion::Colour(1.0f, 0.0f, 0.0f, 1.0f));
	mMaterial->SetLightingEnabled(true);
	mMaterial->SetReceiveShadows(true);

	mQuad = new ion::render::Primitive(*mScene, ion::render::Primitive::Proj3D);
	mQuad->AddQuad(mMaterial, 10.0f, 5.0f, ion::render::Primitive::xz, ion::Vector3());
	mQuad->SetCastShadows(false);
	mQuadNode = new ion::render::SceneNode(*mScene);
	mQuadNode->Attach(*mQuad);
	mQuadNode->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));

	ion::render::Primitive* quad = new ion::render::Primitive(*mScene, ion::render::Primitive::Proj3D);
	quad->AddQuad(mMaterial, 15.0f, 10.0f, ion::render::Primitive::xz, ion::Vector3());
	quad->SetCastShadows(false);
	ion::render::SceneNode* sceneNode = new ion::render::SceneNode(*mScene);
	sceneNode->Attach(*quad);
	sceneNode->SetPosition(ion::Vector3(-1.0f, -1.0f, -1.0f));

	mCube = new ion::render::Primitive(*mScene, ion::render::Primitive::Proj3D);
	mCube->AddBox(mMaterial, ion::Vector3(0.5f, 0.5f, 0.5f), ion::Vector3());
	mCube->SetCastShadows(true);
	mCubeNode = new ion::render::SceneNode(*mScene);
	mCubeNode->Attach(*mCube);
	mCubeNode->SetPosition(ion::Vector3(-2.0f, 0.5f, 0.0f));

	mSphere = new ion::render::Primitive(*mScene, ion::render::Primitive::Proj3D);
	mSphere->AddSphere(mMaterial, 1.0f, 32, 32);
	mSphere->SetCastShadows(true);
	mSphereNode = new ion::render::SceneNode(*mScene);
	mSphereNode->Attach(*mSphere);
	mSphereNode->SetPosition(ion::Vector3(2.0f, 0.5f, 0.0f));
	*/

	//mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 5.0f));
	//mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//mCamera->SetDrawMode(ion::render::Camera::Wireframe);

	//mMesh = new ion::render::Mesh;
	//mMesh->Load("meshes\\testmesh3.ion.mesh");

	//mMeshNode = new ion::render::SceneNode(*mScene);
	//mMeshNode->Attach(*mMesh);

	//mPointLight2 = new ion::render::Light(ion::render::Light::Point, *mScene);

	//mDirectionalLights[0] = new ion::render::Light(ion::render::Light::Directional, *mScene);
	//mDirectionalLights[1] = new ion::render::Light(ion::render::Light::Directional, *mScene);
	//mDirectionalLights[2] = new ion::render::Light(ion::render::Light::Directional, *mScene);
	//mDirectionalLights[3] = new ion::render::Light(ion::render::Light::Directional, *mScene);

	/*
	mPointLight[0] = new ion::render::Light(ion::render::Light::Point);
	mPointLightNode[0] = new ion::render::SceneNode();
	mScene->AddSceneNode(*mPointLightNode[0]);
	mPointLightNode[0]->Attach(*mPointLight[0]);
	mPointLightNode[0]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));
	mPointLight[0]->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mPointLight[0]->SetSpecular(ion::ColourRGB(1.0f, 1.0f, 1.0f));
	mPointLight[0]->SetAttenuation(100.0f);

	mPointLight[1] = new ion::render::Light(ion::render::Light::Point);
	mPointLightNode[1] = new ion::render::SceneNode();
	mScene->AddSceneNode(*mPointLightNode[1]);
	mPointLightNode[1]->Attach(*mPointLight[1]);
	mPointLightNode[1]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));
	mPointLight[1]->SetDiffuse(ion::ColourRGB(0.8f, 0.0f, 0.0f));
	mPointLight[1]->SetSpecular(ion::ColourRGB(1.0f, 0.0f, 0.0f));
	mPointLight[1]->SetAttenuation(100.0f);

	mPointLight[2] = new ion::render::Light(ion::render::Light::Point);
	mPointLightNode[2] = new ion::render::SceneNode();
	mScene->AddSceneNode(*mPointLightNode[2]);
	mPointLightNode[2]->Attach(*mPointLight[2]);
	mPointLightNode[2]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));
	mPointLight[2]->SetDiffuse(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mPointLight[2]->SetSpecular(ion::ColourRGB(0.0f, 1.0f, 0.0f));
	mPointLight[2]->SetAttenuation(100.0f);

	mPointLight[3] = new ion::render::Light(ion::render::Light::Point);
	mPointLightNode[3] = new ion::render::SceneNode();
	mScene->AddSceneNode(*mPointLightNode[3]);
	mPointLightNode[3]->Attach(*mPointLight[3]);
	mPointLightNode[3]->SetPosition(ion::Vector3(-1.0f, 1.0f, -1.0f));
	mPointLight[3]->SetDiffuse(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mPointLight[3]->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 1.0f));
	mPointLight[3]->SetAttenuation(100.0f);

	mPointLightDebugSphereMaterial = new ion::render::Material();
	mPointLightDebugSphereMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetSpecularColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mPointLightDebugSphereMaterial->SetLightingEnabled(false);
	mPointLightDebugSphereMaterial->SetReceiveShadows(false);

	mPointLightDebugSphere[0] = new ion::render::Primitive(ion::render::Primitive::Proj3D);
	mPointLightNode[0]->Attach(*mPointLightDebugSphere[0]);
	mPointLightDebugSphere[0]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);

	mPointLightDebugSphere[1] = new ion::render::Primitive(ion::render::Primitive::Proj3D);
	mPointLightNode[1]->Attach(*mPointLightDebugSphere[1]);
	mPointLightDebugSphere[1]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);

	mPointLightDebugSphere[2] = new ion::render::Primitive(ion::render::Primitive::Proj3D);
	mPointLightNode[2]->Attach(*mPointLightDebugSphere[2]);
	mPointLightDebugSphere[2]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);

	mPointLightDebugSphere[3] = new ion::render::Primitive(ion::render::Primitive::Proj3D);
	mPointLightNode[3]->Attach(*mPointLightDebugSphere[3]);
	mPointLightDebugSphere[3]->AddSphere(mPointLightDebugSphereMaterial, 0.1f, 6, 6);
	*/

	/*
	mPointLight2->SetPosition(ion::Vector3(-100.0f, 0.0f, -100.0f));
	mPointLight2->SetDiffuse(ion::ColourRGB(0.0f, 0.7f, 0.0f));
	mPointLight2->SetSpecular(ion::ColourRGB(0.0f, 0.8f, 0.0f));
	mPointLight2->SetAttenuation(50.0f, 10.0f, 0.5f);
	*/

	/*
	mSpotLight = new ion::render::Light(ion::render::Light::Spot, *mScene);
	mSpotLight->SetDiffuse(ion::ColourRGB(0.7f, 0.0f, 0.0f));
	mSpotLight->SetSpecular(ion::ColourRGB(0.0f, 0.0f, 0.8f));
	mSpotLight->SetAttenuation(10.0f);
	mSpotLight->SetSpotRange(10.0f, 45.0f);
	mSpotLight->CastShadows(true);

	mSpotLightNode = new ion::render::SceneNode(*mScene);
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

	/*
	if(mViewport)
		delete mViewport;

	if(mCamera)
		delete mCamera;

	if(mScene)
		delete mScene;
		*/

	if(mShaderDefaultV)
		delete mShaderDefaultV;

	if(mShaderDefaultF)
		delete mShaderDefaultF;

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
	exit |= mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

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
	mCameraYaw -= mouseDeltaX * mMouseSensitivity;
	mCameraPitch += mouseDeltaY * mMouseSensitivity;
	ion::Quaternion cameraQuatYaw(mCameraYaw, ion::renderer::coordsys::Up);
	ion::Quaternion cameraQuatPitch(mCameraPitch, ion::renderer::coordsys::Right);
	mCamera->SetOrientation(cameraQuatYaw * cameraQuatPitch);
	mCamera->Move(cameraMoveVector);

	mSpriteAnim->Update(deltaTime);

	/*
	mLightSin += 0.5f * deltaTime;
	mLightCos += 0.5f * deltaTime;
	mPointLightNode[0]->SetPosition(ion::Vector3(sin(mLightSin) * 6.0f, 4.0f, cos(mLightCos) * 4.0f));
	mPointLightNode[1]->SetPosition(ion::Vector3(sin(mLightSin + 0.3f) * 7.0f, cos(mLightCos + 0.3f) * 2.0f, cos(mLightCos + 0.3f) * 4.0f));
	mPointLightNode[2]->SetPosition(ion::Vector3(sin(mLightSin + 0.6f) * 8.0f, cos(mLightCos + 0.6f) * 2.0f, 2.0f));
	mPointLightNode[3]->SetPosition(ion::Vector3(cos(mLightCos + 0.9f) * 9.0f, 4.0f, sin(mLightSin + 0.9f) * 4.0f));
	*/

	//Update renderer
	exit |= !mRenderer->Update(0.0f);

	//Update FPS display
	if(mFrameCount++ % 100 == 0)
	{
		//Get 100-frame end time and diff
		u64 endTicks = ion::time::GetSystemTicks();
		u64 diffTicks = endTicks - mStartTicks;

		//Calc frame time and frames per second
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
	ion::Matrix4 inverseCameraMtx = mCamera->GetTransform().GetInverse();
	ion::Matrix4 objectMtx;
	ion::Matrix4 modelViewProjectionMtx = objectMtx * inverseCameraMtx * mRenderer->GetProjectionMatrix();

	mRenderer->ClearColour();
	mRenderer->ClearDepth();

	/*
	mShaderDefaultV->Bind();
	mShaderDefaultF->Bind();

	mShaderParamWorldMtx.SetValue(inverseCameraMtx);
	mShaderParamWorldViewProjMtx.SetValue(modelViewProjectionMtx);
	mShaderParamTexture.SetValue(*mTexture);

	mRenderer->DrawVertexBuffer(mBox->GetVertexBuffer(), mBox->GetIndexBuffer());

	mShaderDefaultF->Unbind();
	mShaderDefaultV->Unbind();
	*/

	mSprite->Render(*mRenderer, *mCamera);

	mRenderer->SwapBuffers();
}