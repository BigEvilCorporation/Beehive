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

	Matrix4::Matrix4(float* Float16)
	{
		for(int i = 0; i < 16; i++)
		{
			mMatrix[i] = Float16[i];
		}
	}

	Matrix4::Matrix4(const Matrix4 & Mat)
	{
		for(int i = 0; i < 16; i++)
		{
			mMatrix[i] = Mat.mMatrix[i];
		}
	}

	Matrix4::Matrix4(const Vector3& Position, const Vector3& Up, const Vector3& Forward)
	{
		Vector3 AxisZ = Forward - Position;
		AxisZ.Normalise();

		Vector3 AxisX = Up.Cross(AxisZ);
		AxisX.Normalise();

		Vector3 AxisY = AxisZ.Cross(AxisX);
		mMatrix[0] = AxisX.x;
		mMatrix[1] = AxisY.x;
		mMatrix[2] = AxisZ.x;
		mMatrix[3] = 0;

		mMatrix[4] = AxisX.y;
		mMatrix[5] = AxisY.y;
		mMatrix[6] = AxisZ.y;
		mMatrix[7] = 0;

		mMatrix[8] = AxisX.z;
		mMatrix[9] = AxisY.z;
		mMatrix[10] = AxisZ.z;
		mMatrix[11] = 0;

		mMatrix[12] = -AxisX.Dot(Position);
		mMatrix[13] = -AxisY.Dot(Position);
		mMatrix[14] = -AxisZ.Dot(Position);
		mMatrix[15] = 1.0f;
	}

	Matrix4::~Matrix4()
	{

	}

	void Matrix4::SetTranslation(const Vector3& Pos)
	{
		Set(3, 0, Pos.x);
		Set(3, 1, Pos.y);
		Set(3, 2, Pos.z);
	}

	void Matrix4::SetRotation(float Angle, const Vector3& Axis)
	{
		float s = (float)sin(-Angle * 0.0174532925);
		float c = (float)cos(-Angle * 0.0174532925);
		float t = 1 - c;

		Vector3 Ax;
		Vector3 AxNorm;

		Ax.x = Axis.x;
		Ax.y = Axis.y;
		Ax.z = Axis.z;

		Vector3 Ax2 = Ax / sqrt((Ax * Ax).GetLength());

		float x = Ax2.x;
		float y = Ax2.y;
		float z = Ax2.z;

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

	void Matrix4::SetScale(const Vector3& Scale)
	{
		SetRight(GetRight().Normalise() * Scale.x);
		SetUp(GetUp().Normalise() * Scale.y);
		SetForward(GetForward().Normalise() * Scale.z);
	}

	void Matrix4::SetForward(const Vector3& Forward)
	{
		Set(2, 0, Forward.x);
		Set(2, 1, Forward.y);
		Set(2, 2, Forward.z);
	}

	void Matrix4::SetRight(const Vector3& Right)
	{
		Set(0, 0, Right.x);
		Set(0, 1, Right.y);
		Set(0, 2, Right.z);
	}

	void Matrix4::SetUp(const Vector3& Up)
	{
		Set(1, 0, Up.x);
		Set(1, 1, Up.y);
		Set(1, 2, Up.z);
	}

	Vector3 Matrix4::TransformVector(const Vector3& Vector) const
	{
		return Vector3(	Vector.x * Get(0, 0) + Vector.y * Get(1, 0) + Vector.z * Get(2, 0) + Get(3, 0),
						Vector.x * Get(0, 1) + Vector.y * Get(1, 1) + Vector.z * Get(2, 1) + Get(3, 1),
						Vector.x * Get(0, 2) + Vector.y * Get(1, 2) + Vector.z * Get(2, 2) + Get(3, 2));
	}

	Vector3 Matrix4::RotateVector(const Vector3& Vector) const
	{
		return Vector3(	Vector.x * Get(0, 0) + Vector.y * Get(1, 0) + Vector.z * Get(2, 0),
						Vector.x * Get(0, 1) + Vector.y * Get(1, 1) + Vector.z * Get(2, 1),
						Vector.x * Get(0, 2) + Vector.y * Get(1, 2) + Vector.z * Get(2, 2));
	}

	Vector3 Matrix4::UnrotateVector(const Vector3& Vector) const
	{
		return Vector3(	Vector.x * Get(0, 0) + Vector.y * Get(0, 1) + Vector.z * Get(0, 2),
						Vector.x * Get(1, 0) + Vector.y * Get(1, 1) + Vector.z * Get(1, 2),
						Vector.x * Get(2, 0) + Vector.y * Get(2, 1) + Vector.z * Get(2, 2));
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
		static float Id[16] =
		{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};

		for(int i = 0; i < 16; i++)
		{
			mMatrix[i] = Id[i];
		}
	}

	Matrix4 Matrix4::GetTranspose() const
	{
		Matrix4 Result;

		//Loop through column
		for (int i = 0; i < 4; i++)
		{
			//Loop through rows
			for (int j = i + 1; j < 4; j++)
			{
				//Save value
				float save = Get(i, j);

				//Transpose value
				Result.Set(i, j, Get(j, i));

				//Restore saved value
				Result.Set(j, i, save);
			}
		}

		return Result;
	}

	Matrix4 Matrix4::GetInverse() const
	{
		Matrix4 Matrix;

		float d =   (Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0)) * (Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2)) -
			(Get(0, 0) * Get(1, 2) - Get(0, 2) * Get(1, 0)) * (Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1)) +
			(Get(0, 0) * Get(1, 3) - Get(0, 3) * Get(1, 0)) * (Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1)) +
			(Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1)) * (Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0)) -
			(Get(0, 1) * Get(1, 3) - Get(0, 3) * Get(1, 1)) * (Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0)) +
			(Get(0, 2) * Get(1, 3) - Get(0, 3) * Get(1, 2)) * (Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0));

		const float PrecisionLimit = 1.0e-4f;

		d = 1.0f / d;

		if(fabsf(d) < PrecisionLimit)
		{
			Matrix.SetIdentity();
			return Matrix;
		}

		Matrix.Set(0, 0, (d * (Get(1, 1) * (Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2)) + Get(1, 2) * (Get(2, 3) * Get(3, 1) - Get(2, 1) * Get(3, 3)) + Get(1, 3) * (Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1)))));
		Matrix.Set(0, 1, (d * (Get(2, 1) * (Get(0, 2) * Get(3, 3) - Get(0, 3) * Get(3, 2)) + Get(2, 2) * (Get(0, 3) * Get(3, 1) - Get(0, 1) * Get(3, 3)) + Get(2, 3) * (Get(0, 1) * Get(3, 2) - Get(0, 2) * Get(3, 1)))));
		Matrix.Set(0, 2, (d * (Get(3, 1) * (Get(0, 2) * Get(1, 3) - Get(0, 3) * Get(1, 2)) + Get(3, 2) * (Get(0, 3) * Get(1, 1) - Get(0, 1) * Get(1, 3)) + Get(3, 3) * (Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1)))));
		Matrix.Set(0, 3, (d * (Get(0, 1) * (Get(1, 3) * Get(2, 2) - Get(1, 2) * Get(2, 3)) + Get(0, 2) * (Get(1, 1) * Get(2, 3) - Get(1, 3) * Get(2, 1)) + Get(0, 3) * (Get(1, 2) * Get(2, 1) - Get(1, 1) * Get(2, 2)))));
		Matrix.Set(1, 0, (d * (Get(1, 2) * (Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0)) + Get(1, 3) * (Get(2, 2) * Get(3, 0) - Get(2, 0) * Get(3, 2)) + Get(1, 0) * (Get(2, 3) * Get(3, 2) - Get(2, 2) * Get(3, 3)))));
		Matrix.Set(1, 1, (d * (Get(2, 2) * (Get(0, 0) * Get(3, 3) - Get(0, 3) * Get(3, 0)) + Get(2, 3) * (Get(0, 2) * Get(3, 0) - Get(0, 0) * Get(3, 2)) + Get(2, 0) * (Get(0, 3) * Get(3, 2) - Get(0, 2) * Get(3, 3)))));
		Matrix.Set(1, 2, (d * (Get(3, 2) * (Get(0, 0) * Get(1, 3) - Get(0, 3) * Get(1, 0)) + Get(3, 3) * (Get(0, 2) * Get(1, 0) - Get(0, 0) * Get(1, 2)) + Get(3, 0) * (Get(0, 3) * Get(1, 2) - Get(0, 2) * Get(1, 3)))));
		Matrix.Set(1, 3, (d * (Get(0, 2) * (Get(1, 3) * Get(2, 0) - Get(1, 0) * Get(2, 3)) + Get(0, 3) * (Get(1, 0) * Get(2, 2) - Get(1, 2) * Get(2, 0)) + Get(0, 0) * (Get(1, 2) * Get(2, 3) - Get(1, 3) * Get(2, 2)))));
		Matrix.Set(2, 0, (d * (Get(1, 3) * (Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0)) + Get(1, 0) * (Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1)) + Get(1, 1) * (Get(2, 3) * Get(3, 0) - Get(2, 0) * Get(3, 3)))));
		Matrix.Set(2, 1, (d * (Get(2, 3) * (Get(0, 0) * Get(3, 1) - Get(0, 1) * Get(3, 0)) + Get(2, 0) * (Get(0, 1) * Get(3, 3) - Get(0, 3) * Get(3, 1)) + Get(2, 1) * (Get(0, 3) * Get(3, 0) - Get(0, 0) * Get(3, 3)))));
		Matrix.Set(2, 2, (d * (Get(3, 3) * (Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0)) + Get(3, 0) * (Get(0, 1) * Get(1, 3) - Get(0, 3) * Get(1, 1)) + Get(3, 1) * (Get(0, 3) * Get(1, 0) - Get(0, 0) * Get(1, 3)))));
		Matrix.Set(2, 3, (d * (Get(0, 3) * (Get(1, 1) * Get(2, 0) - Get(1, 0) * Get(2, 1)) + Get(0, 0) * (Get(1, 3) * Get(2, 1) - Get(1, 1) * Get(2, 3)) + Get(0, 1) * (Get(1, 0) * Get(2, 3) - Get(1, 3) * Get(2, 0)))));
		Matrix.Set(3, 0, (d * (Get(1, 0) * (Get(2, 2) * Get(3, 1) - Get(2, 1) * Get(3, 2)) + Get(1, 1) * (Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0)) + Get(1, 2) * (Get(2, 1) * Get(3, 0) - Get(2, 0) * Get(3, 1)))));
		Matrix.Set(3, 1, (d * (Get(2, 0) * (Get(0, 2) * Get(3, 1) - Get(0, 1) * Get(3, 2)) + Get(2, 1) * (Get(0, 0) * Get(3, 2) - Get(0, 2) * Get(3, 0)) + Get(2, 2) * (Get(0, 1) * Get(3, 0) - Get(0, 0) * Get(3, 1)))));
		Matrix.Set(3, 2, (d * (Get(3, 0) * (Get(0, 2) * Get(1, 1) - Get(0, 1) * Get(1, 2)) + Get(3, 1) * (Get(0, 0) * Get(1, 2) - Get(0, 2) * Get(1, 0)) + Get(3, 2) * (Get(0, 1) * Get(1, 0) - Get(0, 0) * Get(1, 1)))));
		Matrix.Set(3, 3, (d * (Get(0, 0) * (Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1)) + Get(0, 1) * (Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2)) + Get(0, 2) * (Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0)))));

		return Matrix;
	}

	Matrix4 Matrix4::GetProduct(const Matrix4& Mat) const
	{
		Matrix4 Result;

		Result.mMatrix[0] = mMatrix[0]*Mat.mMatrix[0] + mMatrix[4]*Mat.mMatrix[1] + mMatrix[8]*Mat.mMatrix[2] + mMatrix[12]*Mat.mMatrix[3];
		Result.mMatrix[1] = mMatrix[1]*Mat.mMatrix[0] + mMatrix[5]*Mat.mMatrix[1] + mMatrix[9]*Mat.mMatrix[2] + mMatrix[13]*Mat.mMatrix[3];
		Result.mMatrix[2] = mMatrix[2]*Mat.mMatrix[0] + mMatrix[6]*Mat.mMatrix[1] + mMatrix[10]*Mat.mMatrix[2] + mMatrix[14]*Mat.mMatrix[3];
		Result.mMatrix[3] = mMatrix[3]*Mat.mMatrix[0] + mMatrix[7]*Mat.mMatrix[1] + mMatrix[11]*Mat.mMatrix[2] + mMatrix[15]*Mat.mMatrix[3];

		Result.mMatrix[4] = mMatrix[0]*Mat.mMatrix[4] + mMatrix[4]*Mat.mMatrix[5] + mMatrix[8]*Mat.mMatrix[6] + mMatrix[12]*Mat.mMatrix[7];
		Result.mMatrix[5] = mMatrix[1]*Mat.mMatrix[4] + mMatrix[5]*Mat.mMatrix[5] + mMatrix[9]*Mat.mMatrix[6] + mMatrix[13]*Mat.mMatrix[7];
		Result.mMatrix[6] = mMatrix[2]*Mat.mMatrix[4] + mMatrix[6]*Mat.mMatrix[5] + mMatrix[10]*Mat.mMatrix[6] + mMatrix[14]*Mat.mMatrix[7];
		Result.mMatrix[7] = mMatrix[3]*Mat.mMatrix[4] + mMatrix[7]*Mat.mMatrix[5] + mMatrix[11]*Mat.mMatrix[6] + mMatrix[15]*Mat.mMatrix[7];

		Result.mMatrix[8] = mMatrix[0]*Mat.mMatrix[8] + mMatrix[4]*Mat.mMatrix[9] + mMatrix[8]*Mat.mMatrix[10] + mMatrix[12]*Mat.mMatrix[11];
		Result.mMatrix[9] = mMatrix[1]*Mat.mMatrix[8] + mMatrix[5]*Mat.mMatrix[9] + mMatrix[9]*Mat.mMatrix[10] + mMatrix[13]*Mat.mMatrix[11];
		Result.mMatrix[10] = mMatrix[2]*Mat.mMatrix[8] + mMatrix[6]*Mat.mMatrix[9] + mMatrix[10]*Mat.mMatrix[10] + mMatrix[14]*Mat.mMatrix[11];
		Result.mMatrix[11] = mMatrix[3]*Mat.mMatrix[8] + mMatrix[7]*Mat.mMatrix[9] + mMatrix[11]*Mat.mMatrix[10] + mMatrix[15]*Mat.mMatrix[11];

		Result.mMatrix[12] = mMatrix[0]*Mat.mMatrix[12] + mMatrix[4]*Mat.mMatrix[13] + mMatrix[8]*Mat.mMatrix[14] + mMatrix[12]*Mat.mMatrix[15];
		Result.mMatrix[13] = mMatrix[1]*Mat.mMatrix[12] + mMatrix[5]*Mat.mMatrix[13] + mMatrix[9]*Mat.mMatrix[14] + mMatrix[13]*Mat.mMatrix[15];
		Result.mMatrix[14] = mMatrix[2]*Mat.mMatrix[12] + mMatrix[6]*Mat.mMatrix[13] + mMatrix[10]*Mat.mMatrix[14] + mMatrix[14]*Mat.mMatrix[15];
		Result.mMatrix[15] = mMatrix[3]*Mat.mMatrix[12] + mMatrix[7]*Mat.mMatrix[13] + mMatrix[11]*Mat.mMatrix[14] + mMatrix[15]*Mat.mMatrix[15];

		return Result;
	}

	Matrix4 Matrix4::GetInterpolated(const Matrix4& Mat, float Time) const
	{
		Matrix4 Result;

		for (int i = 0; i < 16; i += 4)
		{
			Result.mMatrix[i+0] = mMatrix[i+0] + ( Mat.mMatrix[i+0] - mMatrix[i+0] ) * Time;
			Result.mMatrix[i+1] = mMatrix[i+1] + ( Mat.mMatrix[i+1] - mMatrix[i+1] ) * Time;
			Result.mMatrix[i+2] = mMatrix[i+2] + ( Mat.mMatrix[i+2] - mMatrix[i+2] ) * Time;
			Result.mMatrix[i+3] = mMatrix[i+3] + ( Mat.mMatrix[i+3] - mMatrix[i+3] ) * Time;
		}

		return Result;
	}

	Vector3 Matrix4::operator *(const Vector3& Vec) const
	{
		return TransformVector(Vec);
	}

	Matrix4 Matrix4::operator *(const Matrix4& Mat) const
	{
		Matrix4 Temp;

		Temp.mMatrix[ 0] = mMatrix[ 0] * Mat.mMatrix[ 0] + mMatrix[ 1] * Mat.mMatrix[ 4] + mMatrix[ 2] * Mat.mMatrix[ 8] + mMatrix[ 3] * Mat.mMatrix[12];
		Temp.mMatrix[ 1] = mMatrix[ 0] * Mat.mMatrix[ 1] + mMatrix[ 1] * Mat.mMatrix[ 5] + mMatrix[ 2] * Mat.mMatrix[ 9] + mMatrix[ 3] * Mat.mMatrix[13];
		Temp.mMatrix[ 2] = mMatrix[ 0] * Mat.mMatrix[ 2] + mMatrix[ 1] * Mat.mMatrix[ 6] + mMatrix[ 2] * Mat.mMatrix[10] + mMatrix[ 3] * Mat.mMatrix[14];
		Temp.mMatrix[ 3] = mMatrix[ 0] * Mat.mMatrix[ 3] + mMatrix[ 1] * Mat.mMatrix[ 7] + mMatrix[ 2] * Mat.mMatrix[11] + mMatrix[ 3] * Mat.mMatrix[15];

		Temp.mMatrix[ 4] = mMatrix[ 4] * Mat.mMatrix[ 0] + mMatrix[ 5] * Mat.mMatrix[ 4] + mMatrix[ 6] * Mat.mMatrix[ 8] + mMatrix[ 7] * Mat.mMatrix[12];
		Temp.mMatrix[ 5] = mMatrix[ 4] * Mat.mMatrix[ 1] + mMatrix[ 5] * Mat.mMatrix[ 5] + mMatrix[ 6] * Mat.mMatrix[ 9] + mMatrix[ 7] * Mat.mMatrix[13];
		Temp.mMatrix[ 6] = mMatrix[ 4] * Mat.mMatrix[ 2] + mMatrix[ 5] * Mat.mMatrix[ 6] + mMatrix[ 6] * Mat.mMatrix[10] + mMatrix[ 7] * Mat.mMatrix[14];
		Temp.mMatrix[ 7] = mMatrix[ 4] * Mat.mMatrix[ 3] + mMatrix[ 5] * Mat.mMatrix[ 7] + mMatrix[ 6] * Mat.mMatrix[11] + mMatrix[ 7] * Mat.mMatrix[15];

		Temp.mMatrix[ 8] = mMatrix[ 8] * Mat.mMatrix[ 0] + mMatrix[ 9] * Mat.mMatrix[ 4] + mMatrix[10] * Mat.mMatrix[ 8] + mMatrix[11] * Mat.mMatrix[12];
		Temp.mMatrix[ 9] = mMatrix[ 8] * Mat.mMatrix[ 1] + mMatrix[ 9] * Mat.mMatrix[ 5] + mMatrix[10] * Mat.mMatrix[ 9] + mMatrix[11] * Mat.mMatrix[13];
		Temp.mMatrix[10] = mMatrix[ 8] * Mat.mMatrix[ 2] + mMatrix[ 9] * Mat.mMatrix[ 6] + mMatrix[10] * Mat.mMatrix[10] + mMatrix[11] * Mat.mMatrix[14];
		Temp.mMatrix[11] = mMatrix[ 8] * Mat.mMatrix[ 3] + mMatrix[ 9] * Mat.mMatrix[ 7] + mMatrix[10] * Mat.mMatrix[11] + mMatrix[11] * Mat.mMatrix[15];

		Temp.mMatrix[12] = mMatrix[12] * Mat.mMatrix[ 0] + mMatrix[13] * Mat.mMatrix[ 4] + mMatrix[14] * Mat.mMatrix[ 8] + mMatrix[15] * Mat.mMatrix[12];
		Temp.mMatrix[13] = mMatrix[12] * Mat.mMatrix[ 1] + mMatrix[13] * Mat.mMatrix[ 5] + mMatrix[14] * Mat.mMatrix[ 9] + mMatrix[15] * Mat.mMatrix[13];
		Temp.mMatrix[14] = mMatrix[12] * Mat.mMatrix[ 2] + mMatrix[13] * Mat.mMatrix[ 6] + mMatrix[14] * Mat.mMatrix[10] + mMatrix[15] * Mat.mMatrix[14];
		Temp.mMatrix[15] = mMatrix[12] * Mat.mMatrix[ 3] + mMatrix[13] * Mat.mMatrix[ 7] + mMatrix[14] * Mat.mMatrix[11] + mMatrix[15] * Mat.mMatrix[15];

		return Temp;
	}

	Matrix4 Matrix4::operator *(float Scalar) const
	{
		float Result[16];

		for(int i = 0; i < 16; i++)
		{
			Result[i] = mMatrix[i] * Scalar;
		}

		return Matrix4(Result);
	}

	Matrix4 Matrix4::operator +(const Matrix4& Mat) const
	{
		float Result[16];
		const float *Input = Mat.GetAsFloatArray();

		for(int i = 0; i < 16; i++)
		{
			Result[i] = mMatrix[i] + Input[i];
		}

		return Matrix4(Result);
	}

	Matrix4 Matrix4::operator -(const Matrix4& Mat) const
	{
		float Result[16];
		const float *Input = Mat.GetAsFloatArray();

		for(int i = 0; i < 16; i++)
		{
			Result[i] = mMatrix[i] - Input[i];
		}

		return Matrix4(Result);
	}

	float * Matrix4::GetAsFloatArray()
	{
		return mMatrix;
	}

	const float * Matrix4::GetAsFloatArray() const
	{
		return mMatrix;
	}

	float Matrix4::operator[] (int index) const
	{
		return mMatrix[index];
	}

	float Matrix4::Get(const int col, const int row) const
	{
		return mMatrix[col * 4 + row];
	}

	void Matrix4::Set(const int col, const int row, const float val)
	{
		mMatrix[col * 4 + row] = val;
	}

	void Matrix4::Serialise(io::Archive& archive)
	{
		for(int i = 0; i < 16; i++)
		{
			archive.Serialise(mMatrix[i]);
		}
	}
}
