///////////////////////////////////////////////////
// File:		Sprite.h
// Date:		14th December 2013
// Authors:		Matt Phillips
// Description:	Sprite class
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "io/ResourceHandle.h"
#include "io/ResourceManager.h"
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
			enum RenderType { Render2D, Render3D };

			Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteSheetGridSizeX, int spriteSheetGridSizeY, const std::string& spriteSheet, io::ResourceManager& resourceManager);
			virtual ~Sprite();

			void SetFrame(int frame);
			int GetFrame() const;

			void SetColour(const Colour& colour);

			void Render(Renderer& renderer, Camera& camera);

		protected:
			io::ResourceHandle<Shader> mVertexShader;
			io::ResourceHandle<Shader> mPixelShader;
			io::ResourceHandle<Texture> mSpriteSheet;

			struct ShaderParams
			{
				Shader::ParamHndl<Matrix4> mWorldViewProjMtx;
				Shader::ParamHndl<Colour> mDiffuseColour;
				Shader::ParamHndl<Texture> mSpriteSheet;
				Shader::ParamHndl<Vector2> mSpriteSheetGridSize;
				Shader::ParamHndl<float> mSpriteAnimFrame;
			};

			ShaderParams mShaderParams;
			RenderType mRenderType;
			Vector2 mSize;
			float mDrawDepth;
			int mSpriteSheetGridSizeX;
			int mSpriteSheetGridSizeY;
			int mCurrentFrame;
			Colour mColour;

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