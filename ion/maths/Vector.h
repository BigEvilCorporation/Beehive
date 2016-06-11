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
	template<typename T> class TVector2
	{
	public:
		T x, y;

		TVector2();
		TVector2(T X, T Y);
		TVector2(const TVector2<T>& vector);
		T operator [](int index) const;

		TVector2<T> operator *(T scalar) const;
		TVector2<T> operator /(T scalar) const;
		TVector2<T> operator *(const TVector2<T> &vector) const;
		TVector2<T> operator /(const TVector2<T> &vector) const;
		TVector2<T> operator +(const TVector2<T> &vector) const;
		TVector2<T> operator -(const TVector2<T> &vector) const;
		void operator +=(const TVector2<T> &vector);
		void operator -=(const TVector2<T> &vector);

		T GetLength() const;
		T Dot(const TVector2<T>& vector) const;

		//Interpolation with another vector, with a given time
		TVector2<T> Lerp(const TVector2<T> &vector, float time) const;

		//Serialisation
		void Serialise(io::Archive& archive);
	};

	typedef TVector2<float> Vector2;
	typedef TVector2<int> Vector2i;

	class Vector3
	{
	public:

		float x, y, z;

		Vector3();
		Vector3(float X, float Y, float Z);
		Vector3(const float* float3);
		Vector3(const Vector3& vector);
		~Vector3();

		void Zero();

		float operator [](int index) const;

		Vector3 operator *(float scalar) const;
		Vector3 operator *(const Vector3 &vector) const;
		Vector3 operator /(const Vector3 &vector) const;
		Vector3 operator -(const Vector3 &vector) const;
		Vector3 operator +(const Vector3 &vector) const;
		Vector3 operator -(float scalar) const;
		Vector3 operator +(float scalar) const;
		Vector3 operator /(float scalar) const;

		void operator +=(const Vector3 &vector);
		void operator -=(const Vector3 &vector);
		void operator *=(const Vector3 &vector);
		void operator *=(float scalar);
		bool operator ==(const Vector3 &vector) const;

		Vector3 Cross(const Vector3 &vector) const;
		float Dot(const Vector3& vector) const;
		Vector3 Normalise() const;

		float GetLength() const;
		float GetDistance(const Vector3 &vector) const;

		//Interpolation with another vector, with a given time
		Vector3 Lerp(const Vector3 &vector, float time) const;

		//Serialisation
		void Serialise(io::Archive& archive);
	};
}

////////////////////////////////////////////////////////////////
// Template definitions
////////////////////////////////////////////////////////////////
namespace ion
{
	template <typename T> TVector2<T>::TVector2()
	{
		x = 0;
		y = 0;
	}

	template <typename T> TVector2<T>::TVector2(T X, T Y)
	{
		x = X;
		y = Y;
	}

	template <typename T> TVector2<T>::TVector2(const TVector2<T>& vector)
	{
		x = vector.x;
		y = vector.y;
	}

	template <typename T> T TVector2<T>::operator [](int index) const
	{
		if(index == 0)
			return x;
		else if(index == 1)
			return y;
		else
			return 0.0f;
	}

	template <typename T> TVector2<T> TVector2<T>::operator *(T Scalar) const
	{
		return TVector2<T>(x * Scalar, y * Scalar);
	}

	template <typename T> TVector2<T> TVector2<T>::operator /(T Scalar) const
	{
		return TVector2<T>(x / Scalar, y / Scalar);
	}

	template <typename T> TVector2<T> TVector2<T>::operator *(const TVector2<T> &Vector) const
	{
		return TVector2<T>(x * Vector.x, y * Vector.y);
	}

	template <typename T> TVector2<T> TVector2<T>::operator /(const TVector2<T> &Vector) const
	{
		return TVector2<T>(x / Vector.x, y / Vector.y);
	}

	template <typename T> TVector2<T> TVector2<T>::operator +(const TVector2<T> &Vector) const
	{
		return TVector2<T>(x + Vector.x, y + Vector.y);
	}

	template <typename T> TVector2<T> TVector2<T>::operator -(const TVector2<T> &Vector) const
	{
		return TVector2<T>(x - Vector.x, y - Vector.y);
	}

	template <typename T> void TVector2<T>::operator +=(const TVector2<T> &Vector)
	{
		x += Vector.x;
		y += Vector.y;
	}

	template <typename T> void TVector2<T>::operator -=(const TVector2<T> &Vector)
	{
		x -= Vector.x;
		y -= Vector.y;
	}

	template <typename T> T TVector2<T>::GetLength() const
	{
		return sqrt(x*x + y*y);
	}

	template <typename T> T TVector2<T>::Dot(const TVector2<T>& vector) const
	{
		return (x * vector.x + y * vector.y);
	}

	template <typename T> TVector2<T> TVector2<T>::Lerp(const TVector2<T>& Vector, float Weight) const
	{
		TVector2<T> Result;

		if(Weight <= 0.0f)
		{
			Result = (*this);
		}
		else if(Weight >= 1.0f)
		{
			Result = Vector;
		}
		else
		{
			Result.x = x + Weight * (Vector.x - x);
			Result.y = y + Weight * (Vector.y - y);
		}

		return Result;
	}

	template <typename T> void TVector2<T>::Serialise(io::Archive& archive)
	{
		archive.Serialise(x);
		archive.Serialise(y);
	}
}