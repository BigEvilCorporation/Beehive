/*
	Symmetry Engine 2.0
	Matt Phillips (c) 2009

	Matrix.h

	4x4 matrix class
*/

#include "maths/Matrix.h"

namespace ion
{
	Matrix4::Matrix4()
	{
		SetIdentity();
	}

	Matrix4::Matrix4(float* float16)
	{
		for(int i = 0; i < 16; i++)
		{
			m_matrix[i] = float16[i];
		}
	}

	Matrix4::Matrix4(const Matrix4 & mat)
	{
		for(int i = 0; i < 16; i++)
		{
			m_matrix[i] = mat.m_matrix[i];
		}
	}

	Matrix4::Matrix4(const Vector3& position, const Vector3& up, const Vector3& forward)
	{
		Vector3 axisZ = forward - position;
		axisZ.Normalise();

		Vector3 axisX = up.Cross(axisZ);
		axisX.Normalise();

		Vector3 axisY = axisZ.Cross(axisX);
		m_matrix[0] = axisX.x;
		m_matrix[1] = axisY.x;
		m_matrix[2] = axisZ.x;
		m_matrix[3] = 0;

		m_matrix[4] = axisX.y;
		m_matrix[5] = axisY.y;
		m_matrix[6] = axisZ.y;
		m_matrix[7] = 0;

		m_matrix[8] = axisX.z;
		m_matrix[9] = axisY.z;
		m_matrix[10] = axisZ.z;
		m_matrix[11] = 0;

		m_matrix[12] = -axisX.Dot(position);
		m_matrix[13] = -axisY.Dot(position);
		m_matrix[14] = -axisZ.Dot(position);
		m_matrix[15] = 1.0f;
	}

	Matrix4::~Matrix4()
	{

	}

	void Matrix4::SetTranslation(const Vector3& pos)
	{
		Set(3, 0, pos.x);
		Set(3, 1, pos.y);
		Set(3, 2, pos.z);
	}

	void Matrix4::SetRotation(float angle, const Vector3& axis)
	{
		float s = (float)sin(-angle * 0.0174532925);
		float c = (float)cos(-angle * 0.0174532925);
		float t = 1 - c;

		Vector3 ax;
		Vector3 axNorm;

		ax.x = axis.x;
		ax.y = axis.y;
		ax.z = axis.z;

		Vector3 ax2 = ax / sqrt((ax * ax).GetLength());

		float x = ax2.x;
		float y = ax2.y;
		float z = ax2.z;

		Set(0,0,t*x*x+c);
		Set(1,0,t*y*x+s*z);
		Set(2,0,t*z*x-s*y);

		Set(0,1,t*x*y-s*z);
		Set(1,1,t*y*y+c);
		Set(2,1,t*z*y+s*x);

		Set(0,2,t*x*z+s*y);
		Set(1,2,t*y*z-s*x);
		Set(2,2,t*z*z+c);
	}

	void Matrix4::SetScale(const Vector3& scale)
	{
		SetRight(GetRight().Normalise() * scale.x);
		SetUp(GetUp().Normalise() * scale.y);
		SetForward(GetForward().Normalise() * scale.z);
	}

	void Matrix4::SetForward(const Vector3& forward)
	{
		Set(2, 0, forward.x);
		Set(2, 1, forward.y);
		Set(2, 2, forward.z);
	}

	void Matrix4::SetRight(const Vector3& right)
	{
		Set(0, 0, right.x);
		Set(0, 1, right.y);
		Set(0, 2, right.z);
	}

	void Matrix4::SetUp(const Vector3& up)
	{
		Set(1, 0, up.x);
		Set(1, 1, up.y);
		Set(1, 2, up.z);
	}

	Vector3 Matrix4::TransformVector(const Vector3& vector) const
	{
		return Vector3(	vector.x * Get(0, 0) + vector.y * Get(1, 0) + vector.z * Get(2, 0) + Get(3, 0),
						vector.x * Get(0, 1) + vector.y * Get(1, 1) + vector.z * Get(2, 1) + Get(3, 1),
						vector.x * Get(0, 2) + vector.y * Get(1, 2) + vector.z * Get(2, 2) + Get(3, 2));
	}

