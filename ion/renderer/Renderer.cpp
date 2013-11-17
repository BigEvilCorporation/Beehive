///////////////////////////////////////////////////
// File:		Renderer.cpp
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Ogre3D renderer
///////////////////////////////////////////////////

#include "Renderer.h"
#include "../core/Debug.h"

#include <sstream>

#if defined ION_OGRE
#include <Ogre/OgreWindowEventUtilities.h>
#include <Ogre/RenderSystems/Direct3D9/OgreD3D9RenderSystem.h>
#endif

namespace ion
{
	namespace renderer
	{
		Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight, bool fullscreen, int fsaaLevel)
		{
			#if defined ION_OGRE

			//Create Ogre root system
			mOgreRoot = new Ogre::Root("", "engine\\config.cfg", "engine\\renderer.log");

			//Create Direct3d9 renderer
			Ogre::D3D9RenderSystem* renderer = new Ogre::D3D9RenderSystem(GetModuleHandle(NULL));

			//Set renderer
			mOgreRoot->setRenderSystem(renderer);

			//Load NVIDIA Cg plugin
			mOgreRoot->loadPlugin("Plugin_CgProgramManager_d");

			//Initialise Ogre root
			mOgreRoot->initialise(false);

			//Create FSAA params
			Ogre::NameValuePairList windowCreationParams;
			char fsaaLevelText[8] = {0};
			sprintf_s(fsaaLevelText, 8, "%i", fsaaLevel);
			windowCreationParams["FSAA"] = fsaaLevelText;

			//Create render window
			mOgreWindow = mOgreRoot->createRenderWindow(windowTitle, windowWidth, windowHeight, fullscreen, &windowCreationParams);

			//Set active
			mOgreWindow->setActive(true);

			//All rendering handled manually
			mOgreWindow->setAutoUpdated(false);

			//Clear events gathered during initialisation
			mOgreRoot->clearEventTimes();

			//Get window handle
			mOgreWindow->getCustomAttribute("WINDOW", &mWindowHandle);

			//Set Ogre resource manager locations
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation("materials\\ogre", "FileSystem");
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation("shaders\\ogre", "FileSystem");
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation("textures\\ogre", "FileSystem");
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

			#endif
		}

		Renderer::~Renderer()
		{
			#if defined ION_OGRE
			if(mOgreRoot)
			{
				delete mOgreRoot;
			}
			#endif
		}

		void Renderer::SetWindowTitle(const char* title)
		{
			#if defined ION_OGRE
			SetWindowText(mWindowHandle, title);
			#endif
		}

		#if defined ION_OGRE
		HWND Renderer::GetWindowHandle()
		{
			return mWindowHandle;
		}
		#endif

		void Renderer::Update(float deltaTime)
		{
			#if defined ION_OGRE
			if(mOgreRoot)
			{
				//Update window
				mOgreWindow->update(false);

				mOgreRoot->_fireFrameRenderingQueued();
				
				//Render a frame
				mOgreRoot->renderOneFrame(deltaTime);

				//Swap buffers
				mOgreWindow->swapBuffers(true);

				//Pump window messages
				Ogre::WindowEventUtilities::messagePump();
			}
			#endif
		}

		#if defined ION_OGRE
		Ogre::Root* Renderer::GetOgreRoot()
		{
			return mOgreRoot;
		}

		Ogre::RenderWindow* Renderer::GetOgreRenderWindow()
		{
			return mOgreWindow;
		}
		#endif
	}
}