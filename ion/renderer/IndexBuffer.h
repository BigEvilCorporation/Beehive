///////////////////////////////////////////////////
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
			void Add(u16 index) { mIndices.push_back(index); }
			void Add(u16 face0, u16 face1, u16 face2)
			{
				mIndices.push_back(face0);
				mIndices.push_back(face1);
				mIndices.push_back(face2);
			}

			u16 Get(int index) const { return mIndices[index]; }

			int GetSize() const { return mIndices.size(); }
			const u16* GetAddress() const { return &mIndices[0]; }

		private:
			std::vector<u16> mIndices;
		};
	}
}