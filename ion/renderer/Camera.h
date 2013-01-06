///////////////////////////////////////////////////
// File:		Camera.h
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre camera
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"
#include "../core/maths/Matrix.h"
#include "../core/maths/Quaternion.h"

#include <Ogre/OgreCamera.h>

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Scene;

		class Camera
		{
		public:
			enum DrawMode { Solid, Wireframe };
			enum Projection { Perspective, Orthographic };

			Camera(Scene& scene);
			~Camera();
			
			void SetPosition(const Vector3& position);
			void LookAt(const Vector3& lookat);
			void Move(const Vector3& moveVector);
			void Pitch(float pitch);
			void Yaw(float yaw);
			void Roll(float roll);

			const Vector3& GetPosition();

			void SetDrawMode(DrawMode drawMode);
			void SetProjection(Projection projection);
			void SetOrthoDimensions(float width, float height);
			void SetAspectRatio(float aspectRatio);

			Ogre::Camera* GetOgreCameraIFace();

		private:
			Ogre::Camera* mOgreCameraIFace;
			Vector3 mPosition;
		};
	}
}