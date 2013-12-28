///////////////////////////////////////////////////
// File:		Serialise.h
// Date:		22nd November 2012
// Authors:		Matt Phillips
// Description:Base class for serialisable types
///////////////////////////////////////////////////

#pragma once

namespace ion
{
	namespace serialise
	{
		//Forward declaration
		class Archive;

		class Serialisable
		{
		public:
			virtual void Serialise(Archive& archive) = 0;
		};
	}
}