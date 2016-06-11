///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		LocalStorage.h
// Date:		10th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"
#include "core/thread/CriticalSection.h"
#include <vector>

#if defined ION_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace ion
{
	namespace thread
	{
		class StorageData
		{
		public:
			virtual ~StorageData() {}
		};

		class LocalStorage
		{
		public:
			LocalStorage();
			~LocalStorage();

			void SetData(StorageData& data);
			StorageData* GetData() const;

		private:
			#if defined ION_PLATFORM_WINDOWS
			DWORD m_TLSIndex;
			#endif
		};
	}
}