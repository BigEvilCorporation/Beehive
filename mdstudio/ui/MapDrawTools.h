#pragma once

#include <ion/maths/Vector.h>
#include <ion/maths/Matrix.h>

namespace drawtools
{
	void SanitiseBoxOrder(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, ion::Vector2i& topLeft, ion::Vector2i& bottomRight);
	ion::Matrix4 CalcBoxDrawMatrix(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, const ion::Vector2i& mapSizePx, float z);
}