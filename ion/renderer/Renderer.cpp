///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Renderer.cpp
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Ogre3D renderer
///////////////////////////////////////////////////

///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
		Renderer::Renderer()
		{
			m_shaderManager = NULL;
		}

		Renderer::~Renderer()
		{

		}
	}
}
