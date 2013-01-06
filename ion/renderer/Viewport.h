///////////////////////////////////////////////////
// File:		Viewport.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre viewport base class
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreViewport.h>

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

			void SetBackgroundColour(const Colour& colour);

			Ogre::Viewport* GetOgreViewportInterface();

		private:
			Ogre::Viewport* mOgreViewportIFace;
		};
	}
}