	Vector3 Matrix4::RotateVector(const Vector3& vector) const
	{
		return Vector3(	vector.x * Get(0, 0) + vector.y * Get(1, 0) + vector.z * Get(2, 0),
						vector.x * Get(0, 1) + vector.y * Get(1, 1) + vector.z * Get(2, 1),
						vector.x * Get(0, 2) + vector.y * Get(1, 2) + vector.z * Get(2, 2));
	}

	Vector3 Matrix4::UnrotateVector(const Vector3& vector) const
	{
		return Vector3(	vector.x * Get(0, 0) + vector.y * Get(0, 1) + vector.z * Get(0, 2),
						vector.x * Get(1, 0) + vector.y * Get(1, 1) + vector.z * Get(1, 2),
						vector.x * Get(2, 0) + vector.y * Get(2, 1) + vector.z * Get(2, 2));
	}

	Vector3 Matrix4::GetTranslation() const
	{
		return Vector3(Get(3, 0), Get(3, 1), Get(3, 2));
	}

	Vector3 Matrix4::GetScale() const
	{
		return Vector3(GetRight().GetLength(), GetUp().GetLength(), GetForward().GetLength());
	}

	Vector3 Matrix4::GetForward() const
	{
		return Vector3(Get(2, 0), Get(2, 1), Get(2, 2)).Normalise();
	}

	Vector3 Matrix4::GetRight() const
	{
		return Vector3(Get(0, 0), Get(0, 1), Get(0, 2)).Normalise();
	}

	Vector3 Matrix4::GetUp() const
	{
		return Vector3(Get(1, 0), Get(1, 1), Get(1, 2)).Normalise();
	}

	void Matrix4::SetIdentity()
	{
		static float identity[16] =
		{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};

		for(int i = 0; i < 16; i++)
		{
			m_matrix[i] = identity[i];
		}
	}

	Matrix4 Matrix4::GetTranspose() const
	{
		Matrix4 result;

		//Loop through column
		for (int i = 0; i < 4; i++)
		{
			//Loop through rows
			for (int j = i + 1; j < 4; j++)
			{
				//Save value
				float save = Get(i, j);

				//Transpose value
				result.Set(i, j, Get(j, i));

				//Restore saved value
				result.Set(j, i, save);
			}
		}

		return result;
	}

