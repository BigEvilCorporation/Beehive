///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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
			return m_matrix;
		}

		Vector3 Entity::GetPosition() const
		{
			return m_matrix.GetTranslation();
		}

		void Entity::SetTransform(const Matrix4& transform)
		{
			m_matrix = transform;
		}

		void Entity::SetPosition(const Vector3& position)
		{
			m_matrix.SetTranslation(position);
		}

		void Entity::SetOrientation(const Quaternion& orientation)
		{
			Vector3 translation = m_matrix.GetTranslation();
			Quaternion normQuat = orientation;
			normQuat.Normalise();
			m_matrix = normQuat.ToMatrix();
			m_matrix.SetTranslation(translation);
		}

		void Entity::SetLookAt(const Vector3& position)
		{
			Quaternion orientation;
			orientation.FromLookAt(m_matrix.GetTranslation(), position, ion::Vector3(0.0f, 0.0f, -1.0f));
			SetOrientation(orientation);
		}

		void Entity::Move(const Vector3& moveVector)
		{
			m_matrix.SetTranslation(m_matrix.GetTranslation() + m_matrix.RotateVector(moveVector));
		}

		void Entity::Pitch(float pitch, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == eLocal)
			{
				quat.FromAxis(pitch, m_matrix.GetRight());
			}
			else
			{
				quat.FromAxis(pitch, coordsys::Right);
			}

			m_matrix = quat.ToMatrix() * m_matrix;
		}

		void Entity::Yaw(float yaw, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == eLocal)
			{
				quat.FromAxis(yaw, m_matrix.GetUp());
			}
			else
			{
				quat.FromAxis(yaw, coordsys::Up);
			}

			m_matrix = quat.ToMatrix() * m_matrix;
		}

		void Entity::Roll(float roll, TransformSpace transformSpace)
		{
			Quaternion quat;

			if(transformSpace == eLocal)
			{
				quat.FromAxis(roll, m_matrix.GetForward());
			}
			else
			{
				quat.FromAxis(roll, coordsys::Forward);
			}

			m_matrix = quat.ToMatrix() * m_matrix;
		}
	}
}