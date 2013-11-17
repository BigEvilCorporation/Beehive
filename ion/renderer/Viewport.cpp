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
		Viewport::Viewport(Renderer& renderer, Camera& camera, int zOrder)
		{
			#if defined ION_OGRE

			//Create viewport interface
			mOgreViewportIFace = renderer.GetOgreRenderWindow()->addViewport(camera.GetOgreCameraIFace(), zOrder);

			//Correct camera's aspect ratio
			camera.GetOgreCameraIFace()->setAspectRatio(float(mOgreViewportIFace->getActualWidth()) / float(mOgreViewportIFace->getActualHeight()));
 
			//Set viewport's camera
			mOgreViewportIFace->setCamera(camera.GetOgreCameraIFace());

			//Clear every frame
			mOgreViewportIFace->setClearEveryFrame(true);

			//Enable overlays
			mOgreViewportIFace->setOverlaysEnabled(true);

			//Render with Renderer::Update
			mOgreViewportIFace->setAutoUpdated(true);

			//Get width/height
			mWidth = mOgreViewportIFace->getActualWidth();
			mHeight = mOgreViewportIFace->getActualHeight();

			#endif
		}

		Viewport::~Viewport()
		{
			//Ogre::Viewport destroyed with the renderer
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

			#if defined ION_OGRE
			if(mOgreViewportIFace)
			{
				mOgreViewportIFace->setBackgroundColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}
			#endif
		}

		#if defined ION_OGRE
		Ogre::Viewport* Viewport::GetOgreViewportInterface()
		{
			return mOgreViewportIFace;
		}
		#endif
	}
}