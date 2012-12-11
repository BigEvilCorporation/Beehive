/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Maths.cpp

	Maths constants and utility functions
*/

#include "Maths.h"

namespace ion
{
	namespace maths
	{
		int Round(double x)
		{
			return (int)(x + 0.5);
		}

		float RoundToNearest(float Value, int Nearest)
		{
			return floor(Value / Nearest) * Nearest;
		}

		int NextPowerOfTwo(int x)
		{
			double logbase2 = log((double)x) / log((double)2);
			return Round(pow((double)2, (double)ceil(logbase2)));
		}

		float Clamp(float value, float min, float max)
		{
			float clamped = value;

			if(value < min)
				clamped = min;
			else if(value > max)
				clamped = max;

			return clamped;
		}

		float DegreesToRadians(float Degrees)
		{
			return Degrees * DEGREES_TO_RADIANS;
		}

		float RadiansToDegrees(float Radians)
		{
			return Radians * RADIANS_TO_DEGREES;
		}

		int RandInt()
		{
			return rand();
		}

		float RandFloat()
		{
			return (float)rand() / (float)RAND_MAX;
		}
	}
}
