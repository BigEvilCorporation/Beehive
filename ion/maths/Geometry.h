///////////////////////////////////////////////////
// File:		Geometry.h
// Date:		10th April 2016
// Authors:		Matt Phillips
// Description:	Basic geometry tests
///////////////////////////////////////////////////

#pragma once

#include "Vector.h"

namespace ion
{
	namespace maths
	{
		bool PointInsideBox(const Vector2& point, const Vector2& boxMin, const Vector2& boxMax);
	}
}