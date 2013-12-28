///////////////////////////////////////////////////
// File:		Sprite.cpp
// Date:		14th December 2013
// Authors:		Matt Phillips
// Description:	Sprite class
///////////////////////////////////////////////////

#include "renderer/Sprite.h"

namespace ion
{
	namespace render
	{
		Shader* Sprite::sVertexShader = NULL;
		Shader* Sprite::sPixelShader = NULL;
		Shader::ParamHndl<Matrix4> Sprite::sShaderParamWorldViewProjMtx;
		Shader::ParamHndl<Colour> Sprite::sShaderParamDiffuseColour;
		Shader::ParamHndl<Texture> Sprite::sShaderParamSpriteSheet;
		Shader::ParamHndl<Vector2> Sprite::sShaderParamSpriteSheetGridSize;
		Shader::ParamHndl<float> Sprite::sShaderParamSpriteAnimFrame;
		int Sprite::sShaderRefCount = 0;

		Sprite::Sprite(float width, float height, int spriteSheetGridSizeX, int spriteSheetGridSizeY, const Texture* spriteSheet)
		{
			mWidth = width;
			mHeight = height;
			mSpriteSheetGridSizeX = spriteSheetGridSizeX;
			mSpriteSheetGridSizeY = spriteSheetGridSizeY;
			mSpriteSheet = spriteSheet;
			mCurrentFrame = 0;
			mQuadPrimitive = new Quad(NULL, Quad::xy, Vector2(width / 2.0f, height / 2.0f), Vector3());

			if(!sShaderRefCount)
			{
				sVertexShader = Shader::Create();
				sPixelShader = Shader::Create();
				sVertexShader->Load("../shaders/sprite.cgfx", "VertexProgram", Shader::Vertex);
				sPixelShader->Load("../shaders/sprite.cgfx", "FragmentProgram", Shader::Fragment);
				sShaderParamWorldViewProjMtx = sVertexShader->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
				sShaderParamDiffuseColour = sVertexShader->CreateParamHndl<Colour>("gDiffuseColour");
				sShaderParamSpriteSheetGridSize = sVertexShader->CreateParamHndl<Vector2>("gSpriteSheetGridSize");
				sShaderParamSpriteAnimFrame = sVertexShader->CreateParamHndl<float>("gCurrentFrame");
				sShaderParamSpriteSheet = sPixelShader->CreateParamHndl<Texture>("gSpriteSheet");
			}

			sShaderRefCount++;
		}

		Sprite::~Sprite()
		{
			delete mQuadPrimitive;

			sShaderRefCount--;
			if(!sShaderRefCount)
			{
				delete sVertexShader;
				delete sPixelShader;
			}
		}

		void Sprite::SetFrame(int frame)
		{
			mCurrentFrame = frame;
		}

		int Sprite::GetFrame() const
		{
			return mCurrentFrame;
		}

		void Sprite::SetColour(const Colour& colour)
		{
			mColour = colour;
		}

		void Sprite::Render(Renderer& renderer, Camera& camera)
		{
			//Bind shaders
			sVertexShader->Bind();
			sPixelShader->Bind();

			//Set world view projection matrix
			sShaderParamWorldViewProjMtx.SetValue(GetTransform() * camera.GetTransform().GetInverse() * renderer.GetProjectionMatrix());

			//Set colour
			sShaderParamDiffuseColour.SetValue(mColour);
			
			//Set texture
			if(mSpriteSheet)
			{
				sShaderParamSpriteSheet.SetValue(*mSpriteSheet);
			}

			//Set grid size
			sShaderParamSpriteSheetGridSize.SetValue(Vector2((float)mSpriteSheetGridSizeX, (float)mSpriteSheetGridSizeY));

			//Set current anim frame
			sShaderParamSpriteAnimFrame.SetValue((float)mCurrentFrame);

			//Render VBO
			renderer.DrawVertexBuffer(mQuadPrimitive->GetVertexBuffer(), mQuadPrimitive->GetIndexBuffer());

			//Unbind
			sPixelShader->Unbind();
			sVertexShader->Unbind();
		}

		SpriteAnimation::SpriteAnimation(Sprite& sprite)
			: mSprite(sprite)
		{
			mAnimationTrack = NULL;
		}

		void SpriteAnimation::SetAnimationTrack(const AnimationTrackFloat& animationTrack)
		{
			mAnimationTrack = &animationTrack;
		}

		void SpriteAnimation::ApplyFrame(float frame)
		{
			if(mAnimationTrack)
			{
				float value = mAnimationTrack->GetValue(frame);
				mSprite.SetFrame((int)maths::Floor(value));
			}
		}
	}
}