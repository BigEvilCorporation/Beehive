///////////////////////////////////////////////////
// File:		Camera.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Camera
///////////////////////////////////////////////////

#pragma once

#include "renderer/Entity.h"

namespace ion
{
	namespace render
	{
		class Camera : public Entity
		{
		public:
			Camera();
			~Camera();

			void SetZoom(const Vector3& zoom);
		};
	}
}