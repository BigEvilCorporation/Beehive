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
			m_camera = NULL;
		}

		CameraController::~CameraController()
		{
		}

		void CameraController::SetCurrentCamera(ion::render::Camera* camera)
		{
			m_camera = camera;
		}

		ion::render::Camera* CameraController::GetCurrentCamera() const
		{
			return m_camera;
		}
	}
}