	Matrix4 Matrix4::GetInverse() const
	{
		Matrix4 result;

		float d =   (Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0)) * (Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2)) -
			(Get(0, 0) * Get(1, 2) - Get(0, 2) * Get(1, 0)) * (Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1)) +
			(Get(0, 0) * Get(1, 3) - Get(0, 3) * Get(1, 0)) * (Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1)) +
			(Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1)) * (Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0)) -
			(Get(0, 1) * Get(1, 3) - Get(0, 3) * Get(1, 1)) * (Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0)) +
			(Get(0, 2) * Get(1, 3) - Get(0, 3) * Get(1, 2)) * (Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0));

		const float precisionLimit = 1.0e-4f;

		d = 1.0f / d;

		if(fabsf(d) < precisionLimit)
		{
			result.SetIdentity();
			return result;
		}

		result.Set(0, 0, (d * (Get(1, 1) * (Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2)) + Get(1, 2) * (Get(2, 3) * Get(3, 1) - Get(2, 1) * Get(3, 3)) + Get(1, 3) * (Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1)))));
		result.Set(0, 1, (d * (Get(2, 1) * (Get(0, 2) * Get(3, 3) - Get(0, 3) * Get(3, 2)) + Get(2, 2) * (Get(0, 3) * Get(3, 1) - Get(0, 1) * Get(3, 3)) + Get(2, 3) * (Get(0, 1) * Get(3, 2) - Get(0, 2) * Get(3, 1)))));
		result.Set(0, 2, (d * (Get(3, 1) * (Get(0, 2) * Get(1, 3) - Get(0, 3) * Get(1, 2)) + Get(3, 2) * (Get(0, 3) * Get(1, 1) - Get(0, 1) * Get(1, 3)) + Get(3, 3) * (Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1)))));
		result.Set(0, 3, (d * (Get(0, 1) * (Get(1, 3) * Get(2, 2) - Get(1, 2) * Get(2, 3)) + Get(0, 2) * (Get(1, 1) * Get(2, 3) - Get(1, 3) * Get(2, 1)) + Get(0, 3) * (Get(1, 2) * Get(2, 1) - Get(1, 1) * Get(2, 2)))));
		result.Set(1, 0, (d * (Get(1, 2) * (Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0)) + Get(1, 3) * (Get(2, 2) * Get(3, 0) - Get(2, 0) * Get(3, 2)) + Get(1, 0) * (Get(2, 3) * Get(3, 2) - Get(2, 2) * Get(3, 3)))));
		result.Set(1, 1, (d * (Get(2, 2) * (Get(0, 0) * Get(3, 3) - Get(0, 3) * Get(3, 0)) + Get(2, 3) * (Get(0, 2) * Get(3, 0) - Get(0, 0) * Get(3, 2)) + Get(2, 0) * (Get(0, 3) * Get(3, 2) - Get(0, 2) * Get(3, 3)))));
		result.Set(1, 2, (d * (Get(3, 2) * (Get(0, 0) * Get(1, 3) - Get(0, 3) * Get(1, 0)) + Get(3, 3) * (Get(0, 2) * Get(1, 0) - Get(0, 0) * Get(1, 2)) + Get(3, 0) * (Get(0, 3) * Get(1, 2) - Get(0, 2) * Get(1, 3)))));
		result.Set(1, 3, (d * (Get(0, 2) * (Get(1, 3) * Get(2, 0) - Get(1, 0) * Get(2, 3)) + Get(0, 3) * (Get(1, 0) * Get(2, 2) - Get(1, 2) * Get(2, 0)) + Get(0, 0) * (Get(1, 2) * Get(2, 3) - Get(1, 3) * Get(2, 2)))));
		result.Set(2, 0, (d * (Get(1, 3) * (Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0)) + Get(1, 0) * (Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1)) + Get(1, 1) * (Get(2, 3) * Get(3, 0) - Get(2, 0) * Get(3, 3)))));
		result.Set(2, 1, (d * (Get(2, 3) * (Get(0, 0) * Get(3, 1) - Get(0, 1) * Get(3, 0)) + Get(2, 0) * (Get(0, 1) * Get(3, 3) - Get(0, 3) * Get(3, 1)) + Get(2, 1) * (Get(0, 3) * Get(3, 0) - Get(0, 0) * Get(3, 3)))));
		result.Set(2, 2, (d * (Get(3, 3) * (Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0)) + Get(3, 0) * (Get(0, 1) * Get(1, 3) - Get(0, 3) * Get(1, 1)) + Get(3, 1) * (Get(0, 3) * Get(1, 0) - Get(0, 0) * Get(1, 3)))));
		result.Set(2, 3, (d * (Get(0, 3) * (Get(1, 1) * Get(2, 0) - Get(1, 0) * Get(2, 1)) + Get(0, 0) * (Get(1, 3) * Get(2, 1) - Get(1, 1) * Get(2, 3)) + Get(0, 1) * (Get(1, 0) * Get(2, 3) - Get(1, 3) * Get(2, 0)))));
		result.Set(3, 0, (d * (Get(1, 0) * (Get(2, 2) * Get(3, 1) - Get(2, 1) * Get(3, 2)) + Get(1, 1) * (Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0)) + Get(1, 2) * (Get(2, 1) * Get(3, 0) - Get(2, 0) * Get(3, 1)))));
		result.Set(3, 1, (d * (Get(2, 0) * (Get(0, 2) * Get(3, 1) - Get(0, 1) * Get(3, 2)) + Get(2, 1) * (Get(0, 0) * Get(3, 2) - Get(0, 2) * Get(3, 0)) + Get(2, 2) * (Get(0, 1) * Get(3, 0) - Get(0, 0) * Get(3, 1)))));
		result.Set(3, 2, (d * (Get(3, 0) * (Get(0, 2) * Get(1, 1) - Get(0, 1) * Get(1, 2)) + Get(3, 1) * (Get(0, 0) * Get(1, 2) - Get(0, 2) * Get(1, 0)) + Get(3, 2) * (Get(0, 1) * Get(1, 0) - Get(0, 0) * Get(1, 1)))));
		result.Set(3, 3, (d * (Get(0, 0) * (Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1)) + Get(0, 1) * (Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2)) + Get(0, 2) * (Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0)))));

		return result;
	}

	Matrix4 Matrix4::GetProduct(const Matrix4& Mat) const
	{
		Matrix4 result;

		result.m_matrix[0] = m_matrix[0]*Mat.m_matrix[0] + m_matrix[4]*Mat.m_matrix[1] + m_matrix[8]*Mat.m_matrix[2] + m_matrix[12]*Mat.m_matrix[3];
		result.m_matrix[1] = m_matrix[1]*Mat.m_matrix[0] + m_matrix[5]*Mat.m_matrix[1] + m_matrix[9]*Mat.m_matrix[2] + m_matrix[13]*Mat.m_matrix[3];
		result.m_matrix[2] = m_matrix[2]*Mat.m_matrix[0] + m_matrix[6]*Mat.m_matrix[1] + m_matrix[10]*Mat.m_matrix[2] + m_matrix[14]*Mat.m_matrix[3];
		result.m_matrix[3] = m_matrix[3]*Mat.m_matrix[0] + m_matrix[7]*Mat.m_matrix[1] + m_matrix[11]*Mat.m_matrix[2] + m_matrix[15]*Mat.m_matrix[3];

		result.m_matrix[4] = m_matrix[0]*Mat.m_matrix[4] + m_matrix[4]*Mat.m_matrix[5] + m_matrix[8]*Mat.m_matrix[6] + m_matrix[12]*Mat.m_matrix[7];
		result.m_matrix[5] = m_matrix[1]*Mat.m_matrix[4] + m_matrix[5]*Mat.m_matrix[5] + m_matrix[9]*Mat.m_matrix[6] + m_matrix[13]*Mat.m_matrix[7];
		result.m_matrix[6] = m_matrix[2]*Mat.m_matrix[4] + m_matrix[6]*Mat.m_matrix[5] + m_matrix[10]*Mat.m_matrix[6] + m_matrix[14]*Mat.m_matrix[7];
		result.m_matrix[7] = m_matrix[3]*Mat.m_matrix[4] + m_matrix[7]*Mat.m_matrix[5] + m_matrix[11]*Mat.m_matrix[6] + m_matrix[15]*Mat.m_matrix[7];

		result.m_matrix[8] = m_matrix[0]*Mat.m_matrix[8] + m_matrix[4]*Mat.m_matrix[9] + m_matrix[8]*Mat.m_matrix[10] + m_matrix[12]*Mat.m_matrix[11];
		result.m_matrix[9] = m_matrix[1]*Mat.m_matrix[8] + m_matrix[5]*Mat.m_matrix[9] + m_matrix[9]*Mat.m_matrix[10] + m_matrix[13]*Mat.m_matrix[11];
		result.m_matrix[10] = m_matrix[2]*Mat.m_matrix[8] + m_matrix[6]*Mat.m_matrix[9] + m_matrix[10]*Mat.m_matrix[10] + m_matrix[14]*Mat.m_matrix[11];
		result.m_matrix[11] = m_matrix[3]*Mat.m_matrix[8] + m_matrix[7]*Mat.m_matrix[9] + m_matrix[11]*Mat.m_matrix[10] + m_matrix[15]*Mat.m_matrix[11];

		result.m_matrix[12] = m_matrix[0]*Mat.m_matrix[12] + m_matrix[4]*Mat.m_matrix[13] + m_matrix[8]*Mat.m_matrix[14] + m_matrix[12]*Mat.m_matrix[15];
		result.m_matrix[13] = m_matrix[1]*Mat.m_matrix[12] + m_matrix[5]*Mat.m_matrix[13] + m_matrix[9]*Mat.m_matrix[14] + m_matrix[13]*Mat.m_matrix[15];
		result.m_matrix[14] = m_matrix[2]*Mat.m_matrix[12] + m_matrix[6]*Mat.m_matrix[13] + m_matrix[10]*Mat.m_matrix[14] + m_matrix[14]*Mat.m_matrix[15];
		result.m_matrix[15] = m_matrix[3]*Mat.m_matrix[12] + m_matrix[7]*Mat.m_matrix[13] + m_matrix[11]*Mat.m_matrix[14] + m_matrix[15]*Mat.m_matrix[15];

		return result;
	}

	Matrix4 Matrix4::GetInterpolated(const Matrix4& Mat, float Time) const
	{
		Matrix4 result;

		for (int i = 0; i < 16; i += 4)
		{
			result.m_matrix[i+0] = m_matrix[i+0] + ( Mat.m_matrix[i+0] - m_matrix[i+0] ) * Time;
			result.m_matrix[i+1] = m_matrix[i+1] + ( Mat.m_matrix[i+1] - m_matrix[i+1] ) * Time;
			result.m_matrix[i+2] = m_matrix[i+2] + ( Mat.m_matrix[i+2] - m_matrix[i+2] ) * Time;
			result.m_matrix[i+3] = m_matrix[i+3] + ( Mat.m_matrix[i+3] - m_matrix[i+3] ) * Time;
		}

		return result;
	}

	Vector3 Matrix4::operator *(const Vector3& vec) const
	{
		return TransformVector(vec);
	}

	Matrix4 Matrix4::operator *(const Matrix4& mat) const
	{
		Matrix4 result;

		result.m_matrix[ 0] = m_matrix[ 0] * mat.m_matrix[ 0] + m_matrix[ 1] * mat.m_matrix[ 4] + m_matrix[ 2] * mat.m_matrix[ 8] + m_matrix[ 3] * mat.m_matrix[12];
		result.m_matrix[ 1] = m_matrix[ 0] * mat.m_matrix[ 1] + m_matrix[ 1] * mat.m_matrix[ 5] + m_matrix[ 2] * mat.m_matrix[ 9] + m_matrix[ 3] * mat.m_matrix[13];
		result.m_matrix[ 2] = m_matrix[ 0] * mat.m_matrix[ 2] + m_matrix[ 1] * mat.m_matrix[ 6] + m_matrix[ 2] * mat.m_matrix[10] + m_matrix[ 3] * mat.m_matrix[14];
		result.m_matrix[ 3] = m_matrix[ 0] * mat.m_matrix[ 3] + m_matrix[ 1] * mat.m_matrix[ 7] + m_matrix[ 2] * mat.m_matrix[11] + m_matrix[ 3] * mat.m_matrix[15];

		result.m_matrix[ 4] = m_matrix[ 4] * mat.m_matrix[ 0] + m_matrix[ 5] * mat.m_matrix[ 4] + m_matrix[ 6] * mat.m_matrix[ 8] + m_matrix[ 7] * mat.m_matrix[12];
		result.m_matrix[ 5] = m_matrix[ 4] * mat.m_matrix[ 1] + m_matrix[ 5] * mat.m_matrix[ 5] + m_matrix[ 6] * mat.m_matrix[ 9] + m_matrix[ 7] * mat.m_matrix[13];
		result.m_matrix[ 6] = m_matrix[ 4] * mat.m_matrix[ 2] + m_matrix[ 5] * mat.m_matrix[ 6] + m_matrix[ 6] * mat.m_matrix[10] + m_matrix[ 7] * mat.m_matrix[14];
		result.m_matrix[ 7] = m_matrix[ 4] * mat.m_matrix[ 3] + m_matrix[ 5] * mat.m_matrix[ 7] + m_matrix[ 6] * mat.m_matrix[11] + m_matrix[ 7] * mat.m_matrix[15];

		result.m_matrix[ 8] = m_matrix[ 8] * mat.m_matrix[ 0] + m_matrix[ 9] * mat.m_matrix[ 4] + m_matrix[10] * mat.m_matrix[ 8] + m_matrix[11] * mat.m_matrix[12];
		result.m_matrix[ 9] = m_matrix[ 8] * mat.m_matrix[ 1] + m_matrix[ 9] * mat.m_matrix[ 5] + m_matrix[10] * mat.m_matrix[ 9] + m_matrix[11] * mat.m_matrix[13];
		result.m_matrix[10] = m_matrix[ 8] * mat.m_matrix[ 2] + m_matrix[ 9] * mat.m_matrix[ 6] + m_matrix[10] * mat.m_matrix[10] + m_matrix[11] * mat.m_matrix[14];
		result.m_matrix[11] = m_matrix[ 8] * mat.m_matrix[ 3] + m_matrix[ 9] * mat.m_matrix[ 7] + m_matrix[10] * mat.m_matrix[11] + m_matrix[11] * mat.m_matrix[15];

		result.m_matrix[12] = m_matrix[12] * mat.m_matrix[ 0] + m_matrix[13] * mat.m_matrix[ 4] + m_matrix[14] * mat.m_matrix[ 8] + m_matrix[15] * mat.m_matrix[12];
		result.m_matrix[13] = m_matrix[12] * mat.m_matrix[ 1] + m_matrix[13] * mat.m_matrix[ 5] + m_matrix[14] * mat.m_matrix[ 9] + m_matrix[15] * mat.m_matrix[13];
		result.m_matrix[14] = m_matrix[12] * mat.m_matrix[ 2] + m_matrix[13] * mat.m_matrix[ 6] + m_matrix[14] * mat.m_matrix[10] + m_matrix[15] * mat.m_matrix[14];
		result.m_matrix[15] = m_matrix[12] * mat.m_matrix[ 3] + m_matrix[13] * mat.m_matrix[ 7] + m_matrix[14] * mat.m_matrix[11] + m_matrix[15] * mat.m_matrix[15];

		return result;
	}

	Matrix4 Matrix4::operator *(float scalar) const
	{
		float result[16];

		for(int i = 0; i < 16; i++)
		{
			result[i] = m_matrix[i] * scalar;
		}

		return Matrix4(result);
	}

	Matrix4 Matrix4::operator +(const Matrix4& mat) const
	{
		float result[16];
		const float* input = mat.GetAsFloatArray();

		for(int i = 0; i < 16; i++)
		{
			result[i] = m_matrix[i] + input[i];
		}

		return Matrix4(result);
	}

	Matrix4 Matrix4::operator -(const Matrix4& mat) const
	{
		float result[16];
		const float* input = mat.GetAsFloatArray();

		for(int i = 0; i < 16; i++)
		{
			result[i] = m_matrix[i] - input[i];
		}

		return Matrix4(result);
	}

	float * Matrix4::GetAsFloatArray()
	{
		return m_matrix;
	}

	const float * Matrix4::GetAsFloatArray() const
	{
		return m_matrix;
	}

	float Matrix4::operator[] (int index) const
	{
		return m_matrix[index];
	}

	float Matrix4::Get(const int col, const int row) const
	{
		return m_matrix[col * 4 + row];
	}

	void Matrix4::Set(const int col, const int row, const float val)
	{
		m_matrix[col * 4 + row] = val;
	}

	void Matrix4::Serialise(io::Archive& archive)
	{
		for(int i = 0; i < 16; i++)
		{
			archive.Serialise(m_matrix[i]);
		}
	}
}
