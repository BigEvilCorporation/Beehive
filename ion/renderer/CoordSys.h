///////////////////////////////////////////////////
// File:		CoordSys.h
// Date:		6th January 2014
// Authors:		Matt Phillips
// Description:	Coordinate system constants and utils
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"

namespace ion
{
	namespace render
	{
		namespace coordsys
		{
			static const Vector3 Up = Vector3(0.0f, 1.0f, 0.0f);
			static const Vector3 Right = Vector3(1.0f, 0.0f, 0.0f);
			static const Vector3 Forward = Vector3(0.0f, 0.0f, 1.0f);
		}
	}
}