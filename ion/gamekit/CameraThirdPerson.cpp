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
			mMoveSpeed = 10.0f;
		}

		CameraThirdPerson::~CameraThirdPerson()
		{

		}

		void CameraThirdPerson::SetTargetHeadPosition(ion::Vector3& targetPosition)
		{
			mTargetHeadPosition = targetPosition;
		}

		void CameraThirdPerson::SetTargetFocusPosition(ion::Vector3& targetPosition)
		{
			mTargetFocusPosition = targetPosition;
		}

		void CameraThirdPerson::SetMoveSpeed(float speed)
		{
			mMoveSpeed = speed;
		}

		void CameraThirdPerson::Update(float deltaTime)
		{
			renderer::Camera* camera = GetCurrentCamera();

			if(camera)
			{
				const ion::Vector3& currentPosition = camera->GetPosition();

				ion::Vector3 newPosition = currentPosition.Lerp(mTargetHeadPosition, mMoveSpeed * deltaTime);
				ion::Vector3 newFocus = mFocusPosition.Lerp(mTargetFocusPosition, mMoveSpeed * deltaTime);

				camera->SetPosition(newPosition);
				camera->LookAt(newFocus);

				mFocusPosition = newFocus;
			}
		}
	}
}