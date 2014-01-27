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
		Sprite::Sprite(RenderType renderType, float width, float height, int spriteSheetGridSizeX, int spriteSheetGridSizeY, const std::string& spriteSheet, io::ResourceManager& resourceManager)
		{
			mRenderType = renderType;
			mWidth = width;
			mHeight = height;
			mSpriteSheetGridSizeX = spriteSheetGridSizeX;
			mSpriteSheetGridSizeY = spriteSheetGridSizeY;
			mCurrentFrame = 0;
			mQuadPrimitive = new Quad(Quad::xy, Vector2(width / 2.0f, height / 2.0f), Vector3());

			mVertexShader = resourceManager.GetResource<Shader>("sprite_v.ion.shader");
			mPixelShader = resourceManager.GetResource<Shader>("sprite_p.ion.shader");
			mSpriteSheet = resourceManager.GetResource<Texture>(spriteSheet);
		}

		Sprite::~Sprite()
		{
			delete mQuadPrimitive;
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
			if(mVertexShader && mPixelShader)
			{
				if(!mShaderParams.mWorldViewProjMtx.IsValid())
				{
					mShaderParams.mWorldViewProjMtx = mVertexShader.Get()->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
					mShaderParams.mDiffuseColour = mVertexShader.Get()->CreateParamHndl<Colour>("gDiffuseColour");
					mShaderParams.mSpriteSheetGridSize = mVertexShader.Get()->CreateParamHndl<Vector2>("gSpriteSheetGridSize");
					mShaderParams.mSpriteAnimFrame = mVertexShader.Get()->CreateParamHndl<float>("gCurrentFrame");
					mShaderParams.mSpriteSheet = mPixelShader.Get()->CreateParamHndl<Texture>("gSpriteSheet");
				}

				//Bind shaders
				mVertexShader.Get()->Bind();
				mPixelShader.Get()->Bind();

				Matrix4 worldViewProj;

				if(mRenderType == Render2D)
				{
					worldViewProj = GetTransform();
					worldViewProj.SetTranslation(worldViewProj.GetTranslation() + Vector3(0.0f, -0.01f, 0.0f));
				}
				else
				{
					worldViewProj = GetTransform() * camera.GetTransform().GetInverse() * renderer.GetProjectionMatrix();
				}

				//Set world view projection matrix
				mShaderParams.mWorldViewProjMtx.SetValue(worldViewProj);

				//Set colour
				mShaderParams.mDiffuseColour.SetValue(mColour);
			
				//Set texture
				if(mSpriteSheet)
				{
					mShaderParams.mSpriteSheet.SetValue(*mSpriteSheet);
				}

				//Set grid size
				mShaderParams.mSpriteSheetGridSize.SetValue(Vector2((float)mSpriteSheetGridSizeX, (float)mSpriteSheetGridSizeY));

				//Set current anim frame
				mShaderParams.mSpriteAnimFrame.SetValue((float)mCurrentFrame);

				//Render VBO
				renderer.DrawVertexBuffer(mQuadPrimitive->GetVertexBuffer(), mQuadPrimitive->GetIndexBuffer());

				//Unbind
				mPixelShader.Get()->Unbind();
				mVertexShader.Get()->Unbind();
			}
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