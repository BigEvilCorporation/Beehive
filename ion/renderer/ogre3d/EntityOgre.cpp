///////////////////////////////////////////////////
// File:		EntityNodeOgre.cpp
// Date:		27th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3dentity implementation
///////////////////////////////////////////////////

#include "EntityOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			EntityOgre::EntityOgre()
			{
				mOgreMovable = new Ogre::Movable();
			}

			EntityOgre::~EntityOgre()
			{
				delete mOgreMovable;
			}

			void EntityOgre::SetTransform(const Matrix4& matrix)
			{
				Entity::SetTransform(matrix);
				SetOgreTransform();
			}

			void EntityOgre::SetPosition(const Vector3& position)
			{
				Entity::SetPosition(position);
				SetOgreTransform();
			}

			void EntityOgre::SetOrientation(const Quaternion& orientation)
			{
				Entity::SetOrientation(orientation);
				SetOgreTransform();
			}

			void EntityOgre::SetLookAt(const Vector3& position)
			{
				Entity::SetLookAt(position);
				SetOgreTransform();
			}

			void EntityOgre::Move(const Vector3& moveVector)
			{
				Entity::Move(moveVector);
				SetOgreTransform();
			}

			void EntityOgre::Pitch(float pitch, TransformSpace transformSpace)
			{
				Entity::Pitch(pitch, transformSpace);
				SetOgreTransform();
			}

			void EntityOgre::Yaw(float yaw, TransformSpace transformSpace)
			{
				Entity::Yaw(yaw, transformSpace);
				SetOgreTransform();
			}

			void Entity::Roll(roll, transformSpace)
			{
				SetOgreTransform();
			}

			void EntityOgre::SetOgreTransform()
			{
				if(mOgreMovable)
				{
					const Matrix4& ionTransform = GetTransform();
					Vector3 ionTranslation = ionTransform.GetTranslation();
					Quaternion ionOrientation;
					ionOrientation.FromMatrix(ionTransform);
					Ogre::Vector3 translation(ionTranslation.x, ionTranslation.y, ionTranslation.z);
					Ogre::Quaternion orientation(ionOrientation.w, ionOrientation.x, ionOrientation.y, ionOrientation.z);
					mOgreMovable->setPosition(translation);
					mOgreMovable->setOrientation(orientation);
				}
			}
		}
	}
}