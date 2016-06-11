///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			m_renderType = renderType;
			m_size = size;
			m_drawDepth = drawDepth;
			m_spriteSheetGridSizeX = spriteGridSizeX;
			m_spriteSheetGridSizeY = spriteGridSizeY;
			m_currentFrame = 0;
			m_quadPrimitive = new Quad(Quad::xy, Vector2(1.0f, 1.0f));

			m_vertexShader = resourceManager.GetResource<Shader>("sprite_v.ion.shader");
			m_pixelShader = resourceManager.GetResource<Shader>("sprite_p.ion.shader");
			m_spriteSheet = resourceManager.GetResource<Texture>(sprite);
		}

		Sprite::Sprite(RenderType renderType, const Vector2& size, float drawDepth, int spriteGridSizeX, int spriteGridSizeY, const io::ResourceHandle<Texture>& sprite, io::ResourceManager& resourceManager)
		{
			m_renderType = renderType;
			m_size = size;
			m_drawDepth = drawDepth;
			m_spriteSheetGridSizeX = spriteGridSizeX;
			m_spriteSheetGridSizeY = spriteGridSizeY;
			m_currentFrame = 0;
			m_quadPrimitive = new Quad(Quad::xy, Vector2(1.0f, 1.0f));

			m_vertexShader = resourceManager.GetResource<Shader>("sprite_v.ion.shader");
			m_pixelShader = resourceManager.GetResource<Shader>("sprite_p.ion.shader");
			m_spriteSheet = sprite;
		}

		Sprite::~Sprite()
		{
			delete m_quadPrimitive;
		}

		void Sprite::SetFrame(int frame)
		{
			char text[128] = {0};
			sprintf(text, "%i\n", frame);
			ion::debug::Log(text);
			m_currentFrame = frame;
		}

		int Sprite::GetFrame() const
		{
			return m_currentFrame;
		}

		void Sprite::SetColour(const Colour& colour)
		{
			m_colour = colour;
		}

		void Sprite::Render(Renderer& renderer, Camera& camera)
		{
			if(m_vertexShader && m_pixelShader && m_spriteSheet)
			{
				if(!m_shaderParams.m_worldViewProjMtx.IsValid())
				{
					m_shaderParams.m_worldViewProjMtx = m_vertexShader.Get()->CreateParamHndl<Matrix4>("gWorldViewProjectionMatrix");
					m_shaderParams.m_diffuseColour = m_vertexShader.Get()->CreateParamHndl<Colour>("gDiffuseColour");
					m_shaderParams.m_spriteSheetGridSize = m_vertexShader.Get()->CreateParamHndl<Vector2>("gSpriteSheetGridSize");
					m_shaderParams.m_spriteAnimFrame = m_vertexShader.Get()->CreateParamHndl<float>("gCurrentFrame");
					m_shaderParams.m_spriteSheet = m_pixelShader.Get()->CreateParamHndl<Texture>("gSpriteSheet");
				}

				Matrix4 drawMatrix;

				if(m_renderType == eRender2D)
				{
					Vector3 position = Vector3(GetTransform().GetTranslation().x, GetTransform().GetTranslation().y, -m_drawDepth);
					Vector3 scale = Vector3(m_size.x, m_size.y, 1.0f);

					drawMatrix.SetTranslation(position);
					drawMatrix.SetScale(scale);
				}
				else
				{
					//Use a world view projection matrix
					drawMatrix = GetTransform() * camera.GetTransform().GetInverse() * renderer.GetProjectionMatrix();
				}

				//Set world view projection matrix
				m_shaderParams.m_worldViewProjMtx.SetValue(drawMatrix);

				//Set colour
				m_shaderParams.m_diffuseColour.SetValue(m_colour);
			
				//Set texture
				m_shaderParams.m_spriteSheet.SetValue(*m_spriteSheet);

				//Set grid size
				m_shaderParams.m_spriteSheetGridSize.SetValue(Vector2((float)m_spriteSheetGridSizeX, (float)m_spriteSheetGridSizeY));

				//Set current anim frame
				m_shaderParams.m_spriteAnimFrame.SetValue((float)m_currentFrame);

				//Bind shaders
				m_vertexShader.Get()->Bind();
				m_pixelShader.Get()->Bind();

				//Render VBO
				renderer.DrawVertexBuffer(m_quadPrimitive->GetVertexBuffer(), m_quadPrimitive->GetIndexBuffer());

				//Unbind
				m_pixelShader.Get()->Unbind();
				m_vertexShader.Get()->Unbind();
			}
		}

		SpriteAnimation::SpriteAnimation(Sprite& sprite)
			: m_sprite(sprite)
		{
			m_animationTrack = NULL;
		}

		void SpriteAnimation::SetAnimationTrack(const AnimationTrackInt& animationTrack)
		{
			m_animationTrack = &animationTrack;
		}

		void SpriteAnimation::ApplyFrame(float frame)
		{
			if(m_animationTrack)
			{
				int value = m_animationTrack->GetValue(frame);
				m_sprite.SetFrame(value);
			}
		}
	}
}