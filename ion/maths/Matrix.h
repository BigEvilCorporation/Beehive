/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Matrix.h

	4x4 matrix class
*/

#pragma once

#include "maths/Maths.h"
#include "maths/Vector.h"
#include "io/archive.h"

namespace ion
{
	class Matrix4
	{
	public:

		Matrix4();
		Matrix4(float* float16);
		Matrix4(const Matrix4& mat);
		Matrix4(const Vector3& position, const Vector3& up, const Vector3& forward);
		~Matrix4();

		void SetIdentity();

		void SetTranslation(const Vector3& pos);
		void SetRotation(float agle, const Vector3& axis);
		void SetScale(const Vector3& scale);
		void SetForward(const Vector3& forward);
		void SetRight(const Vector3& right);
		void SetUp(const Vector3& up);

		Vector3 GetTranslation() const;
		Vector3 GetScale() const;
		Vector3 GetForward() const;
		Vector3 GetRight() const;
		Vector3 GetUp() const;

		Matrix4 GetTranspose() const;
		Matrix4 GetInverse() const;
		Matrix4 GetProduct(const Matrix4& mat) const;
		Matrix4 GetInterpolated(const Matrix4& mat, float time) const;

		Vector3 TransformVector(const Vector3& vector) const;
		Vector3 RotateVector(const Vector3& vector) const;
		Vector3 UnrotateVector(const Vector3& vector) const;

		Vector3 operator *(const Vector3& vec) const;
		Matrix4 operator *(const Matrix4& mat) const;
		Matrix4 operator *(float scalar) const;
		Matrix4 operator +(const Matrix4& mat) const;
		Matrix4 operator -(const Matrix4& mat) const;

		float* GetAsFloatArray();
		const float* GetAsFloatArray() const;
		float operator[] (int index) const;

		float Get(const int col, const int row) const;
		void Set(const int col, const int row, const float val);

		//Serialisation
		void Serialise(io::Archive& archive);

	protected:

		//The 4x4 matrix
		float m_matrix[16];
	};
}

