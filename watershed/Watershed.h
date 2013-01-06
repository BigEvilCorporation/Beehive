///////////////////////////////////////////////////
// File:		Watershed.h
// Date:		4th January 2013
// Authors:		Matt Phillips
// Description:	Watershed - Main game class
///////////////////////////////////////////////////

#include "core/Version.h"
#include "core/Debug.h"
#include "core/Time.h"
#include "core/Application.h"
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
#include "renderer/Skeleton.h"
#include "renderer/Light.h"
#include "renderer/Material.h"
#include "renderer/PostEffect.h"
#include "renderer/PostEffectBloom.h"
#include "renderer/PostEffectMotionBlur.h"
#include "physics/World.h"
#include "physics/Body.h"
#include "gamekit/CameraThirdPerson.h"

#include "Creature.h"
#include "ColourWheel.h"

#include <vector>

namespace ws
{
	class Watershed : public ion::framework::Application
	{
	public:
		Watershed();
		~Watershed();

		bool Initialise();
		void Shutdown();
		bool Update(float deltaTime);
		void Render();
		
		ion::renderer::Renderer* mRenderer;
		ion::renderer::Scene* mScene2d;
		ion::renderer::Scene* mScene3d;
		ion::renderer::Camera* mCamera2d;
		ion::renderer::Camera* mCamera3d;
		ion::renderer::Viewport* mViewport2d;
		ion::renderer::Viewport* mViewport3d;
		ion::renderer::PostEffectBloom* mPostEffectBloom;
		ion::renderer::PostEffectMotionBlur* mPostEffectMotionBlur;
		ion::input::Keyboard* mKeyboard;
		ion::input::Mouse* mMouse;
		ion::input::Gamepad* mGamepad;

		ion::renderer::Light* mDirectionalLight;

		ion::physics::World* mPhysicsWorld;

		//Cameras
		enum CameraType { FirstPerson = 0, ThirdPerson, MaxCameraTypes };
		CameraType mCameraType;
		ion::gamekit::CameraThirdPerson* mCameraThirdPerson;
		ion::Vector3 mThirdPersonCameraHeadOffset;
		float mFirstPersonCameraMoveSpeed;

		//Floor
		ion::renderer::Primitive* mFloorQuad;
		ion::renderer::SceneNode* mFloorNode;
		ion::physics::Body* mFloorBody;

		//Character
		ws::Creature* mPlayer;

		//Colour wheel
		ws::ColourWheel* mColourWheel;

		ion::renderer::Mesh* mTestMesh;
		ion::renderer::Mesh::SubMesh* mTestSubMesh;
		ion::renderer::Skeleton* mTestSkeleton;
		std::vector<ion::renderer::Bone*> mTestBones;
		ion::renderer::MeshInstance* mTestMeshInstance;
		ion::renderer::SceneNode* mTestMeshNode;

		float mMouseSensitivity;

		u64 mStartTicks;
		u32 mFrameCount;
	};
}
