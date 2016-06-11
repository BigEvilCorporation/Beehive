///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Application.h
// Date:		25th July 2011
// Authors:		Matt Phillips
// Description:	Application framework base class
//				and entry point
///////////////////////////////////////////////////

#pragma once

#include <string>

namespace ion
{
	namespace framework
	{
		class Application
		{
		public:
			Application(const char* name);
			virtual ~Application();

			virtual bool Initialise();
			virtual void Shutdown();
			virtual bool Update(float deltaTime);
			virtual void Render();

		private:
			std::string m_name;
		};
	}
}