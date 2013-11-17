///////////////////////////////////////////////////
// File:		Viewport.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre viewport base class
///////////////////////////////////////////////////

#pragma once

#include "../Core/Colour.h"

#if defined ION_OGRE
#include <Ogre/OgreViewport.h>
#endif

namespace ion
{
	//Forward declaration
	class Colour;

	namespace renderer
	{
		//Forward declaration
		class Camera;
		class Renderer;

		class Viewport
		{
		public:
			Viewport(Renderer& renderer, Camera& camera, int zOrder = 0);
			~Viewport();

			int GetWidth() const;
			int GetHeight() const;

			void SetBackgroundColour(const Colour& colour);

			#if defined ION_OGRE
			Ogre::Viewport* GetOgreViewportInterface();
			#endif

		private:
			int mWidth;
			int mHeight;
			Colour mBackgroundColour;

			#if defined ION_OGRE
			Ogre::Viewport* mOgreViewportIFace;
			#endif
		};
	}
}