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
			render::Camera* camera = GetCurrentCamera();

			if(camera)
			{
				const ion::Matrix4& transform = camera->GetTransform();
				const ion::Vector3& currentPosition = transform.GetTranslation();

				ion::Vector3 newPosition = currentPosition.Lerp(mTargetHeadPosition, mMoveSpeed * deltaTime);
				ion::Vector3 newFocus = mFocusPosition.Lerp(mTargetFocusPosition, mMoveSpeed * deltaTime);

				camera->SetPosition(newPosition);
				camera->SetLookAt(newFocus);

				mFocusPosition = newFocus;
			}
		}
	}
}