/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Quaternion.cpp

	Quaternion rotation class
*/

#include "Quaternion.h"

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

	Quaternion::Quaternion(float Degrees, Vector3 &Axis)
	{
		x = Axis.x;
		y = Axis.y;
		z = Axis.z;
		w = Degrees;
	}

	Quaternion Quaternion::operator * (const Quaternion &Quat) const
	{
		float X = w * Quat.x + Quat.w * x + y * Quat.z - z * Quat.y;
		float Y = w * Quat.y + Quat.w * y + z * Quat.x - x * Quat.z;
		float Z = w * Quat.z + Quat.w * z + x * Quat.y - y * Quat.x;
		float W = w * Quat.w - (x * Quat.x + y * Quat.y + z * Quat.z);

		return Quaternion(X, Y, Z, W);
	}

	void Quaternion::operator *= (const Quaternion &Quat)
	{
		float W = w * Quat.w - (x * Quat.x + y * Quat.y + z * Quat.z);

		float X = w * Quat.x + Quat.w * x + y * Quat.z - z * Quat.y;
		float Y = w * Quat.y + Quat.w * y + z * Quat.x - x * Quat.z;
		float Z = w * Quat.z + Quat.w * z + x * Quat.y - y * Quat.x;

		w = W;
		x = X;
		y = Y;
		z = Z;
	}

	Vector3 Quaternion::operator* (const Vector3 &Vector) const
	{
		Vector3 vn = Vector;
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

	Quaternion Quaternion::operator +(const Quaternion& Quat) const
	{
		return Quaternion(x + Quat.x, y + Quat.y, z + Quat.z, w + Quat.w);
	}


	void Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	void Quaternion::Invert()
	{
		float Norm = sqrt(w * w + x * x + y * y + z * z);
		if (Norm == 0.0) Norm = 1.0;

		float Recip = 1.0f / Norm;

		x = -x * Recip;
		y = -y * Recip;
		z = -z * Recip;
		w =  w * Recip;
	}

	void Quaternion::Normalise()
	{
		float Norm = sqrt(w * w + x * x + y * y + z * z);
		if (Norm == 0.0)
		{
			w = 1.0;
			x = y = z = 0.0;
		}
		else
		{
			float Recip = 1.0f / Norm;

			x *= Recip;
			y *= Recip;
			z *= Recip;
			w *= Recip;
		}
	}

	float Quaternion::Dot(Quaternion& Quat) const
	{
		return (x * Quat.x) + (y * Quat.y) + (z * Quat.z) + (w * Quat.w);
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

	void Quaternion::FromAxis(float Angle, Vector3 Axis)
	{
		w = 0;
		x = Axis.x;
		y = Axis.y;
		z = Axis.z;
		if (x == 0.0f && y == 0.0f && z == 0.0f) x = 1.0f;
		Normalise();

		Angle = Angle / 2;
		w = (float)cos(Angle);

		float Scale = (float)sin(Angle);
		x *= Scale;
		y *= Scale;
		z *= Scale;
	}

	void Quaternion::FromLookAt(Vector3 Forward, Vector3 Position, Vector3 Target, float Factor)
	{
		Vector3 Direction = Target - Position;
		Direction.Normalise();

		Vector3 Right = Forward.Cross(Direction);
		Right.Normalise();

		Vector3 Up = Direction.Cross(Right);
		Up.Normalise();

		Matrix4 LookAtMatrix;

		LookAtMatrix.Set(0, 0, Right.x);
		LookAtMatrix.Set(1, 0, Right.y);
		LookAtMatrix.Set(2, 0, Right.z);

		LookAtMatrix.Set(0, 1, Up.x);
		LookAtMatrix.Set(1, 1, Up.y);
		LookAtMatrix.Set(2, 1, Up.z);

		LookAtMatrix.Set(0, 2, Direction.x);
		LookAtMatrix.Set(1, 2, Direction.y);
		LookAtMatrix.Set(2, 2, Direction.z);

		Quaternion TargetQuat;
		TargetQuat.FromMatrix(LookAtMatrix);

		//Slerp current quaternion using new quaternion, by factor
		//Slerp((*this), TargetQuat, Factor);

		*this = TargetQuat;
	}

	Matrix4 Quaternion::ToMatrix() const
	{
		float Matrix[16];

		// First row
		Matrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );
		Matrix[ 1] = 2.0f * (x * y + z * w);
		Matrix[ 2] = 2.0f * (x * z - y * w);
		Matrix[ 3] = 0.0f;

		// Second row
		Matrix[ 4] = 2.0f * ( x * y - z * w );
		Matrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );
		Matrix[ 6] = 2.0f * (z * y + x * w );
		Matrix[ 7] = 0.0f;

		// Third row
		Matrix[ 8] = 2.0f * ( x * z + y * w );
		Matrix[ 9] = 2.0f * ( y * z - x * w );
		Matrix[10] = 1.0f - 2.0f * ( x * x + y * y );
		Matrix[11] = 0.0f;

		// Fourth row
		Matrix[12] = 0;
		Matrix[13] = 0;
		Matrix[14] = 0;
		Matrix[15] = 1.0f;

		Matrix4 SMatrix(Matrix);

		return SMatrix;
	}

	void Quaternion::FromEuler(Vector3 Euler)
	{
		float cosYaw = cos(Euler.y / 2);
		float sinYaw = sin(Euler.y / 2);
		float cosPitch = cos(Euler.x / 2);
		float sinPitch = sin(Euler.x / 2);
		float cosRoll = cos(Euler.z / 2);
		float sinRoll = sin(Euler.z / 2);
		x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	}

	Vector3 Quaternion::ToEuler() const
	{
		Vector3 Euler;
		float testValue = x * y + z * w;
		if(testValue > 0.499f)
		{
			Euler.y = 2 * atan2(x,w);
			Euler.z = maths::PI / 2;
			Euler.x = 0.0f;
			return Euler;
		}
		if(testValue < -0.499f)
		{
			Euler.y = -2 * atan2(x,w);
			Euler.z = -maths::PI / 2;
			Euler.x = 0.0f;
			return Euler;
		}

		float sqx = x*x;
		float sqy = y*y;
		float sqz = z*z;
		float sqw = w*w;
		float unit = sqx + sqy + sqz + sqw;

		Euler.y = atan2( (float)2.0f*y*w - 2.0f*x*z, (float)sqx - sqy - sqz + sqw );
		Euler.z = asin ( (float)2.0f * testValue / unit );
		Euler.x = atan2( (float)2.0f*x*w - 2.0f*y*z, (float)-sqx + sqy - sqz + sqw );

		return Euler;
	}

	void Quaternion::FromMatrix(const Matrix4& Matrix)
	{
		float trace = Matrix.Get(0, 0) + Matrix.Get(1, 1) + Matrix.Get(2, 2);
		if (trace > 0)
		{
			float root = (float)sqrt(trace + 1);
			w = root * 0.5f;

			root = 0.5f / root;
			x = (Matrix.Get(2, 1) - Matrix.Get(1, 2)) * root;
			y = (Matrix.Get(0, 2) - Matrix.Get(2, 0)) * root;
			z = (Matrix.Get(1, 0) - Matrix.Get(0, 1)) * root;
		}
		else
		{
			int next[3] = { 1, 2, 0 };

			int i = 0;
			if (Matrix.Get(1, 1) > Matrix.Get(0, 0)) i = 1;
			if (Matrix.Get(2, 2) > Matrix.Get(i, i)) i = 2;

			int j = next[i];
			int k = next[j];

			float root = sqrt(Matrix.Get(i, i) - Matrix.Get(j, j) - Matrix.Get(k, k) + 1);

			x = 0.5f * root;
			root = 0.5f / root;
			y = (Matrix.Get(j, i) + Matrix.Get(i, j)) * root;
			z = (Matrix.Get(k, i) + Matrix.Get(i, k)) * root;
			w = (Matrix.Get(k, j) - Matrix.Get(j, k)) * root;
		}
	}

	void Quaternion::Slerp(Quaternion &Quat1, Quaternion &Quat2, float t)
	{
		float omega, cosom, sinom, sclp, sclq;

		cosom = Quat1.x*Quat2.x + Quat1.y*Quat2.y + Quat1.z*Quat2.z + Quat1.w*Quat2.w;


		if ((1.0f+cosom) > maths::FLOAT_EPSILON)
		{
			if ((1.0f-cosom) > maths::FLOAT_EPSILON)
			{
				omega = acos(cosom);
				sinom = sin(omega);
				sclp = sin((1.0f-t)*omega) / sinom;
				sclq = sin(t*omega) / sinom;
			}
			else
			{
				sclp = 1.0f - t;
				sclq = t;
			}

			x = sclp*Quat1.x + sclq*Quat2.x;
			y = sclp*Quat1.y + sclq*Quat2.y;
			z = sclp*Quat1.z + sclq*Quat2.z;
			w = sclp*Quat1.w + sclq*Quat2.w;

		}
		else
		{
			x =-Quat1.y;
			y = Quat1.x;
			z =-Quat1.w;
			w = Quat1.z;

			sclp = (float)sin((1.0f - t) * maths::PI * 0.5f);
			sclq = (float)sin(t * maths::PI * 0.5f);

			x = sclp*Quat1.x + sclq*Quat2.x;
			y = sclp*Quat1.y + sclq*Quat2.y;
			z = sclp*Quat1.z + sclq*Quat2.z;
		}
	}

	void Quaternion::exp()
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

	void Quaternion::log()
	{
		float Length;

		Length = sqrt(x*x + y*y + z*z);
		Length = atan(Length / w);

		w = 0.0;

		z *= Length;
		y *= Length;
		z *= Length;
	}

	void Quaternion::Serialise(serialise::Archive& archive)
	{
		archive.Serialise(x);
		archive.Serialise(y);
		archive.Serialise(z);
		archive.Serialise(w);
	}
}