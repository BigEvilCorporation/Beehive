///////////////////////////////////////////////////
// File:		Renderer.cpp
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Ogre3D renderer
///////////////////////////////////////////////////

#include "Renderer.h"
#include "../core/Debug.h"

#include <sstream>
#include <Ogre/OgreWindowEventUtilities.h>
#include <Ogre/RenderSystems/Direct3D9/OgreD3D9RenderSystem.h>

namespace ion
{
	namespace renderer
	{
		Renderer::Renderer(const char* windowTitle, int windowWidth, int windowHeight, bool fullscreen)
		{
			//Create Ogre root system
			mOgreRoot = new Ogre::Root("", "engine\\config.cfg", "engine\\renderer.log");

			//Create Direct3d9 renderer
			Ogre::D3D9RenderSystem* renderer = new Ogre::D3D9RenderSystem(GetModuleHandle(NULL));

			//Set renderer
			mOgreRoot->setRenderSystem(renderer);

			//Initialise Ogre root
			mOgreRoot->initialise(false);

			//Create render window
			mOgreWindow = mOgreRoot->createRenderWindow(windowTitle, windowWidth, windowHeight, fullscreen);

			//Set active
			mOgreWindow->setActive(true);

			//All rendering handled manually
			mOgreWindow->setAutoUpdated(false);

			//Clear events gathered during initialisation
			mOgreRoot->clearEventTimes();

			//Get window handle
			mOgreWindow->getCustomAttribute("WINDOW", &mWindowHandle);
		}

		Renderer::~Renderer()
		{
			if(mOgreRoot)
			{
				delete mOgreRoot;
			}
		}

		void Renderer::SetWindowTitle(const char* title)
		{
			SetWindowText(mWindowHandle, title);
		}

		HWND Renderer::GetWindowHandle()
		{
			return mWindowHandle;
		}

		void Renderer::Update(float deltaTime)
		{
			if(mOgreRoot)
			{
				//Update window
				mOgreWindow->update(false);
				
				//Render a frame
				mOgreRoot->renderOneFrame(deltaTime);

				//Swap buffers
				mOgreWindow->swapBuffers(true);

				//Pump window messages
				Ogre::WindowEventUtilities::messagePump();
			}
		}

		Ogre::Root* Renderer::GetOgreRoot()
		{
			return mOgreRoot;
		}

		Ogre::RenderWindow* Renderer::GetOgreRenderWindow()
		{
			return mOgreWindow;
		}
	}
}