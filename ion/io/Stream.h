///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Stream.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Read/write stream for serialisation
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

#include <vector>

namespace ion
{
	namespace io
	{
		class Stream
		{
		public:
			virtual u64 GetSize() const = 0;
			virtual u64 GetPosition() const = 0;
			virtual u64 Read(void* data, u64 size) = 0;
			virtual u64 Write(const void* data, u64 size) = 0;
			virtual u64 Seek(u64 position) = 0;
		};

		class MemoryStream : public Stream
		{
		public:
			MemoryStream();
			virtual u64 GetSize() const;
			virtual u64 GetPosition() const;
			virtual u64 Read(void* data, u64 size);
			virtual u64 Write(const void* data, u64 size);
			virtual u64 Seek(u64 position);

		private:
			std::vector<u8> m_bytes;
			u64 m_position;
		};
	}
}