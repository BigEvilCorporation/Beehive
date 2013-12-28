///////////////////////////////////////////////////
// File:		ViewportOgre.h
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d viewport implementation
///////////////////////////////////////////////////

#include "ViewportOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			ViewportOgre::ViewportOgre(Renderer& renderer, Camera& camera, int zOrder)
				: Viewport(renderer, camera, zOrder)
			{
				//Create viewport interface
				mOgreViewport = renderer.GetOgreRenderWindow()->addViewport(((CameraOgre&)camera).mOgreCamera, zOrder);

				//Correct camera's aspect ratio
				((CameraOgre&)camera).mOgreCamera->setAspectRatio(float(mOgreViewport->getActualWidth()) / float(mOgreViewport->getActualHeight()));
 
				//Set viewport's camera
				mOgreViewport->setCamera(((CameraOgre&)camera).mOgreCamera);

				//Clear every frame
				mOgreViewport->setClearEveryFrame(true);

				//Enable overlays
				mOgreViewport->setOverlaysEnabled(true);

				//Render with Renderer::Update
				mOgreViewport->setAutoUpdated(true);

				//Get width/height
				mWidth = mOgreViewport->getActualWidth();
				mHeight = mOgreViewport->getActualHeight();
			}

			ViewportOgre::~ViewportOgre()
			{
			}

			void ViewportOgre::SetBackgroundColour(const Colour& colour)
			{
				Viewport::SetBackgroundColour(colour);
				mOgreViewport->setBackgroundColour(Ogre::ColourValue(colour.r, colour.g, colour.b));
			}
		}
	}
}