///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		CriticalSection.h
// Date:		8th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

#if defined ION_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace ion
{
	namespace thread
	{
		class CriticalSection
		{
		public:
			CriticalSection();
			~CriticalSection();

			bool TryBegin();
			void Begin();
			void End();
			
		private:
			#if defined ION_PLATFORM_WINDOWS
			CRITICAL_SECTION m_criticalSectionHndl;
			#endif
		};
	}
}