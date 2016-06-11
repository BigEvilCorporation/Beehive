///////////////////////////////////////////////////
// File:		CameraThirdPerson.h
// Date:		10th December 2012
// Authors:		Matt Phillips
// Description:	Third person camera controller
///////////////////////////////////////////////////

#include "CameraThirdPerson.h"

namespace ion
{
	namespace gamekit
	{
		CameraThirdPerson::CameraThirdPerson()
		{
			m_moveSpeed = 10.0f;
		}

		CameraThirdPerson::~CameraThirdPerson()
		{

		}

		void CameraThirdPerson::SetTargetHeadPosition(ion::Vector3& targetPosition)
		{
			m_targetHeadPosition = targetPosition;
		}

		void CameraThirdPerson::SetTargetFocusPosition(ion::Vector3& targetPosition)
		{
			m_targetFocusPosition = targetPosition;
		}

		void CameraThirdPerson::SetMoveSpeed(float speed)
		{
			m_moveSpeed = speed;
		}

		void CameraThirdPerson::Update(float deltaTime)
		{
			render::Camera* camera = GetCurrentCamera();

			if(camera)
			{
				const ion::Matrix4& transform = camera->GetTransform();
				const ion::Vector3& currentPosition = transform.GetTranslation();

				ion::Vector3 newPosition = currentPosition.Lerp(m_targetHeadPosition, m_moveSpeed * deltaTime);
				ion::Vector3 newFocus = m_focusPosition.Lerp(m_targetFocusPosition, m_moveSpeed * deltaTime);

				camera->SetPosition(newPosition);
				camera->SetLookAt(newFocus);

				m_focusPosition = newFocus;
			}
		}
	}
}