///////////////////////////////////////////////////
// File:		GUISystem.h
// Date:		5th September 2011
// Authors:		Matt Phillips
// Description:	Graphical UI system
///////////////////////////////////////////////////

#pragma once

#include "../input/Keyboard.h"
#include "../input/Mouse.h"
#include "../input/Gamepad.h"

#include "../renderer/Renderer.h"

#include <CEGUI/CEGUI.h>

#if CEGUI_VERSION >= 8
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/RendererModules/Ogre/ImageCodec.h>
#else
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreImageCodec.h>
#endif


#include <string>

namespace ion
{
	namespace gui
	{
		//Forward declaration
		class Root;

		class GUISystem
		{
		public:
			GUISystem(renderer::Renderer& renderer);
			~GUISystem();

			bool LoadFont(std::string fontName);

			void SetActiveRoot(Root* root);

			void Update(input::Keyboard* keyboard, input::Mouse* mouse, input::Gamepad* gamepad);

		private:
			CEGUI::MouseButton IonMouseToCEMouse(input::Mouse::Buttons button);

			CEGUI::OgreRenderer* mCEGUIRenderer;
			CEGUI::OgreImageCodec* mCEGUIImageCodec;
			bool mPrevMouseButtonStates[3];
		};
	}
}