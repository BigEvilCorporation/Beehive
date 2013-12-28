///////////////////////////////////////////////////
// File:		ViewportOgre.h
// Date:		30th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d viewport implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreViewport.h>

#include "core/Colour.h"
#include "renderer/Viewport.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class ViewportOgre : public Viewport
			{
			public:
				ViewportOgre(Renderer& renderer, Camera& camera, int zOrder = 0);
				~ViewportOgre();

				virtual void SetBackgroundColour(const Colour& colour);

				Ogre::Viewport* mOgreViewport;
			};
		}

		typedef ogre::ViewportOgre ViewportImpl;
	}
}