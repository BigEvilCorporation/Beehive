#include "maths/Vector.h"

namespace ion
{
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

	Vector3::Vector3(const float* float3)
	{
		x = float3[0];
		y = float3[1];
		z = float3[2];
	}

	Vector3::Vector3(const Vector3& vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

	Vector3::~Vector3() {}

	void Vector3::Zero()
	{
		x = y = z = 0.0f;
	}

	float Vector3::operator [](int index) const
	{
		switch(index)
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

	Vector3 Vector3::operator *(float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 Vector3::operator *(const Vector3& vector) const
	{
		return Vector3(x * vector.x, y * vector.y, z * vector.z);
	}

	Vector3 Vector3::operator /(const Vector3& vector) const
	{
		return Vector3(x / vector.x, y / vector.y, z / vector.z);
	}

	Vector3 Vector3::operator -(float scalar) const
	{
		return Vector3(x - scalar, y - scalar, z - scalar);
	}

	Vector3 Vector3::operator +(float scalar) const
	{
		return Vector3(x + scalar, y + scalar, z + scalar);
	}

	Vector3 Vector3::operator /(float scalar) const
	{
		float inv = 1.0f / scalar;
		return Vector3(x * inv, y * inv, z * inv);
	}

	Vector3 Vector3::operator -(const Vector3& vector) const
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	Vector3 Vector3::operator +(const Vector3& vector) const
	{
		return Vector3(vector.x + x, vector.y + y, vector.z + z);
	}

	void Vector3::operator +=(const Vector3& vector)
	{
		*this = *this + vector;
	}

	void Vector3::operator -=(const Vector3& vector)
	{
		*this = *this - vector;
	}

	void Vector3::operator *=(const Vector3& vector)
	{
		x = x * vector.x;
		y = y * vector.y;
		z = z * vector.z;
	}

	void Vector3::operator *=(float scalar)
	{
		*this = *this * scalar;
	}

	bool Vector3::operator ==(const Vector3& vector) const
	{
		return (x == vector.x && y == vector.y && z == vector.z);
	}

	Vector3 Vector3::Cross(const Vector3& vector) const
	{
		return Vector3((y * vector.z - vector.y * z), (z * vector.x - vector.z * x), (x * vector.y - vector.x * y));
	}

	float Vector3::Dot(const Vector3& vector) const
	{
		return (x * vector.x + y * vector.y + z * vector.z);
	}

	Vector3 Vector3::Normalise() const
	{
		float length = GetLength();

		Vector3 result;

		if(length != 0.0f)
		{
			if(x != 0.0f) result.x = x / length;
			if(y != 0.0f) result.y = y / length;
			if(z != 0.0f) result.z = z / length;
		}

		return result;
	}

	float Vector3::GetLength() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	float Vector3::GetDistance(const Vector3& vector) const
	{
		float dx, dy, dz;
		dx = x - vector.x;
		dy = y - vector.y;
		dz = z - vector.z;
		return sqrt(dx*dx + dy*dy + dz*dz);
	}

	Vector3 Vector3::Lerp(const Vector3& vector, float time) const
	{
		Vector3 result;

		if (time <= 0.0f)
		{
			result = (*this);
		}
		else if(time >= 1.0f)
		{
			result = vector;
		}
		else
		{
			result.x = x + time * (vector.x - x);
			result.y = y + time * (vector.y - y);
			result.z = z + time * (vector.z - z);
		}

		return result;
	}

	void Vector3::Serialise(io::Archive& archive)
	{
		archive.Serialise(x);
		archive.Serialise(y);
		archive.Serialise(z);
	}
}
