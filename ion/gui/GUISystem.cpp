///////////////////////////////////////////////////
// File:		GUISystem.cpp
// Date:		5th September 2011
// Authors:		Matt Phillips
// Description:	Graphical UI system
///////////////////////////////////////////////////

#include "GUISystem.h"
#include "Root.h"

#include <Ogre/OgreResourceGroupManager.h>

namespace ion
{
	namespace gui
	{
		GUISystem::GUISystem(renderer::Renderer& renderer)
		{
			//Create OGRE rendering interface
			mCEGUIRenderer = &CEGUI::OgreRenderer::create(*(Ogre::RenderTarget*)renderer.GetOgreRenderWindow());

			//Create Ogre image codec
			mCEGUIImageCodec = &mCEGUIRenderer->createOgreImageCodec();

			//Create default resource provider
			CEGUI::DefaultResourceProvider* resourceProvider = new CEGUI::DefaultResourceProvider;

			//Create CEGUI system
			CEGUI::System::create(*mCEGUIRenderer, resourceProvider, NULL, mCEGUIImageCodec, NULL, "", "engine\\gui.log");
    
			//Set resource directories
			resourceProvider->setResourceGroupDirectory("schemes", "gui/schemes/");
			resourceProvider->setResourceGroupDirectory("imagesets", "gui/imagesets/");
			resourceProvider->setResourceGroupDirectory("fonts", "gui/fonts/");
			resourceProvider->setResourceGroupDirectory("layouts", "gui/layouts/");
			resourceProvider->setResourceGroupDirectory("looknfeels", "gui/widgets/");
			resourceProvider->setResourceGroupDirectory("lua_scripts", "gui/scripts/");

			//Set resource groups as defaults
			#if CEGUI_VERSION >= 8
			CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
			#else
			CEGUI::Imageset::setDefaultResourceGroup("imagesets");
			#endif

			CEGUI::Font::setDefaultResourceGroup("fonts");
			CEGUI::Scheme::setDefaultResourceGroup("schemes");
			CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
			CEGUI::WindowManager::setDefaultResourceGroup("layouts");
			CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

			//Reset mouse state
			for(int i = 0; i < 3; i++)
			{
				mPrevMouseButtonStates[i] = false;
			}
		}

		GUISystem::~GUISystem()
		{
		}

		void GUISystem::Update(input::Keyboard* keyboard, input::Mouse* mouse, input::Gamepad* gamepad)
		{
			if(keyboard)
			{
			}

			if(mouse)
			{
				//Update mouse delta
				CEGUI::System::getSingleton().injectMouseMove((float)mouse->GetDeltaX(), (float)mouse->GetDeltaY());

				//Update buttons
				for(int i = 0; i < 3; i++)
				{
					bool currState = mouse->ButtonDown((ion::input::Mouse::Buttons)i);

					if(currState && !mPrevMouseButtonStates[i])
					{
						CEGUI::System::getSingleton().injectMouseButtonDown(IonMouseToCEMouse((ion::input::Mouse::Buttons)i));
					}
					else if(!currState && mPrevMouseButtonStates[i])
					{
						CEGUI::System::getSingleton().injectMouseButtonUp(IonMouseToCEMouse((ion::input::Mouse::Buttons)i));
					}

					mPrevMouseButtonStates[i] = currState;
				}
			}

			if(gamepad)
			{
			}
		}

		bool GUISystem::LoadFont(std::string fontName)
		{
			fontName += ".font";

			#if CEGUI_VERSION >= 8
			CEGUI::FontManager::getSingleton().createFromFile(fontName.c_str());
			#else
			CEGUI::FontManager::getSingleton().create(fontName.c_str());
			#endif

			return true;
		}

		void GUISystem::SetActiveRoot(Root* root)
		{
			CEGUI::System::getSingleton().setGUISheet(root ? root->GetCEWidget() : NULL);
		}

		CEGUI::MouseButton GUISystem::IonMouseToCEMouse(input::Mouse::Buttons button)
		{
			CEGUI::MouseButton ceButton;

			if(button == input::Mouse::LB)
				ceButton = CEGUI::LeftButton;
			else if(button == input::Mouse::MB)
				ceButton = CEGUI::MiddleButton;
			else if(button == input::Mouse::RB)
				ceButton = CEGUI::RightButton;

			return ceButton;
		}
	}
}