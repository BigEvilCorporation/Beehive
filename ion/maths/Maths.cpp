/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Maths.cpp

	Maths constants and utility functions
*/

#include "maths/Maths.h"

namespace ion
{
	namespace maths
	{
		int Round(double x)
		{
			return (int)(x + 0.5);
		}

		float RoundToNearest(float value, int nearest)
		{
			return floor(value / nearest) * nearest;
		}

		int NextPowerOfTwo(int x)
		{
			double logbase2 = log((double)x) / log((double)2);
			return Round(pow((double)2, (double)ceil(logbase2)));
		}

		float Floor(float value)
		{
			return floor(value);
		}

		float Ceil(float value)
		{
			return ceil(value);
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

		float Abs(float value)
		{
			return abs(value);
		}

		float Fmod(float value, float divisor)
		{
			return fmod(value, divisor);
		}

		float Lerp(float valueA, float valueB, float time)
		{
			return valueA + ((valueB - valueA) * time);
		}

		float UnLerp(float valueFrom, float valueTo, float valueCurrent)
		{
			float time = 0.0f;

			if(valueTo - valueFrom != 0.0f)
			{
				time = (valueFrom - valueCurrent) / (valueFrom - valueTo);
			}

			return time;
		}

		float DegreesToRadians(float degrees)
		{
			return degrees * DEGREES_TO_RADIANS;
		}

		float RadiansToDegrees(float radians)
		{
			return radians * RADIANS_TO_DEGREES;
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
