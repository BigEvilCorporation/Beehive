///////////////////////////////////////////////////
// File:		Viewport.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre viewport wrapper
///////////////////////////////////////////////////

#include "Viewport.h"
#include "Renderer.h"
#include "Camera.h"
#include "../core/Colour.h"

namespace ion
{
	namespace renderer
	{
		Viewport::Viewport(Renderer& renderer, Camera& camera)
		{
			//Create viewport interface
			mOgreViewportIFace = renderer.GetOgreRenderWindow()->addViewport(camera.GetOgreCameraIFace());

			//Correct camera's aspect ratio
			camera.GetOgreCameraIFace()->setAspectRatio(float(mOgreViewportIFace->getActualWidth()) / float(mOgreViewportIFace->getActualHeight()));
 
			//Set viewport's camera
			mOgreViewportIFace->setCamera(camera.GetOgreCameraIFace());

			//Render with Renderer::Update
			mOgreViewportIFace->setAutoUpdated(true);
		}

		Viewport::~Viewport()
		{
			//Ogre::Viewport destroyed with the renderer
		}

		Ogre::Viewport* Viewport::GetOgreViewportInterface()
		{
			return mOgreViewportIFace;
		}

		void Viewport::SetBackgroundColour(const Colour& colour)
		{
			if(mOgreViewportIFace)
			{
				mOgreViewportIFace->setBackgroundColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}
		}
	}
}