#include "AnimationTest.h"
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

AnimationTest::AnimationTest()
	: ion::framework::Application("AnimationTest")
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

AnimationTest::~AnimationTest()
{
}

bool AnimationTest::Initialise()
{
	std::stringstream windowTitle;
	windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

	//Create filesystem
	mFileSystem = new ion::io::FileSystem();

	//Create renderer, scene, camera and viewport
	mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 1024, 768, false, 4);
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

	//Set default ambient light and window background colour
	mScene->SetAmbientLight(ion::ColourRGB(0.8f, 0.8f, 0.8f));
	mViewport->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

	//Set default camera position and direction
	mCamera->SetPosition(ion::Vector3(0.0f, 0.0f, 10.0f));
	mCamera->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create material
	mMaterial = new ion::renderer::Material();
	mMaterial->SetAmbientColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetSpecularColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	mMaterial->SetEmissiveColour(ion::Colour(1.0f, 0.0f, 0.0f, 1.0f));
	mMaterial->SetLightingEnabled(true);
	mMaterial->SetReceiveShadows(true);

	//Create floor quad
	mQuad = new ion::renderer::Primitive(*mScene, ion::renderer::Primitive::Proj3D);
	mQuad->AddQuad(mMaterial, 10.0f, 5.0f, ion::renderer::Primitive::xz, ion::Vector3());
	mQuad->SetCastShadows(false);
	mQuadNode = new ion::renderer::SceneNode(*mScene);
	mQuadNode->Attach(*mQuad);
	mQuadNode->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));

	//Create mesh
	mTestMesh = new ion::renderer::Mesh();

	//Create submesh
	mTestSubMesh = mTestMesh->CreateSubMesh();

	//Add vertices and faces
	ion::Vector3 halfExtents(1.0f, 0.5f, 1.0f);
	ion::Vector3 offset(0.0f, 0.0f, 0.0f);

	mTestSubMesh->AddVertex(ion::renderer::Vertex( halfExtents.x, -halfExtents.y, -halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex( halfExtents.x, -halfExtents.y,  halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex( halfExtents.x,  halfExtents.y, -halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex( halfExtents.x,  halfExtents.y,  halfExtents.z));

	mTestSubMesh->AddVertex(ion::renderer::Vertex(-halfExtents.x, -halfExtents.y, -halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex(-halfExtents.x, -halfExtents.y,  halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex(-halfExtents.x,  halfExtents.y, -halfExtents.z));
	mTestSubMesh->AddVertex(ion::renderer::Vertex(-halfExtents.x,  halfExtents.y,  halfExtents.z));

	ion::renderer::Face faces[12] = {{0}};

	faces[0].mIndices[0] = 2; faces[0].mIndices[1] = 1; faces[0].mIndices[2] = 0;
	faces[1].mIndices[0] = 3; faces[1].mIndices[1] = 1; faces[1].mIndices[2] = 2;

	faces[2].mIndices[0] = 7; faces[2].mIndices[1] = 4; faces[2].mIndices[2] = 5;
	faces[3].mIndices[0] = 6; faces[3].mIndices[1] = 4; faces[3].mIndices[2] = 7;

	faces[4].mIndices[0] = 6; faces[4].mIndices[1] = 0; faces[4].mIndices[2] = 4;
	faces[5].mIndices[0] = 2; faces[5].mIndices[1] = 0; faces[5].mIndices[2] = 6;

	faces[6].mIndices[0] = 3; faces[6].mIndices[1] = 5; faces[6].mIndices[2] = 1;
	faces[7].mIndices[0] = 7; faces[7].mIndices[1] = 5; faces[7].mIndices[2] = 3;

	faces[8].mIndices[0] = 7; faces[8].mIndices[1] = 2; faces[8].mIndices[2] = 6;
	faces[9].mIndices[0] = 3; faces[9].mIndices[1] = 2; faces[9].mIndices[2] = 7;

	faces[10].mIndices[0] = 5; faces[10].mIndices[1] = 4; faces[10].mIndices[2] = 1;
	faces[11].mIndices[0] = 1; faces[11].mIndices[1] = 4; faces[11].mIndices[2] = 0;
	
	for(int i = 0; i < 12; i++)
	{
		mTestSubMesh->AddFace(faces[i]);
	}

	//Create skeleton
	mTestSkeleton = new ion::renderer::Skeleton();

	//Create bones
	ion::renderer::Bone* bone0 = mTestSkeleton->CreateBone("bone0");
	ion::renderer::Bone* bone1 = mTestSkeleton->CreateBone("bone1");

	mTestBones.push_back(bone0);
	mTestBones.push_back(bone1);

	//Set bone transforms
	bone0->SetLocalTranslation(ion::Vector3(0.0f,  halfExtents.y, 0.0f));
	bone1->SetLocalTranslation(ion::Vector3(0.0f, -halfExtents.y, 0.0f));

	//Fix current bone transforms as binding pose
	mTestSkeleton->FixBindingPose();

	//Assign skeleton to mesh
	mTestMesh->SetSkeleton(*mTestSkeleton);

	//Map bones to vertices
	mTestSubMesh->MapBone(*bone0, 0, 1.0f);
	mTestSubMesh->MapBone(*bone0, 1, 1.0f);
	mTestSubMesh->MapBone(*bone0, 2, 1.0f);
	mTestSubMesh->MapBone(*bone0, 3, 1.0f);
	mTestSubMesh->MapBone(*bone1, 4, 1.0f);
	mTestSubMesh->MapBone(*bone1, 5, 1.0f);
	mTestSubMesh->MapBone(*bone1, 6, 1.0f);
	mTestSubMesh->MapBone(*bone1, 7, 1.0f);

	//Finalise submesh
	mTestSubMesh->Finalise();

	//Finalise mesh
	mTestMesh->CalculateBounds();
	mTestMesh->Finalise();

	//Create mesh instance
	mTestMeshInstance = new ion::renderer::MeshInstance(*mTestMesh, *mScene);
	mTestMeshInstance->SetCastShadows(false);
	mTestMeshInstance->SetDrawDebugSkeleton(true);

	/*
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 0, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 1, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 2, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 3, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 4, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 5, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 6, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone0, 7, 1.0f);

	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 8, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 9, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 10, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 11, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 12, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 13, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 14, 1.0f);
	mTestMeshInstance->MapBone(*mTestSubMesh, *bone1, 15, 1.0f);
	*/

	//Attach mesh instance to scene node
	mTestMeshNode = new ion::renderer::SceneNode(*mScene);
	mTestMeshNode->Attach(*mTestMeshInstance);
	mTestMeshNode->SetPosition(ion::Vector3(0.0f, 5.0f, 0.0f));

	//Transform bone of mesh instance
	ion::Matrix4 boneTransform;
	boneTransform.SetTranslation(ion::Vector3(3.0f, 1.0f, 0.5f));
	mTestMeshInstance->SetBoneTransform(*bone1, boneTransform);

	//Initialise FPS timer
	mStartTicks = ion::time::GetSystemTicks();
	
	return true;
}

void AnimationTest::Shutdown()
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

bool AnimationTest::Update(float deltaTime)
{
	mKeyboard->Update();
	mMouse->Update();
	mGamepad->Update();

	//Get state of escape key and gamepad back/select button, for exit
	bool exit = mKeyboard->KeyDown(DIK_ESCAPE) || mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

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

void AnimationTest::Render()
{
}