///////////////////////////////////////////////////
// File:		Sprite.h
// Date:		14th December 2013
// Authors:		Matt Phillips
// Description:	Sprite class
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "renderer/colour.h"
#include "renderer/Animation.h"
#include "renderer/Camera.h"
#include "renderer/Entity.h"
#include "renderer/Primitive.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"

namespace ion
{
	namespace render
	{
		class Sprite : public Entity
		{
		public:
			Sprite(float width, float height, int spriteSheetGridSizeX, int spriteSheetGridSizeY, const Texture* spriteSheet);
			virtual ~Sprite();

			void SetFrame(int frame);
			int GetFrame() const;

			void SetColour(const Colour& colour);

			void Render(Renderer& renderer, Camera& camera);

		protected:
			static Shader* sVertexShader;
			static Shader* sPixelShader;
			static Shader::ParamHndl<Matrix4> sShaderParamWorldViewProjMtx;
			static Shader::ParamHndl<Colour> sShaderParamDiffuseColour;
			static Shader::ParamHndl<Texture> sShaderParamSpriteSheet;
			static Shader::ParamHndl<Vector2> sShaderParamSpriteSheetGridSize;
			static Shader::ParamHndl<float> sShaderParamSpriteAnimFrame;
			static int sShaderRefCount;

			float mWidth;
			float mHeight;
			int mSpriteSheetGridSizeX;
			int mSpriteSheetGridSizeY;
			int mCurrentFrame;
			Colour mColour;
			const Texture* mSpriteSheet;

			Quad* mQuadPrimitive;
		};

		class SpriteAnimation : public Animation
		{
		public:
			SpriteAnimation(Sprite& sprite);

			void SetAnimationTrack(const AnimationTrackFloat& animationTrack);

		protected:
			virtual void ApplyFrame(float frame);

		private:
			Sprite& mSprite;
			const AnimationTrackFloat* mAnimationTrack;
		};
	}
}