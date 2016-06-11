///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			enum RenderType { eRender2D, eRender3D };

			Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteGridSizeX, int spriteGridSizeY, const std::string& sprite, io::ResourceManager& resourceManager);
			Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteGridSizeX, int spriteGridSizeY, const io::ResourceHandle<Texture>& sprite, io::ResourceManager& resourceManager);
			virtual ~Sprite();

			void SetFrame(int frame);
			int GetFrame() const;

			void SetColour(const Colour& colour);

			void Render(Renderer& renderer, Camera& camera);

		protected:
			io::ResourceHandle<Shader> m_vertexShader;
			io::ResourceHandle<Shader> m_pixelShader;
			io::ResourceHandle<Texture> m_spriteSheet;

			struct ShaderParams
			{
				Shader::ParamHndl<Matrix4> m_worldViewProjMtx;
				Shader::ParamHndl<Colour> m_diffuseColour;
				Shader::ParamHndl<Texture> m_spriteSheet;
				Shader::ParamHndl<Vector2> m_spriteSheetGridSize;
				Shader::ParamHndl<float> m_spriteAnimFrame;
			};

			ShaderParams m_shaderParams;
			RenderType m_renderType;
			Vector2 m_size;
			float m_drawDepth;
			int m_spriteSheetGridSizeX;
			int m_spriteSheetGridSizeY;
			int m_currentFrame;
			Colour m_colour;

			Quad* m_quadPrimitive;
		};

		class SpriteAnimation : public Animation
		{
		public:
			SpriteAnimation(Sprite& sprite);

			void SetAnimationTrack(const AnimationTrackInt& animationTrack);

		protected:
			virtual void ApplyFrame(float frame);

		private:
			Sprite& m_sprite;
			const AnimationTrackInt* m_animationTrack;
		};
	}
}