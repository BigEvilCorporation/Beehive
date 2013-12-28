/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Maths.h

	Maths constants and utility functions
*/

#pragma once

#include <math.h>
#include <stdlib.h>

namespace ion
{
	namespace maths
	{
		const float PI = 3.14159264f;
		const float TWO_PI = PI * 2.0f;
		const float HALF_PI = PI / 2.0f;
		const float PI_DIV_180 = PI / 180.0f;
		const float ONE80_DIV_PI = 180.0f / PI;

		const float DEGREES_TO_RADIANS = PI / 180.0f;
		const float RADIANS_TO_DEGREES = 180.0f / PI;

		const float FLOAT_MIN = 1.175494351e-38f;
		const float FLOAT_MAX = 3.402823466e+38f;
		const float FLOAT_EPSILON = 1.19209290e-7f;

		int Round(double x);
		float RoundToNearest(float value, int nearest);
		int NextPowerOfTwo(int x);

		float Floor(float value);
		float Ceil(float value);
		float Clamp(float value, float min, float max);
		float Abs(float value);
		float Fmod(float value, float divisor);
		float Lerp(float valueA, float valueB, float time);
		float UnLerp(float valueFrom, float valueTo, float valueCurrent);

		int RandInt();
		float RandFloat();

		float DegreesToRadians(float degrees);
		float RadiansToDegrees(float radians);
	}
}
