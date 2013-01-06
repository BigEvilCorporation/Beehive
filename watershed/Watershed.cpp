///////////////////////////////////////////////////
// File:		Watershed.cpp
// Date:		4th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - Main game class
///////////////////////////////////////////////////

#include "Watershed.h"

#include <sstream>

namespace ws
{
	Watershed::Watershed()
		: ion::framework::Application("Watershed")
	{
		mRenderer = NULL;

		mKeyboard = NULL;
		mMouse = NULL;
		mGamepad = NULL;

		mMouseSensitivity = 0.005f;

		mCameraType = ThirdPerson;
		mThirdPersonCameraHeadOffset.y = 2.5f;
		mThirdPersonCameraHeadOffset.z = 15.0f;
		mFirstPersonCameraMoveSpeed = 10.0f;

		mFrameCount = 0;
	}

	Watershed::~Watershed()
	{
	}

	bool Watershed::Initialise()
	{
		std::stringstream windowTitle;
		windowTitle << "ion::engine - build " << ion::sVersion.Major << "." << ion::sVersion.Minor << "." << ion::sVersion.Build;

		//Create renderer, scenes, cameras and viewports
		mRenderer = new ion::renderer::Renderer(windowTitle.str().c_str(), 1024, 768, false, 8);
		mScene2d = new ion::renderer::Scene();
		mScene3d = new ion::renderer::Scene();
		mCamera2d = new ion::renderer::Camera(*mScene2d);
		mCamera3d = new ion::renderer::Camera(*mScene3d);
		mViewport2d = new ion::renderer::Viewport(*mRenderer, *mCamera2d, 1);
		mViewport3d = new ion::renderer::Viewport(*mRenderer, *mCamera3d, 0);

		//Create camera controllers
		mCameraThirdPerson = new ion::gamekit::CameraThirdPerson();
		mCameraThirdPerson->SetMoveSpeed(5.0f);

		//Create input handlers
		mKeyboard = new ion::input::Keyboard();
		mMouse = new ion::input::Mouse();
		mGamepad = new ion::input::Gamepad();

		//Set input cooperative level with renderer window
		mKeyboard->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Keyboard::Exclusive);
		mMouse->SetCooperativeWindow(mRenderer->GetWindowHandle(), ion::input::Mouse::Exclusive);

		//Add post effects
		mPostEffectBloom = new ion::renderer::PostEffectBloom;
		mPostEffectBloom->AssignToViewport(*mViewport3d);
		mPostEffectBloom->SetBlurWidth(0.01f);
		mPostEffectBloom->SetBlendAlpha(0.15f);

		mPostEffectMotionBlur = new ion::renderer::PostEffectMotionBlur;
		mPostEffectMotionBlur->AssignToViewport(*mViewport3d);
		mPostEffectMotionBlur->SetBlurWeight(0.2f);

		//Set default ambient light and window background colour
		mScene2d->SetAmbientLight(ion::ColourRGB(1.0f, 1.0f, 1.0f));
		mScene3d->SetAmbientLight(ion::ColourRGB(0.5f, 0.5f, 0.5f));
		mViewport2d->SetBackgroundColour(ion::Colour(0.0f, 0.0f, 0.0f));
		mViewport3d->SetBackgroundColour(ion::Colour(0.2f, 0.2f, 0.2f));

		//Create directional light
		mDirectionalLight = new ion::renderer::Light(ion::renderer::Light::Directional, *mScene3d);
		mDirectionalLight->SetDiffuse(ion::ColourRGB(0.8f, 0.8f, 0.8f));
		mDirectionalLight->SetSpecular(ion::ColourRGB(0.9f, 0.9f, 0.9f));
		mDirectionalLight->SetDirection(ion::Vector3(1.0f, -1.0f, 1.0f));

		//Set default camera projections, positions and directions
		mCamera2d->SetProjection(ion::renderer::Camera::Orthographic);
		mCamera2d->SetOrthoDimensions(1.0f, 1.0f);
		mCamera2d->SetAspectRatio(1.0f);
		mCamera3d->SetPosition(ion::Vector3(0.0f, 0.0f, 0.0f));
		mCamera3d->SetProjection(ion::renderer::Camera::Perspective);
		mCamera3d->SetPosition(ion::Vector3(0.0f, 2.0f, 10.0f));
		mCamera3d->LookAt(ion::Vector3(0.0f, 0.0f, 0.0f));

