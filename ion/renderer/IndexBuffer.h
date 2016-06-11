///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		IndexBuffer.h
// Date:		4rd December 2013
// Authors:		Matt Phillips
// Description:	Index buffer class
///////////////////////////////////////////////////

#pragma once

#include "core/Types.h"

#include <vector>

namespace ion
{
	namespace render
	{
		class IndexBuffer
		{
		public:
			void Add(u32 index) { m_indices.push_back(index); }
			void Add(u32 face0, u32 face1, u32 face2)
			{
				m_indices.push_back(face0);
				m_indices.push_back(face1);
				m_indices.push_back(face2);
			}

			u32 Get(int index) const { return m_indices[index]; }

			int GetSize() const { return m_indices.size(); }
			const u32* GetAddress() const { return &m_indices[0]; }

		private:
			std::vector<u32> m_indices;
		};
	}
}