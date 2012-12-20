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
#include "physics/Character.h"
#include "gamekit/CameraThirdPerson.h"

#include <vector>

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
	ion::renderer::Scene* mScene;
	ion::renderer::Camera* mCamera;
	ion::renderer::Viewport* mViewport;
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
	ion::renderer::Primitive* mCharacter;
	ion::renderer::Material* mCharacterMaterial;
	ion::renderer::SceneNode* mCharacterNode;
	ion::physics::Character* mCharacterBody;
	float mCharacterAcceleration;
	float mCharacterDeceleration;
	float mCharacterCurrSpeed;
	float mCharacterTopSpeed;

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