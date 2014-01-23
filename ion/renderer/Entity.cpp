///////////////////////////////////////////////////
// File:		Entity.cpp
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Entity (transformable) class
///////////////////////////////////////////////////

#pragma once

#include "renderer/Entity.h"
#include "renderer/CoordSys.h"

namespace ion
{
	namespace render
	{
		Entity::Entity()
		{
		}

		Entity::~Entity()
		{
		}

		const Matrix4& Entity::GetTransform() const
		{
			return mMatrix;
		}

		void Entity::SetTransform(const Matrix4& transform)
		{
			mMatrix = transform;
		}

		void Entity::SetPosition(const Vector3& position)
		{
			mMatrix.SetTranslation(position);
		}

		void Entity::SetOrientation(const Quaternion& orientation)
		{
			Vector3 translation = mMatrix.GetTranslation();
			Quaternion normQuat = orientation;
			normQuat.Normalise();
			mMatrix = normQuat.ToMatrix();
			mMatrix.SetTranslation(translation);
		}

		void Entity::SetLookAt(const Vector3& position)
		{
			Quaternion orientation;
			orientation.FromLookAt(mMatrix.GetTranslation(), position, ion::Vector3(0.0f, 0.0f, -1.0f));
			SetOrientation(orientation);
		}

		void Entity::Move(const Vector3& moveVector)
		{
			mMatrix.SetTranslation(mMatrix.GetTranslation() + mMatrix.RotateVector(moveVector));
		}

		void Entity::Pitch(float pitch, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == Local)
			{
				quat.FromAxis(pitch, mMatrix.GetRight());
			}
			else
			{
				quat.FromAxis(pitch, coordsys::Right);
			}

			mMatrix = quat.ToMatrix() * mMatrix;
		}

		void Entity::Yaw(float yaw, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == Local)
			{
				quat.FromAxis(yaw, mMatrix.GetUp());
			}
			else
			{
				quat.FromAxis(yaw, coordsys::Up);
			}

			mMatrix = quat.ToMatrix() * mMatrix;
		}

		void Entity::Roll(float roll, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == Local)
			{
				quat.FromAxis(roll, mMatrix.GetForward());
			}
			else
			{
				quat.FromAxis(roll, coordsys::Forward);
			}

			mMatrix = quat.ToMatrix() * mMatrix;
		}
	}
}