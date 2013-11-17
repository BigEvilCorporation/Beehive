///////////////////////////////////////////////////
// File:		Renderer.h
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Ogre3D renderer
///////////////////////////////////////////////////

#pragma once

#if defined ION_OGRE
#include <Ogre/OgreRoot.h>
#include <Ogre/OgreRenderWindow.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Viewport;

		class Renderer
		{
		public:
			Renderer(const char* windowTitle, int windowWidth, int windowHeight, bool fullscreen, int fsaaLevel);
			~Renderer();

			void AddViewport(Viewport& viewport);

			void Update(float deltaTime);

			void SetWindowTitle(const char* title);

			#if defined ION_OGRE
			Ogre::Root* GetOgreRoot();
			Ogre::RenderWindow* GetOgreRenderWindow();
			HWND GetWindowHandle();
			#endif

		private:
			#if defined ION_OGRE
			Ogre::Root* mOgreRoot;
			Ogre::RenderWindow* mOgreWindow;
			HWND mWindowHandle;
			#endif
		};
	}
}