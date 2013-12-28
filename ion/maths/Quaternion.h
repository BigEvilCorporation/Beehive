/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Quaternion.h

	Quaternion rotation class
*/

#pragma once

#include "maths/Maths.h"
#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "io/archive.h"

namespace ion
{
	class Quaternion
	{
	public:

		float x, y, z, w;

		Quaternion();
		Quaternion(float X, float Y, float Z, float W);
		Quaternion(float W, const Vector3 &axis);

		void Invert();
		void Conjugate();
		void Normalise();
		void Slerp(const Quaternion& quat1, const Quaternion& quat2, float time);

		float Dot(const Quaternion& quat) const;
		Quaternion GetInverse() const;
		Quaternion GetConjugate() const;
		Quaternion GetToPowerOf(float n) const;
		
		void FromAxis(float radian, const Vector3& axis);
		void FromEuler(const Vector3& euler);
		void FromMatrix(const Matrix4& Matrix);
		void FromLookAt(const Vector3& forward, const Vector3& position, const Vector3& target, float factor);

		Matrix4 ToMatrix() const;
		Vector3 ToEuler() const;

		void Exp();
		void Log();

		Quaternion operator * (const Quaternion& quat) const;
		void operator *= (const Quaternion& quat);
		Quaternion operator +(const Quaternion& quat) const;
		Vector3 operator* (const Vector3 &vector) const;

		//Serialisation
		void Serialise(io::Archive& archive);
	};
}
