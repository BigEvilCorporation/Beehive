///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		LocalStorage.cpp
// Date:		10th January 2014
// Authors:		Matt Phillips
// Description:	Threading and synchronisation
///////////////////////////////////////////////////

#include "core/debug/Debug.h"
#include "core/thread/LocalStorage.h"

namespace ion
{
	namespace thread
	{
		LocalStorage::LocalStorage()
		{
			#if defined ION_PLATFORM_WINDOWS
			m_TLSIndex = TlsAlloc();
			debug::Assert(m_TLSIndex != TLS_OUT_OF_INDEXES, "Could not create TLS - out of indices");
			#endif
		}

		LocalStorage::~LocalStorage()
		{
			#if defined ION_PLATFORM_WINDOWS
			TlsFree(m_TLSIndex);
			#endif
		}

		void LocalStorage::SetData(StorageData& data)
		{
			#if defined ION_PLATFORM_WINDOWS
			TlsSetValue(m_TLSIndex, &data);
			#endif
		}

		StorageData* LocalStorage::GetData() const
		{
			#if defined ION_PLATFORM_WINDOWS
			return (StorageData*)TlsGetValue(m_TLSIndex);
			#endif
		}
	}
}