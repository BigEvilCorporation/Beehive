///////////////////////////////////////////////////
// File:		CameraOgre.h
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d camera implementation
///////////////////////////////////////////////////

#pragma once

#include <Ogre/OgreRoot.h>
#include <Ogre/OgreCamera.h>

#include "renderer/Camera.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
			class CameraOgre : public Camera
			{
			public:
				CameraOgre();
				~CameraOgre();

				virtual void SetDrawMode(DrawMode drawMode);
				virtual void SetProjection(Projection projection);
				virtual void SetOrthoDimensions(float width, float height);
				virtual void SetAspectRatio(float aspectRatio);

				void AddToOgreScene(Ogre::SceneManager* sceneManager);

				Ogre::Camera* mOgreCamera;
			};
		}

		typedef ogre::CameraOgre CameraImpl;
	}
}