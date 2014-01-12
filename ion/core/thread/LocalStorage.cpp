///////////////////////////////////////////////////
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
			mTLSIndex = TlsAlloc();
			debug::Assert(mTLSIndex != TLS_OUT_OF_INDEXES, "Could not create TLS - out of indices");
			#endif
		}

		LocalStorage::~LocalStorage()
		{
			#if defined ION_PLATFORM_WINDOWS
			TlsFree(mTLSIndex);
			#endif
		}

		void LocalStorage::SetData(StorageData& data)
		{
			#if defined ION_PLATFORM_WINDOWS
			TlsSetValue(mTLSIndex, &data);
			#endif
		}

		StorageData* LocalStorage::GetData() const
		{
			#if defined ION_PLATFORM_WINDOWS
			return (StorageData*)TlsGetValue(mTLSIndex);
			#endif
		}
	}
}