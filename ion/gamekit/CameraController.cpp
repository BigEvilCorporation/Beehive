///////////////////////////////////////////////////
// File:		CameraController.cpp
// Date:		10th December 2012
// Authors:		Matt Phillips
// Description:	Camera controller base
///////////////////////////////////////////////////

#include "CameraController.h"

namespace ion
{
	namespace gamekit
	{
		CameraController::CameraController()
		{
			mCamera = NULL;
		}

		CameraController::~CameraController()
		{
		}

		void CameraController::SetCurrentCamera(ion::renderer::Camera* camera)
		{
			mCamera = camera;
		}

		ion::renderer::Camera* CameraController::GetCurrentCamera() const
		{
			return mCamera;
		}
	}
}