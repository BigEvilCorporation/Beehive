///////////////////////////////////////////////////
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
//
// File:		CameraThirdPerson.h
// Date:		10th December 2012
// Authors:		Matt Phillips
// Description:	Third person camera controller
///////////////////////////////////////////////////

#pragma once

#include "CameraController.h"

namespace ion
{
	namespace gamekit
	{
		class CameraThirdPerson : public CameraController
		{
		public:
			CameraThirdPerson();
			virtual ~CameraThirdPerson();

			void SetTargetHeadPosition(ion::Vector3& targetPosition);
			void SetTargetFocusPosition(ion::Vector3& targetPosition);

			void SetMoveSpeed(float speed);

			virtual void Update(float deltaTime);

		private:
			ion::Vector3 m_targetHeadPosition;
			ion::Vector3 m_targetFocusPosition;

			Vector3 m_focusPosition;

			float m_moveSpeed;
		};
	}
}