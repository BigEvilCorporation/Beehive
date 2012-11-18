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

class RenderTest : public ion::framework::Application
{
public:
	RenderTest();
	~RenderTest();

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

	ion::renderer::Primitive* mCube;
	ion::renderer::SceneNode* mCubeNode;

	ion::renderer::Primitive* mQuad;
	ion::renderer::SceneNode* mQuadNode;

	ion::renderer::Primitive* mSphere;
	ion::renderer::SceneNode* mSphereNode;

	ion::renderer::Mesh* mMesh;
	ion::renderer::SceneNode* mMeshNode;

	ion::renderer::Light* mPointLight[4];
	ion::renderer::Light* mSpotLight;
	ion::renderer::Light* mDirectionalLight;

	ion::renderer::SceneNode* mPointLightNode[4];
	ion::renderer::SceneNode* mSpotLightNode;
	ion::renderer::SceneNode* mDirectionalLightNode;

	ion::renderer::Primitive* mPointLightDebugSphere[4];
	ion::renderer::Material* mPointLightDebugSphereMaterial;

	ion::renderer::Material* mMaterial;
	ion::renderer::Texture* mTexture;

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	float mLightSin;
	float mLightCos;

	u64 mStartTicks;
	u32 mFrameCount;
};