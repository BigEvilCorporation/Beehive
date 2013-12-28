/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Quaternion.cpp

	Quaternion rotation class
*/

#include "maths/Quaternion.h"

namespace ion
{
	Quaternion::Quaternion()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 1.0;
	}

	Quaternion::Quaternion(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	Quaternion::Quaternion(float W, const Vector3 &axis)
	{
		x = axis.x;
		y = axis.y;
		z = axis.z;
		w = W;
	}

	Quaternion Quaternion::operator * (const Quaternion &quat) const
	{
		float X = w * quat.x + quat.w * x + y * quat.z - z * quat.y;
		float Y = w * quat.y + quat.w * y + z * quat.x - x * quat.z;
		float Z = w * quat.z + quat.w * z + x * quat.y - y * quat.x;
		float W = w * quat.w - (x * quat.x + y * quat.y + z * quat.z);

		return Quaternion(X, Y, Z, W);
	}

	void Quaternion::operator *= (const Quaternion &quat)
	{
		float W = w * quat.w - (x * quat.x + y * quat.y + z * quat.z);

		float X = w * quat.x + quat.w * x + y * quat.z - z * quat.y;
		float Y = w * quat.y + quat.w * y + z * quat.x - x * quat.z;
		float Z = w * quat.z + quat.w * z + x * quat.y - y * quat.x;

		w = W;
		x = X;
		y = Y;
		z = Z;
	}

	Vector3 Quaternion::operator* (const Vector3 &vector) const
	{
		Vector3 vn = vector;
		vn.Normalise();

		Quaternion vecQuat, resQuat;
		vecQuat.x = vn.x;
		vecQuat.y = vn.y;
		vecQuat.z = vn.z;
		vecQuat.w = 0.0f;

		resQuat = vecQuat * GetConjugate();
		resQuat = *this * resQuat;

		return (Vector3(resQuat.x, resQuat.y, resQuat.z));
	}

	Quaternion Quaternion::operator +(const Quaternion& quat) const
	{
		return Quaternion(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
	}


	void Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void Quaternion::Invert()
	{
		float norm = sqrt(w * w + x * x + y * y + z * z);
		if (norm == 0.0) norm = 1.0;

		float recip = 1.0f / norm;

		x = -x * recip;
		y = -y * recip;
		z = -z * recip;
		w =  w * recip;
	}

	void Quaternion::Normalise()
	{
		float norm = sqrt(w * w + x * x + y * y + z * z);
		if (norm == 0.0)
		{
			w = 1.0;
			x = y = z = 0.0;
		}
		else
		{
			float recip = 1.0f / norm;

			x *= recip;
			y *= recip;
			z *= recip;
			w *= recip;
		}
	}

	float Quaternion::Dot(const Quaternion& quat) const
	{
		return (x * quat.x) + (y * quat.y) + (z * quat.z) + (w * quat.w);
	}

	Quaternion Quaternion::GetInverse() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	Quaternion Quaternion::GetConjugate() const
	{
		return Quaternion(-y, -z, w, -x);
	}

	Quaternion Quaternion::GetToPowerOf(float n) const
	{
		// q = w + (x,y,z)
		// q = cos(theta) + u*sin(theta)
		// q^n = cos(n*theta) + u*sin(n*theta)

		float ntheta = n * z;
		float costheta = cos(ntheta);
		float sintheta = sin(ntheta);
		return Quaternion(costheta, x * sintheta, y * sintheta, z * sintheta);
	}

	void Quaternion::FromAxis(float radians, const Vector3& axis)
	{
		w = 0;
		x = axis.x;
		y = axis.y;
		z = axis.z;
		if (x == 0.0f && y == 0.0f && z == 0.0f) x = 1.0f;
		Normalise();

		radians = radians / 2;
		w = (float)cos(radians);

		float Scale = (float)sin(radians);
		x *= Scale;
		y *= Scale;
		z *= Scale;
	}

	void Quaternion::FromLookAt(const Vector3& forward, const Vector3& position, const Vector3& target, float factor)
	{
		Vector3 direction = (target - position).Normalise();
		Vector3 right = forward.Cross(direction).Normalise();
		Vector3 up = direction.Cross(right).Normalise();

		Matrix4 lookAtMatrix;
		lookAtMatrix.SetRight(right);
		lookAtMatrix.SetUp(up);
		lookAtMatrix.SetForward(forward);

		Quaternion targetQuat;
		targetQuat.FromMatrix(lookAtMatrix);

		//Slerp current quaternion using new quaternion, by factor
		if(factor == 1.0f)
		{
			*this = targetQuat;
		}
		else if(factor > 0.0f)
		{
			Slerp((*this), targetQuat, factor);
		}

		*this = targetQuat;
	}

	Matrix4 Quaternion::ToMatrix() const
	{
		float matrix[16];

		// First row
		matrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );
		matrix[ 1] = 2.0f * (x * y + z * w);
		matrix[ 2] = 2.0f * (x * z - y * w);
		matrix[ 3] = 0.0f;

		// Second row
		matrix[ 4] = 2.0f * ( x * y - z * w );
		matrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );
		matrix[ 6] = 2.0f * (z * y + x * w );
		matrix[ 7] = 0.0f;

		// Third row
		matrix[ 8] = 2.0f * ( x * z + y * w );
		matrix[ 9] = 2.0f * ( y * z - x * w );
		matrix[10] = 1.0f - 2.0f * ( x * x + y * y );
		matrix[11] = 0.0f;

		// Fourth row
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1.0f;

		Matrix4 SMatrix(matrix);

		return SMatrix;
	}

	void Quaternion::FromEuler(const Vector3& euler)
	{
		float cosYaw = cos(euler.y / 2);
		float sinYaw = sin(euler.y / 2);
		float cosPitch = cos(euler.x / 2);
		float sinPitch = sin(euler.x / 2);
		float cosRoll = cos(euler.z / 2);
		float sinRoll = sin(euler.z / 2);
		x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}

	Vector3 Quaternion::ToEuler() const
	{
		Vector3 euler;
		float testValue = x * y + z * w;
		if(testValue > 0.499f)
		{
			euler.y = 2 * atan2(x,w);
			euler.z = maths::PI / 2;
			euler.x = 0.0f;
			return euler;
		}
		if(testValue < -0.499f)
		{
			euler.y = -2 * atan2(x,w);
			euler.z = -maths::PI / 2;
			euler.x = 0.0f;
			return euler;
		}

		float sqx = x*x;
		float sqy = y*y;
		float sqz = z*z;
		float sqw = w*w;
		float unit = sqx + sqy + sqz + sqw;

		euler.y = atan2( (float)2.0f*y*w - 2.0f*x*z, (float)sqx - sqy - sqz + sqw );
		euler.z = asin ( (float)2.0f * testValue / unit );
		euler.x = atan2( (float)2.0f*x*w - 2.0f*y*z, (float)-sqx + sqy - sqz + sqw );

		return euler;
	}

	void Quaternion::FromMatrix(const Matrix4& matrix)
	{
		float trace = matrix.Get(0, 0) + matrix.Get(1, 1) + matrix.Get(2, 2);
		if (trace > 0)
		{
			float root = (float)sqrt(trace + 1);
			w = root * 0.5f;

			root = 0.5f / root;
			x = (matrix.Get(2, 1) - matrix.Get(1, 2)) * root;
			y = (matrix.Get(0, 2) - matrix.Get(2, 0)) * root;
			z = (matrix.Get(1, 0) - matrix.Get(0, 1)) * root;
		}
		else
		{
			int next[3] = { 1, 2, 0 };

			int i = 0;
			if (matrix.Get(1, 1) > matrix.Get(0, 0)) i = 1;
			if (matrix.Get(2, 2) > matrix.Get(i, i)) i = 2;

			int j = next[i];
			int k = next[j];

			float root = sqrt(matrix.Get(i, i) - matrix.Get(j, j) - matrix.Get(k, k) + 1);

			x = 0.5f * root;
			root = 0.5f / root;
			y = (matrix.Get(j, i) + matrix.Get(i, j)) * root;
			z = (matrix.Get(k, i) + matrix.Get(i, k)) * root;
			w = (matrix.Get(k, j) - matrix.Get(j, k)) * root;
		}
	}

	void Quaternion::Slerp(const Quaternion &quat1, const Quaternion &quat2, float time)
	{
		float omega, cosom, sinom, sclp, sclq;

		cosom = quat1.x*quat2.x + quat1.y*quat2.y + quat1.z*quat2.z + quat1.w*quat2.w;


		if ((1.0f+cosom) > maths::FLOAT_EPSILON)
		{
			if ((1.0f-cosom) > maths::FLOAT_EPSILON)
			{
				omega = acos(cosom);
				sinom = sin(omega);
				sclp = sin((1.0f-time)*omega) / sinom;
				sclq = sin(time*omega) / sinom;
			}
			else
			{
				sclp = 1.0f - time;
				sclq = time;
			}

			x = sclp*quat1.x + sclq*quat2.x;
			y = sclp*quat1.y + sclq*quat2.y;
			z = sclp*quat1.z + sclq*quat2.z;
			w = sclp*quat1.w + sclq*quat2.w;

		}
		else
		{
			x =-quat1.y;
			y = quat1.x;
			z =-quat1.w;
			w = quat1.z;

			sclp = (float)sin((1.0f - time) * maths::PI * 0.5f);
			sclq = (float)sin(time * maths::PI * 0.5f);

			x = sclp*quat1.x + sclq*quat2.x;
			y = sclp*quat1.y + sclq*quat2.y;
			z = sclp*quat1.z + sclq*quat2.z;
		}
	}

	void Quaternion::Exp()
	{
		float Mul;
		float Length = sqrt(x*x + y*y + z*z);

		if (Length > 1.0e-4)
			Mul = sin(Length) / Length;
		else
			Mul = 1.0;

		w = cos(Length);

		x *= Mul;
		y *= Mul;
		z *= Mul;
	}

	void Quaternion::Log()
	{
		float Length;

		Length = sqrt(x*x + y*y + z*z);
		Length = atan(Length / w);

		w = 0.0;

		z *= Length;
		y *= Length;
		z *= Length;
	}

	void Quaternion::Serialise(io::Archive& archive)
	{
		archive.Serialise(x);
		archive.Serialise(y);
		archive.Serialise(z);
		archive.Serialise(w);
	}
}