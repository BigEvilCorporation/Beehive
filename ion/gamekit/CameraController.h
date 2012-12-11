///////////////////////////////////////////////////
// File:		CameraController.h
// Date:		10th December 2012
// Authors:		Matt Phillips
// Description:	Camera controller base
///////////////////////////////////////////////////

#pragma once

#include "renderer/Camera.h"

namespace ion
{
	namespace gamekit
	{
		class CameraController
		{
		public:
			CameraController();
			virtual ~CameraController();

			void SetCurrentCamera(ion::renderer::Camera* camera);

			virtual void Update() {}

		protected:
			ion::renderer::Camera* GetCurrentCamera() const;

		private:
			ion::renderer::Camera* mCamera;
		};
	}
}