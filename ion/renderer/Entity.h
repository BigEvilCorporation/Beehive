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

			virtual const Matrix4& GetTransform() const;

			virtual void SetTransform(const Matrix4& matrix);
			virtual void SetPosition(const Vector3& position);
			virtual void SetOrientation(const Quaternion& orientation);
			virtual void SetLookAt(const Vector3& position);

			virtual void Move(const Vector3& moveVector);
			virtual void Pitch(float pitch, TransformSpace transformSpace = Local);
			virtual void Yaw(float yaw, TransformSpace transformSpace = Local);
			virtual void Roll(float roll, TransformSpace transformSpace = Local);

		private:
			Matrix4 mMatrix;
		};
	}
}