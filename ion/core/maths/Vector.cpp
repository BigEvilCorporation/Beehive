#include "Vector.h"

namespace ion
{
	Vector2::Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2::Vector2(float X, float Y)
	{
		x = X;
		y = Y;
	}
	
	Vector2::Vector2(const Vector2& vector)
	{
		x = vector.x;
		y = vector.y;
	}

	float Vector2::operator [](int index) const
	{
		if(index == 0)
			return x;
		else if(index == 1)
			return y;
		else return 0.0f;
	}

	Vector3::Vector3()
	{ 
		Zero();
	}

	Vector3::Vector3(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	Vector3::Vector3(float *Float3)
	{
		x = Float3[0];
		y = Float3[1];
		z = Float3[2];
	}

	Vector3::Vector3(const Vector3 & Vector)
	{
		x = Vector.x;
		y = Vector.y;
		z = Vector.z;
	}

	Vector3::~Vector3() {}

	void Vector3::Zero()
	{
		x = y = z = 0.0f;
	}

	float Vector3::operator [](int Index) const
	{
		switch(Index)
		{
		case 0:
			return x;
			break;

		case 1:
			return y;
			break;

		case 2:
			return z;
			break;

		default:
			return x;
		}
	}

	/*
	float Vector3::operator *(const Vector3 &Vector) const
	{
		return (x * Vector.x) + (y * Vector.y) + (z * Vector.z);
	}
	*/

	Vector3 Vector3::operator *(float Scalar) const
	{
		return Vector3(x * Scalar, y * Scalar, z * Scalar);
	}

	Vector3 Vector3::operator *(const Vector3 &Vector) const
	{
		return Vector3(x * Vector.x, y * Vector.y, z * Vector.z);
	}

	Vector3 Vector3::operator -(float Scalar) const
	{
		return Vector3(x - Scalar, y - Scalar, z - Scalar);
	}

	Vector3 Vector3::operator +(float Scalar) const
	{
		return Vector3(x + Scalar, y + Scalar, z + Scalar);
	}

	Vector3 Vector3::operator /(float Scalar) const
	{
		float inv = 1.0f / Scalar;
		return Vector3(x * inv, y * inv, z * inv);
	}

	Vector3 Vector3::operator -(const Vector3 &Vector) const
	{
		return Vector3(x - Vector.x, y - Vector.y, z - Vector.z);
	}

	Vector3 Vector3::operator +(const Vector3 &Vector) const
	{
		return Vector3(Vector.x + x, Vector.y + y, Vector.z + z);
	}

	void Vector3::operator +=(const Vector3 &Vector)
	{
		*this = *this + Vector;
	}

	void Vector3::operator -=(const Vector3 &Vector)
	{
		*this = *this - Vector;
	}

	void Vector3::operator *=(const Vector3 &Vector)
	{
		x = x * Vector.x;
		y = y * Vector.y;
		z = z * Vector.z;
	}

	void Vector3::operator *=(float Scalar)
	{
		*this = *this * Scalar;
	}

	bool Vector3::operator ==(const Vector3 &Vector) const
	{
		return (x == Vector.x && y == Vector.y && z == Vector.z);
	}

	Vector3 Vector3::Cross(const Vector3& b) const
	{
		return Vector3((y * b.z - b.y * z), (z * b.x - b.z * x), (x * b.y - b.x * y));
	}

	float Vector3::Dot(const Vector3& Vector) const
	{
		return (x * Vector.x + y * Vector.y + z * Vector.z);
	}

	Vector3 Vector3::Normalise() const
	{
		float Length = GetLength();

		Vector3 Result;

		if(Length != 0.0f)
		{
			if(x != 0.0f) Result.x = x / Length;
			if(y != 0.0f) Result.y = y / Length;
			if(z != 0.0f) Result.z = z / Length;
		}

		return Result;
	}

	float Vector3::GetLength() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Vector3::GetDistance(const Vector3& v) const
	{
		float dx, dy, dz;
		dx = x - v.x;
		dy = y - v.y;
		dz = z - v.z;
		return sqrt(dx*dx + dy*dy + dz*dz);
	}

	Vector3 Vector3::Lerp(const Vector3& Vector, float Weight) const
	{
		Vector3 Result;

		if (Weight <= 0.0f)
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
			Result.z = z + Weight * (Vector.z - z);
		}

		return Result;
	}
}
