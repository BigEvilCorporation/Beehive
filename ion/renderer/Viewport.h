///////////////////////////////////////////////////
// File:		Viewport.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre viewport base class
///////////////////////////////////////////////////

#pragma once

#include "core/Colour.h"

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Camera;
		class Renderer;

		class Viewport
		{
		public:
			static Viewport* Create(Renderer& renderer, Camera& camera, int zOrder = 0);
			static void Release(Viewport* viewport);

			int GetWidth() const;
			int GetHeight() const;

			virtual void SetBackgroundColour(const Colour& colour);

		protected:
			Viewport(Renderer& renderer, Camera& camera, int zOrder = 0);
			virtual ~Viewport();

			int mWidth;
			int mHeight;
			Colour mBackgroundColour;
		};
	}
}