/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Vertex.h

	Vertex definition
*/

#pragma once

#include "../core/maths/Vector.h"

namespace ion
{
	namespace renderer
	{
		class Vertex
		{
		public:
			Vertex()
			{
				x = y = z = 0.0f;
			}

			Vertex(float X, float Y, float Z)
			{
				x = X;
				y = Y;
				z = Z;
			}

			Vertex(const Vector3 &Vector)
			{
				x = Vector.x;
				y = Vector.y;
				z = Vector.z;
			}

			float x, y, z;
		};
	}
}