		//Create floor quad
		mFloorQuad = new ion::renderer::Primitive(*mScene3d, ion::renderer::Primitive::Proj3D);
		mFloorQuad->AddQuad(NULL, 15.0f, 10.0f, ion::renderer::Primitive::xz, ion::Vector3());
		mFloorNode = new ion::renderer::SceneNode(*mScene3d);
		mFloorNode->Attach(*mFloorQuad);

		//Create physics world
		mPhysicsWorld = new ion::physics::World;
		mPhysicsWorld->SetGravity(ion::Vector3(0.0f, -9.8f, 0.0f));

		//Create physics floor
		mFloorBody = new ion::physics::Body(ion::physics::Body::InfinitePlane, ion::Vector3(0.0f, 1.0f, 0.0f));
		mPhysicsWorld->AddBody(*mFloorBody);

		//Ensure it's immovable
		mFloorBody->SetMass(0.0f);

		//Create character
		mPlayer = new ws::Creature(*mPhysicsWorld, *mScene3d);

		//Create colour wheel
		mColourWheel = new ws::ColourWheel(*mScene2d, 0.3f, 0.15f);
		mColourWheel->AddEntry(ws::ColourWheel::Entry(0, ion::Colour(1.0f, 0.0f, 0.0f)));
		mColourWheel->AddEntry(ws::ColourWheel::Entry(0, ion::Colour(0.0f, 1.0f, 0.0f)));
		mColourWheel->AddEntry(ws::ColourWheel::Entry(0, ion::Colour(0.0f, 0.0f, 1.0f)));

		/*
		mTestMesh = new ion::renderer::Mesh();
		mTestMesh->Load("meshes/maya_test5.ion.mesh");
		mTestMeshInstance = new ion::renderer::MeshInstance(*mTestMesh, *mScene3d);
		mTestMeshNode = new ion::renderer::SceneNode(*mScene3d);
		mTestMeshNode->Attach(*mTestMeshInstance);
		*/

		/*
		mTestSubMesh = mTestMesh->CreateSubMesh();

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

		mTestMesh->CalculateBounds();

		mTestSkeleton = new ion::renderer::Skeleton();

		ion::renderer::Bone* bone0 = mTestSkeleton->CreateBone("bone0");
		ion::renderer::Bone* bone1 = mTestSkeleton->CreateBone("bone1");

		mTestBones.push_back(bone0);
		mTestBones.push_back(bone1);

		bone0->SetLocalTranslation(ion::Vector3(0.0f,  halfExtents.y, 0.0f));
		bone1->SetLocalTranslation(ion::Vector3(0.0f, -halfExtents.y, 0.0f));

		mTestSubMesh->MapBone(*bone0, 0, 1.0f);

		mTestSubMesh->Finalise();
		mTestMesh->Finalise();

		mTestMesh->SetSkeleton(*mTestSkeleton);
		mTestSkeleton->FixBindingPose();

		mTestMeshInstance = new ion::renderer::MeshInstance(*mTestMesh, *mScene3d);
		mTestMeshInstance->SetCastShadows(false);
		mTestMeshInstance->SetDrawDebugSkeleton(true);

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

		mTestMeshNode = new ion::renderer::SceneNode(*mScene3d);
		mTestMeshNode->Attach(*mTestMeshInstance);
		mTestMeshNode->SetPosition(ion::Vector3(0.0f, 5.0f, 0.0f));

		ion::Matrix4 boneTransform;
		boneTransform.SetTranslation(ion::Vector3(1.0f, 0.0f, 0.5f));
		mTestMeshInstance->SetBoneTransform(*bone1, boneTransform);

		//bone1->Translate(ion::Vector3(0.5f, 0.0f, 0.0f));
		//bone1->Rotate(ion::Quaternion(0.0f, 1.0f, 0.0f, ion::maths::DegreesToRadians(45.0f)));
		*/

		//Initialise FPS timer
		mStartTicks = ion::time::GetSystemTicks();
		
