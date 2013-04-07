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
#include "renderer/Skeleton.h"
#include "renderer/Light.h"
#include "renderer/Material.h"
#include "renderer/Texture.h"
#include "renderer/PostEffect.h"
#include "renderer/PostEffectBloom.h"

class AnimationTest : public ion::framework::Application
{
public:
	AnimationTest();
	~AnimationTest();

	bool Initialise();
	void Shutdown();
	bool Update(float deltaTime);
	void Render();

	ion::io::FileSystem* mFileSystem;
	
	ion::renderer::Renderer* mRenderer;
	ion::renderer::Scene* mScene;
	ion::renderer::Camera* mCamera;
	ion::renderer::Viewport* mViewport;
	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;

	ion::renderer::Material* mMaterial;

	ion::renderer::Primitive* mQuad;
	ion::renderer::SceneNode* mQuadNode;

	ion::renderer::Mesh* mTestMesh;
	ion::renderer::Mesh::SubMesh* mTestSubMesh;
	ion::renderer::Skeleton* mTestSkeleton;
	ion::renderer::SkeletalAnimation* mTestSkeletalAnimation;
	ion::renderer::AnimationTrackTransform* mTestAnimationTrack;
	std::vector<ion::renderer::Bone*> mTestBones;
	ion::renderer::MeshInstance* mTestMeshInstance;
	ion::renderer::SceneNode* mTestMeshNode;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	float mLightSin;
	float mLightCos;

	u64 mStartTicks;
	u32 mFrameCount;
};