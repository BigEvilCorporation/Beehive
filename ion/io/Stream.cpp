///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		Stream.cpp
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:	Read/write stream for serialisation
///////////////////////////////////////////////////

#include "Stream.h"
#include "core/memory/Memory.h"
#include "core/debug/Debug.h"

#include <algorithm>

namespace ion
{
	namespace io
	{
		MemoryStream::MemoryStream()
		{
			m_position = 0;
		}

		u64 MemoryStream::GetSize() const
		{
			return m_bytes.size();
		}

		u64 MemoryStream::GetPosition() const
		{
			return m_position;
		}

		u64 MemoryStream::Read(void* data, u64 size)
		{
			size = std::min(size, GetSize() - m_position);
			memory::MemCopy(data, &m_bytes[m_position], size);
			m_position += size;
			return size;
		}

		u64 MemoryStream::Write(const void* data, u64 size)
		{
			u64 requiredSize = m_position + size + 1;
			if(m_position + size > GetSize())
				m_bytes.resize(requiredSize);
			memory::MemCopy(&m_bytes[m_position], data, size);
			m_position += size;
			return size;
		}

		u64 MemoryStream::Seek(u64 position)
		{
			debug::Assert(position < GetSize(), "MemoryStream::Seek() - eOut of range");
			m_position = position;
			return m_position;
		}
	}
}