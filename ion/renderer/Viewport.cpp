///////////////////////////////////////////////////
// File:		Viewport.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Rendering viewport
///////////////////////////////////////////////////

#include "Viewport.h"
#include "core/debug/Debug.h"

namespace ion
{
	namespace render
	{
		Viewport::Viewport(int width, int height, PerspectiveMode perspectiveMode)
		{
			debug::Assert(width > 0 && height > 0, "Viewport::Viewport() - Bad width/height");
			m_width = width;
			m_height = height;
			m_perspectiveMode = perspectiveMode;
			m_clearColour = Colour(0.0f, 0.0f, 0.0f, 1.0f);
		}

		Viewport::~Viewport()
		{

		}

		int Viewport::GetWidth() const
		{
			return m_width;
		}

		int Viewport::GetHeight() const
		{
			return m_height;
		}

		void Viewport::Resize(int width, int height)
		{
			debug::Assert(width > 0 && height > 0, "Viewport::Resize() - Bad width/height");
			m_width = width;
			m_height = height;
		}

		void Viewport::SetPerspectiveMode(PerspectiveMode perspectiveMode)
		{
			m_perspectiveMode = perspectiveMode;
		}

		void Viewport::SetClearColour(const Colour& colour)
		{
			m_clearColour = colour;
		}

		Viewport::PerspectiveMode Viewport::GetPerspectiveMode() const
		{
			return m_perspectiveMode;
		}

		const Colour& Viewport::GetClearColour() const
		{
			return m_clearColour;
		}
	}
}