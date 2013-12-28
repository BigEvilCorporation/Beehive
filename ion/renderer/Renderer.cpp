///////////////////////////////////////////////////
// File:		Renderer.cpp
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Ogre3D renderer
///////////////////////////////////////////////////

///////////////////////////////////////////////////
// File:		Renderer.cpp
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Renderer base class
///////////////////////////////////////////////////

#include "renderer/Renderer.h"

namespace ion
{
	namespace render
	{
		Renderer::Renderer(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
		{
			mWindowWidth = windowWidth;
			mWindowHeight = windowHeight;
		}

		Renderer::~Renderer()
		{

		}
	}
}
