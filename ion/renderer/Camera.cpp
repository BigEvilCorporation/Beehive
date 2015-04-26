///////////////////////////////////////////////////
// File:		Camera.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Camera
///////////////////////////////////////////////////

#include "renderer/Camera.h"

namespace ion
{
	namespace render
	{
		Camera::Camera()
		{

		}

		Camera::~Camera()
		{

		}

		void Camera::SetZoom(const Vector3& zoom)
		{
			Matrix4 transform = GetTransform();
			transform.SetScale(Vector3(1.0f, 1.0f, 1.0f) / zoom);
			SetTransform(transform);
		}
	}
}