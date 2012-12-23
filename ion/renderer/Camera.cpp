///////////////////////////////////////////////////
// File:		Camera.cpp
// Date:		3rd August 2011
// Authors:		Matt Phillips
// Description:	Ogre camera
///////////////////////////////////////////////////

#include "Camera.h"
#include "Scene.h"

namespace ion
{
	namespace renderer
	{
		Camera::Camera(Scene& scene)
		{
			#if defined ION_OGRE
			mOgreCameraIFace = scene.GetOgreSceneMgrIFace()->createCamera("Camera");
			mOgreCameraIFace->setNearClipDistance(0.1f);
			mOgreCameraIFace->setFarClipDistance(1000.0f);
			#endif
		}

		Camera::~Camera()
		{
			//Ogre::Camera gets destroyed with the Scene
		}
			
		void Camera::SetPosition(const Vector3& position)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->setPosition(position.x, position.y, position.z);
			}
			#endif

			mPosition = position;
		}

		const Vector3& Camera::GetPosition()
		{
			#if defined ION_OGRE
			const Ogre::Vector3& ogrePos = mOgreCameraIFace->getPosition();
			mPosition.x = ogrePos.x;
			mPosition.y = ogrePos.y;
			mPosition.z = ogrePos.z;
			#endif

			return mPosition;
		}

		void Camera::LookAt(const Vector3& lookat)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->lookAt(lookat.x, lookat.y, lookat.z);
			}
			#endif
		}

		void Camera::Move(const Vector3& moveVector)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->moveRelative(Ogre::Vector3(moveVector.x, moveVector.y, moveVector.z));
			}
			#endif
		}

		void Camera::Pitch(float pitch)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->pitch(Ogre::Radian(pitch));
			}
			#endif
		}

		void Camera::Yaw(float yaw)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->yaw(Ogre::Radian(yaw));
			}
			#endif
		}

		void Camera::Roll(float roll)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				mOgreCameraIFace->roll(Ogre::Radian(roll));
			}
			#endif
		}

		void Camera::SetDrawMode(DrawMode drawMode)
		{
			#if defined ION_OGRE
			if(mOgreCameraIFace)
			{
				Ogre::PolygonMode polyMode = Ogre::PM_SOLID;

				switch(drawMode)
				{
				case Solid:
					polyMode = Ogre::PM_SOLID;
					break;
				case Wireframe:
					polyMode = Ogre::PM_WIREFRAME;
					break;
				}

				mOgreCameraIFace->setPolygonMode(polyMode);
			}
			#endif
		}

		Ogre::Camera* Camera::GetOgreCameraIFace()
		{
			return mOgreCameraIFace;
		}
	}
}