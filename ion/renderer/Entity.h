///////////////////////////////////////////////////
// File:		Entity.h
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Entity (transformable) class
///////////////////////////////////////////////////

#pragma once

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/Quaternion.h"

namespace ion
{
	namespace render
	{
		class Entity
		{
		public:
			enum TransformSpace { Local, World };

			Entity();
			virtual ~Entity();

			const Matrix4& GetTransform() const;

			Vector3 GetPosition() const;

			void SetTransform(const Matrix4& matrix);
			void SetPosition(const Vector3& position);
			void SetOrientation(const Quaternion& orientation);
			void SetLookAt(const Vector3& position);

			void Move(const Vector3& moveVector);
			void Pitch(float pitch, TransformSpace transformSpace = Local);
			void Yaw(float yaw, TransformSpace transformSpace = Local);
			void Roll(float roll, TransformSpace transformSpace = Local);

		private:
			Matrix4 mMatrix;
		};
	}
}