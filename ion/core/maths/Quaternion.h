/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Quaternion.h

	Quaternion rotation class
*/

#pragma once

#include "Maths.h"
#include "Vector.h"
#include "Matrix.h"
#include "../../Core/Archive.h"

namespace ion
{
	class Quaternion
	{
	public:

		float x, y, z, w;

		Quaternion();
		Quaternion(float X, float Y, float Z, float W);
		Quaternion(float Degrees, Vector3 &Axis);

		void Invert();
		void Conjugate();
		void Normalise();
		void Slerp(Quaternion& Quat1, Quaternion& Quat2, float);

		float Dot(Quaternion& Quat) const;
		Quaternion GetInverse() const;
		Quaternion GetConjugate() const;
		Quaternion GetToPowerOf(float n) const;
		
		void FromAxis(float Angle, Vector3 Axis);
		void FromEuler(Vector3 Euler);
		void FromMatrix(const Matrix4& Matrix);
		void FromLookAt(Vector3 Forward, Vector3 Position, Vector3 Target, float Factor);

		Matrix4 ToMatrix() const;
		Vector3 ToEuler() const;

		void exp();
		void log();

		Quaternion operator * (const Quaternion& Quat) const;
		void operator *= (const Quaternion& Quat);
		Quaternion operator +(const Quaternion& Quat) const;
		Vector3 operator* (const Vector3 &Vector) const;

		//Serialisation
		void Serialise(serialise::Archive& archive);
	};
}
