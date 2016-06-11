///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
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