		return true;
	}

	void Watershed::Shutdown()
	{
		if(mKeyboard)
			delete mKeyboard;

		if(mMouse)
			delete mMouse;

		if(mGamepad)
			delete mGamepad;

		if(mViewport3d)
			delete mViewport3d;

		if(mCamera3d)
			delete mCamera3d;

		if(mScene3d)
			delete mScene3d;

		if(mRenderer)
			delete mRenderer;
	}

	bool Watershed::Update(float deltaTime)
	{
		mKeyboard->Update();
		mMouse->Update();
		mGamepad->Update();

		//Get state of escape key and gamepad back/select button, for exit
		bool exit = mKeyboard->KeyDown(DIK_ESCAPE) || mGamepad->ButtonDown(ion::input::Gamepad::SELECT);

		//Toggle camera type with TAB key
		if(mKeyboard->KeyPressedThisFrame(DIK_TAB))
		{
			mCameraType = (CameraType)(((int)mCameraType + 1) % MaxCameraTypes);
		}

		//First person (cutscene/debug) camera, no character control
		if(mCameraType == FirstPerson)
		{
			//Un-register third person camera controller
			mCameraThirdPerson->SetCurrentCamera(mCamera3d);

			//Create camera move vector from WASD state
			ion::Vector3 cameraMoveVector;
			if(mKeyboard->KeyDown(DIK_W))
				cameraMoveVector.z -= mFirstPersonCameraMoveSpeed * deltaTime;
			if(mKeyboard->KeyDown(DIK_S))
				cameraMoveVector.z += mFirstPersonCameraMoveSpeed * deltaTime;
			if(mKeyboard->KeyDown(DIK_A))
				cameraMoveVector.x -= mFirstPersonCameraMoveSpeed * deltaTime;
			if(mKeyboard->KeyDown(DIK_D))
				cameraMoveVector.x += mFirstPersonCameraMoveSpeed * deltaTime;

			//Get mouse deltas
			float mouseDeltaX = (float)mMouse->GetDeltaX();
			float mouseDeltaY = (float)mMouse->GetDeltaY();

			//Move, pitch and yaw camera
			mCamera3d->Move(cameraMoveVector);
			mCamera3d->Pitch(-mouseDeltaY * mMouseSensitivity);
			mCamera3d->Yaw(-mouseDeltaX * mMouseSensitivity);

			//Update character movement
			mPlayer->UpdateMovement(deltaTime, ws::Creature::Idle);
		}
		else if(mCameraType == ThirdPerson)
		{
			//Set third person camera
			mCameraThirdPerson->SetCurrentCamera(mCamera3d);

			//Get character move state
			const float characterJumpForce(10.0f);

			ws::Creature::ControlState controlState = ws::Creature::Idle;

			if(mKeyboard->KeyDown(DIK_A))
			{
				controlState = ws::Creature::Left;
			}
			else if(mKeyboard->KeyDown(DIK_D))
			{
				controlState = ws::Creature::Right;
			}

			if(mKeyboard->KeyPressedThisFrame(DIK_SPACE))
			{
				//Jump
				mPlayer->Jump(characterJumpForce);
			}

			//Update character movement
			mPlayer->UpdateMovement(deltaTime, controlState);

			//Set camera focus target position as character position
			const ion::Matrix4 characterMtx = mPlayer->GetTransform();
			mCameraThirdPerson->SetTargetFocusPosition(characterMtx.GetTranslation());

			//Set camera head target position as character position plus offset
			mCameraThirdPerson->SetTargetHeadPosition(characterMtx.GetTranslation() + mThirdPersonCameraHeadOffset);

			//Update camera
			mCameraThirdPerson->Update(deltaTime);
		}

		//Open/close colour wheel
		if(mMouse->ButtonDown(ion::input::Mouse::MB))
		{
			if(!mColourWheel->IsOpen())
			{
				mColourWheel->Open();
			}
		}
		else
		{
			if(mColourWheel->IsOpen())
			{
				mColourWheel->Close();
			}
		}

		//Update physics world using 10 substeps
		mPhysicsWorld->Step(deltaTime, 10);

		//Update character animation
		mPlayer->UpdateAnimation(deltaTime);

		//Update renderer
		mRenderer->Update(deltaTime);

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

			//TODO: Slow! Draw to screen instead
			//mRenderer->SetWindowTitle(text.str().c_str());

			//Reset timer
			mStartTicks = ion::time::GetSystemTicks();
		}

		return !exit;
	}

	void Watershed::Render()
	{
	}
}
