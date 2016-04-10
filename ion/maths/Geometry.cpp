///////////////////////////////////////////////////
// File:		Geometry.cpp
// Date:		10th April 2016
// Authors:		Matt Phillips
// Description:	Basic geometry tests
///////////////////////////////////////////////////

#include "Geometry.h"

namespace ion
{
	namespace maths
	{
		bool PointInsideBox(const Vector2& point, const Vector2& boxMin, const Vector2& boxMax)
		{
			if(point.x < boxMin.x) return false;
			if(point.x > boxMax.x) return false;
			if(point.y < boxMin.y) return false;
			if(point.y > boxMax.y) return false;
			return true;
		}
	}
}