#include "core/Application.h"
#include "core/Version.h"
#include "core/Debug.h"
#include "core/Thread.h"
#include "core/Time.h"
#include "io/fileSystem.h"
#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/Gamepad.h"
#include "renderer/Renderer.h"
#include "renderer/CoordSys.h"
#include "renderer/Colour.h"
#include "renderer/Shader.h"
//#include "renderer/Scene.h"
//#include "renderer/SceneNode.h"
#include "renderer/Camera.h"
//#include "renderer/Viewport.h"
#include "renderer/Primitive.h"
//#include "renderer/Mesh.h"
//#include "renderer/Light.h"
//#include "renderer/Material.h"
#include "renderer/Texture.h"
#include "renderer/Sprite.h"
//#include "renderer/PostEffect.h"
//#include "renderer/PostEffectBloom.h"

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

	ion::input::Keyboard* mKeyboard;
	ion::input::Mouse* mMouse;
	ion::input::Gamepad* mGamepad;
	
	ion::render::Renderer* mRenderer;
	ion::render::Camera* mCamera;

	ion::render::Primitive* mQuad;
	ion::render::Primitive* mBox;
	ion::render::Primitive* mSphere;

	ion::render::Shader* mShaderDefaultV;
	ion::render::Shader* mShaderDefaultF;

	ion::render::Shader::ParamHndl<ion::Matrix4> mShaderParamWorldMtx;
	ion::render::Shader::ParamHndl<ion::Matrix4> mShaderParamWorldViewProjMtx;
	ion::render::Shader::ParamHndl<ion::render::Texture> mShaderParamTexture;

	ion::render::Texture* mTexture;
	ion::render::Sprite* mSprite;
	ion::render::SpriteAnimation* mSpriteAnim;
	ion::render::AnimationTrackFloat* mSpriteAnimTrack;

	/*
	ion::render::Scene* mScene;
	ion::render::Viewport* mViewport;

	ion::render::PostEffectBloom* mPostEffectBloom;

	ion::render::SceneNode* mCameraNode;

	ion::render::Primitive* mCube;
	ion::render::SceneNode* mCubeNode;

	ion::render::Primitive* mQuad;
	ion::render::SceneNode* mQuadNode;

	ion::render::Primitive* mSphere;
	ion::render::SceneNode* mSphereNode;

	ion::render::Mesh* mMesh;
	ion::render::SceneNode* mMeshNode;

	ion::render::Light* mPointLight[4];
	ion::render::Light* mSpotLight;
	ion::render::Light* mDirectionalLight;

	ion::render::SceneNode* mPointLightNode[4];
	ion::render::SceneNode* mSpotLightNode;
	ion::render::SceneNode* mDirectionalLightNode;

	ion::render::Primitive* mPointLightDebugSphere[4];
	ion::render::Material* mPointLightDebugSphereMaterial;

	ion::render::Material* mMaterial;
	ion::render::Texture* mTexture;
	*/

	float mCameraPitch;
	float mCameraYaw;
	float mCameraSpeed;
	float mMouseSensitivity;

	float mLightSin;
	float mLightCos;

	u64 mStartTicks;
	u32 mFrameCount;
};