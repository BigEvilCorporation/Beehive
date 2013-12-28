///////////////////////////////////////////////////
// File:		Viewport.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre viewport wrapper
///////////////////////////////////////////////////

#include "core/Colour.h"
#include "renderer/Viewport.h"
#include "renderer/Renderer.h"
#include "renderer/Camera.h"
#include "renderer/Factory.h"

namespace ion
{
	namespace renderer
	{
		Viewport* Viewport::Create(Renderer& renderer, Camera& camera, int zOrder)
		{
			return new ViewportImpl(renderer, camera, zOrder);
		}

		void Viewport::Release(Viewport* viewport)
		{
			delete viewport;
		}

		Viewport::Viewport(Renderer& renderer, Camera& camera, int zOrder)
		{
		}

		Viewport::~Viewport()
		{

		}

		int Viewport::GetWidth() const
		{
			return mWidth;
		}

		int Viewport::GetHeight() const
		{
			return mHeight;
		}

		void Viewport::SetBackgroundColour(const Colour& colour)
		{
			mBackgroundColour = colour;
		}
	}
}