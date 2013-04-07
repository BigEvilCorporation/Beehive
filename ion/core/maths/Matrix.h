/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Matrix.h

	4x4 matrix class
*/

#pragma once

#include "Maths.h"
#include "Vector.h"

namespace ion
{
	class Matrix4
	{
	public:

		Matrix4();
		Matrix4(float* Float16);
		Matrix4(const Matrix4 & Mat);
		Matrix4(const Vector3& Position, const Vector3& Up, const Vector3& Forward);
		~Matrix4();

		void SetIdentity();

		void SetTranslation(const Vector3& Pos);
		void SetRotation(float Angle, const Vector3& Axis);
		void SetScale(const Vector3& Scale);
		void SetForward(const Vector3& Forward);
		void SetRight(const Vector3& Right);
		void SetUp(const Vector3& Up);

		Vector3 GetTranslation() const;
		Vector3 GetScale() const;
		Vector3 GetForward() const;
		Vector3 GetRight() const;
		Vector3 GetUp() const;

		Matrix4 GetTranspose() const;
		Matrix4 GetInverse() const;
		Matrix4 GetProduct(const Matrix4& Mat) const;
		Matrix4 GetInterpolated(const Matrix4& Mat, float Time) const;

		Vector3 TransformVector(const Vector3& Vector) const;
		Vector3 RotateVector(const Vector3& Vector) const;
		Vector3 UnrotateVector(const Vector3& Vector) const;

		Vector3 operator *(const Vector3& Vec) const;
		Matrix4 operator *(const Matrix4& Mat) const;
		Matrix4 operator *(float Scalar) const;
		Matrix4 operator +(const Matrix4& Mat) const;
		Matrix4 operator -(const Matrix4& Mat) const;

		const float* GetAsFloatArray() const;

		float Get(const int col, const int row) const;
		void Set(const int col, const int row, const float val);

	protected:

		//The 4x4 matrix
		float mMatrix[16];
	};
}

