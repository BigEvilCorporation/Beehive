///////////////////////////////////////////////////
// File:		EntityOgre.h
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d entity implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreEntity.h>

#include "renderer/Entity.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class EntityOgre : public Entity
			{
			public:
				EntityOgre();
				~EntityOgre();

				virtual void SetTransform(const Matrix4& matrix);
				virtual void SetPosition(const Vector3& position);
				virtual void SetOrientation(const Quaternion& orientation);
				virtual void SetLookAt(const Vector3& position);

				virtual void Move(const Vector3& moveVector);
				virtual void Pitch(float pitch, TransformSpace transformSpace = Local);
				virtual void Yaw(float yaw, TransformSpace transformSpace = Local);
				virtual void Roll(float roll, TransformSpace transformSpace = Local);

				void SetOgreTransform();
				Ogre::MovableObject* mOgreMovable;
			};
		}

		typedef ogre::EntityOgre EntityImpl;
	}
}