///////////////////////////////////////////////////
// File:		CameraOgre.cpp
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d camera implementation
///////////////////////////////////////////////////

#include "CameraOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			CameraOgre::CameraOgre()
			{
			}

			CameraOgre::~CameraOgre()
			{
			}

			void CameraOgre::SetDrawMode(DrawMode drawMode)
			{
				Camera::SetDrawMode(drawMode);

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

				mOgreCamera->setPolygonMode(polyMode);
			}

			void CameraOgre::SetProjection(Projection projection)
			{
				Camera::SetProjection(projection);

				Ogre::ProjectionType ogreProjection = Ogre::PT_PERSPECTIVE;

				switch(projection)
				{
				case Perspective:
					ogreProjection = Ogre::PT_PERSPECTIVE;
					break;
				case Orthographic:
					ogreProjection = Ogre::PT_ORTHOGRAPHIC;
					break;
				}

				mOgreCamera->setProjectionType(ogreProjection);
			}

			void CameraOgre::SetOrthoDimensions(float width, float height)
			{
				Camera::SetOrthoDimensions(width, height);
				mOgreCamera->setOrthoWindow(width, height);
			}

			void CameraOgre::SetAspectRatio(float aspectRatio)
			{
				Camera::SetAspectRatio(aspectRatio);
				mOgreCamera->setAspectRatio(aspectRatio);
			}

			void CameraOgre::AddToOgreScene(Ogre::SceneManager* sceneManager)
			{
				static int cameraId = 0;
				std::stringstream cameraName;
				cameraName << "Camera_" << cameraId++;
				mOgreCamera = sceneManager->createCamera(cameraName.str().c_str());
				mOgreCamera->setNearClipDistance(0.1f);
				mOgreCamera->setFarClipDistance(1000.0f);
			}
		}
	}
}