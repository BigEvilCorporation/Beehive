///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Viewport.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Rendering viewport
///////////////////////////////////////////////////

#pragma once

#include "Colour.h"

namespace ion
{
	namespace render
	{
		class Viewport
		{
		public:
			//Render perspectives
			enum PerspectiveMode { ePerspective3D, eOrtho2DNormalised, eOrtho2DAbsolute };

			Viewport(int width, int height, PerspectiveMode perspectiveMode);
			virtual ~Viewport();

			int GetWidth() const;
			int GetHeight() const;

			void Resize(int width, int height);

			void SetPerspectiveMode(PerspectiveMode perspectiveMode);
			void SetClearColour(const Colour& colour);

			PerspectiveMode GetPerspectiveMode() const;
			const Colour& GetClearColour() const;

		protected:
			int m_width;
			int m_height;
			PerspectiveMode m_perspectiveMode;
			Colour m_clearColour;
		};
	}
}