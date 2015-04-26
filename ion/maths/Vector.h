/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Vector.h

	Vector3 class
*/

#pragma once

#include "maths/Maths.h"
#include "io/archive.h"

namespace ion
{
	class Vector2
	{
	public:
		float x, y;

		Vector2();
		Vector2(float X, float Y);
		Vector2(const Vector2& vector);
		float operator [](int index) const;

		Vector2 operator *(float Scalar) const;
		Vector2 operator *(const Vector2 &Vector) const;
		Vector2 operator +(const Vector2 &Vector) const;
		Vector2 operator -(const Vector2 &Vector) const;

		float GetLength() const;
		float Dot(const Vector2& vector) const;

		//Interpolation with another vector, with a given weight
		Vector2 Lerp(const Vector2 &Vector, float Weight) const;

		//Serialisation
		void Serialise(io::Archive& archive);
	};

	class Vector3
	{
	public:

		float x, y, z;

		Vector3();
		Vector3(float X, float Y, float Z);
		Vector3(float* Float3);
		Vector3(const Vector3& Vector);
		~Vector3();

		void Zero();

		float operator [](int Index) const;

		Vector3 operator *(float Scalar) const;
		Vector3 operator *(const Vector3 &Vector) const;
		Vector3 operator /(const Vector3 &Vector) const;
		Vector3 operator -(const Vector3 &Vector) const;
		Vector3 operator +(const Vector3 &Vector) const;
		Vector3 operator -(float Scalar) const;
		Vector3 operator +(float Scalar) const;
		Vector3 operator /(float Scalar) const;

		void operator +=(const Vector3 &Vector);
		void operator -=(const Vector3 &Vector);
		void operator *=(const Vector3 &Vector);
		void operator *=(float Scalar);
		bool operator ==(const Vector3 &Vector) const;

		Vector3 Cross(const Vector3 &b) const;
		float Dot(const Vector3& Vector) const;
		Vector3 Normalise() const;

		float GetLength() const;
		float GetDistance(const Vector3 &v) const;

		//Interpolation with another vector, with a given weight
		Vector3 Lerp(const Vector3 &Vector, float Weight) const;

		//Serialisation
		void Serialise(io::Archive& archive);
	};
}
