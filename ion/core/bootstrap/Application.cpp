///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Application.cpp
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Application framework base class
//				and entry point
///////////////////////////////////////////////////

#include "Application.h"

namespace ion
{
	namespace framework
	{
		Application::Application(const char* name)
		{
			m_name = name;
		}

		Application::~Application()
		{
		}

		bool Application::Initialise()
		{
			return true;
		}

		void Application::Shutdown()
		{
		}

		bool Application::Update(float deltaTime)
		{
			return true;
		}

		void Application::Render()
		{
		}
	}
}