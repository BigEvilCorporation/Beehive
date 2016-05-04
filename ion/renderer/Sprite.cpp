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
		Sprite::Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteGridSizeX, int spriteGridSizeY, const std::string& sprite, io::ResourceManager& resourceManager)
		{
			mRenderType = renderType;
			mSize = size;
			mDrawDepth = drawDepth;
			mSpriteSheetGridSizeX = spriteGridSizeX;
			mSpriteSheetGridSizeY = spriteGridSizeY;
			mCurrentFrame = 0;
			mQuadPrimitive = new Quad(Quad::xy, Vector2(1.0f, 1.0f));

			mVertexShader = resourceManager.GetResource<Shader>("sprite_v.ion.shader");
			mPixelShader = resourceManager.GetResource<Shader>("sprite_p.ion.shader");
			mSpriteSheet = resourceManager.GetResource<Texture>(sprite);
		}

		Sprite::Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteGridSizeX, int spriteGridSizeY, const io::ResourceHandle<Texture>& sprite, io::ResourceManager& resourceManager)
		{
			mRenderType = renderType;
			mSize = size;
			mDrawDepth = drawDepth;
			mSpriteSheetGridSizeX = spriteGridSizeX;
			mSpriteSheetGridSizeY = spriteGridSizeY;
			mCurrentFrame = 0;
			mQuadPrimitive = new Quad(Quad::xy, Vector2(1.0f, 1.0f));

			mVertexShader = resourceManager.GetResource<Shader>("sprite_v.ion.shader");
			mPixelShader = resourceManager.GetResource<Shader>("sprite_p.ion.shader");
			mSpriteSheet = sprite;
		}

		Sprite::~Sprite()
		{
			delete mQuadPrimitive;
		}

		void Sprite::SetFrame(int frame)
		{
			char text[128] = {0};
			sprintf(text, "%i\n", frame);
			ion::debug::Log(text);
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
			if(mVertexShader && mPixelShader && mSpriteSheet)
			{
				if(!mShaderParams.mWorldViewProjMtx.IsValid())
				{
					mShaderParams.mWorldViewProjMtx = mVertexShader.Get()->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
					mShaderParams.mDiffuseColour = mVertexShader.Get()->CreateParamHndl<Colour>("gDiffuseColour");
					mShaderParams.mSpriteSheetGridSize = mVertexShader.Get()->CreateParamHndl<Vector2>("gSpriteSheetGridSize");
					mShaderParams.mSpriteAnimFrame = mVertexShader.Get()->CreateParamHndl<float>("gCurrentFrame");
					mShaderParams.mSpriteSheet = mPixelShader.Get()->CreateParamHndl<Texture>("gSpriteSheet");
				}

				Matrix4 drawMatrix;

				if(mRenderType == Render2D)
				{
					Vector3 position = Vector3(GetTransform().GetTranslation().x, GetTransform().GetTranslation().y, -mDrawDepth);
					Vector3 scale = Vector3(mSize.x, mSize.y, 1.0f);

					drawMatrix.SetTranslation(position);
					drawMatrix.SetScale(scale);
				}
				else
				{
					//Use a world view projection matrix
					drawMatrix = GetTransform() * camera.GetTransform().GetInverse() * renderer.GetProjectionMatrix();
				}

				//Set world view projection matrix
				mShaderParams.mWorldViewProjMtx.SetValue(drawMatrix);

				//Set colour
				mShaderParams.mDiffuseColour.SetValue(mColour);
			
				//Set texture
				mShaderParams.mSpriteSheet.SetValue(*mSpriteSheet);

				//Set grid size
				mShaderParams.mSpriteSheetGridSize.SetValue(Vector2((float)mSpriteSheetGridSizeX, (float)mSpriteSheetGridSizeY));

				//Set current anim frame
				mShaderParams.mSpriteAnimFrame.SetValue((float)mCurrentFrame);

				//Bind shaders
				mVertexShader.Get()->Bind();
				mPixelShader.Get()->Bind();

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

		void SpriteAnimation::SetAnimationTrack(const AnimationTrackInt& animationTrack)
		{
			mAnimationTrack = &animationTrack;
		}

		void SpriteAnimation::ApplyFrame(float frame)
		{
			if(mAnimationTrack)
			{
				int value = mAnimationTrack->GetValue(frame);
				mSprite.SetFrame(value);
			}
		}
	}
}