/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/
 
 Copyright (c) 2000-2009 Torus Knot Software Ltd
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef __SdkSample_H__
#define __SdkSample_H__

#include "Sample.h"
#include "SdkTrays.h"
#include "SdkCameraMan.h"

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

namespace OgreBites
{
	/*=============================================================================
	// Base SDK sample class. Includes default player camera and SDK trays.
	=============================================================================*/
	class SdkSample : public Sample, public SdkTrayListener
    {
    public:

		SdkSample()
		{
			// so we don't have to worry about checking if these keys exist later
			mInfo["Title"] = "Untitled";
			mInfo["Description"] = "";
			mInfo["Category"] = "Unsorted";
			mInfo["Thumbnail"] = "";
			mInfo["Help"] = "";

			mTrayMgr = 0;
			mCameraMan = 0;
			mCamera = 0;
			mViewport = 0;
			mDetailsPanel = 0;
			mCursorWasVisible = false;
			mDragLook = false;
		}

		virtual ~SdkSample() {}

		/*-----------------------------------------------------------------------------
		| Manually update the cursor position after being unpaused.
		-----------------------------------------------------------------------------*/
		virtual void unpaused()
		{
			mTrayMgr->refreshCursor();
		}

		/*-----------------------------------------------------------------------------
		| Automatically saves position and orientation for free-look cameras.
		-----------------------------------------------------------------------------*/
		virtual void saveState(Ogre::NameValuePairList& state)
		{
			if (mCameraMan->getStyle() == CS_FREELOOK)
			{
				state["CameraPosition"] = Ogre::StringConverter::toString(mCamera->getPosition());
				state["CameraOrientation"] = Ogre::StringConverter::toString(mCamera->getOrientation());
			}
		}

		/*-----------------------------------------------------------------------------
		| Automatically restores position and orientation for free-look cameras.
		-----------------------------------------------------------------------------*/
		virtual void restoreState(Ogre::NameValuePairList& state)
		{
			if (state.find("CameraPosition") != state.end() && state.find("CameraOrientation") != state.end())
			{
				mCameraMan->setStyle(CS_FREELOOK);
				mCamera->setPosition(Ogre::StringConverter::parseVector3(state["CameraPosition"]));
				mCamera->setOrientation(Ogre::StringConverter::parseQuaternion(state["CameraOrientation"]));
			}
		}

		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
		{
			mTrayMgr->frameRenderingQueued(evt);

			if (!mTrayMgr->isDialogVisible())
			{
				mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera

				if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
				{
					mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
					mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
					mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
					mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
					mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
					mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
					mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));

#ifdef USE_RTSHADER_SYSTEM
					mDetailsPanel->setParamValue(14, Ogre::StringConverter::toString(mShaderGenerator->getVertexShaderCount()));
					mDetailsPanel->setParamValue(15, Ogre::StringConverter::toString(mShaderGenerator->getFragmentShaderCount()));		
#endif
				}	
			}

			return true;
		}

		virtual void windowResized(Ogre::RenderWindow* rw)
		{
			mCamera->setAspectRatio((Ogre::Real)mViewport->getActualWidth() / (Ogre::Real)mViewport->getActualHeight());
		}

		virtual bool keyPressed(const OIS::KeyEvent& evt)
		{
			if (evt.key == OIS::KC_H || evt.key == OIS::KC_F1)   // toggle visibility of help dialog
			{
				if (!mTrayMgr->isDialogVisible() && mInfo["Help"] != "") mTrayMgr->showOkDialog("Help", mInfo["Help"]);
				else mTrayMgr->closeDialog();
			}

			if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

			if (evt.key == OIS::KC_F)   // toggle visibility of advanced frame stats
			{
				mTrayMgr->toggleAdvancedFrameStats();
			}
			else if (evt.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
			{
				if (mDetailsPanel->getTrayLocation() == TL_NONE)
				{
					mTrayMgr->moveWidgetToTray(mDetailsPanel, TL_TOPRIGHT, 0);
					mDetailsPanel->show();
				}
				else
				{
					mTrayMgr->removeWidgetFromTray(mDetailsPanel);
					mDetailsPanel->hide();
				}
			}
			else if (evt.key == OIS::KC_T)   // cycle polygon rendering mode
			{
				Ogre::String newVal;
				Ogre::TextureFilterOptions tfo;
				unsigned int aniso;

				switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
				{
				case 'B':
					newVal = "Trilinear";
					tfo = Ogre::TFO_TRILINEAR;
					aniso = 1;
					break;
				case 'T':
					newVal = "Anisotropic";
					tfo = Ogre::TFO_ANISOTROPIC;
					aniso = 8;
					break;
				case 'A':
					newVal = "None";
					tfo = Ogre::TFO_NONE;
					aniso = 1;
					break;
				default:
					newVal = "Bilinear";
					tfo = Ogre::TFO_BILINEAR;
					aniso = 1;
				}

				Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
				Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
				mDetailsPanel->setParamValue(9, newVal);
			}
			else if (evt.key == OIS::KC_R)   // cycle polygon rendering mode
			{
				Ogre::String newVal;
				Ogre::PolygonMode pm;

				switch (mCamera->getPolygonMode())
				{
				case Ogre::PM_SOLID:
					newVal = "Wireframe";
					pm = Ogre::PM_WIREFRAME;
					break;
				case Ogre::PM_WIREFRAME:
					newVal = "Points";
					pm = Ogre::PM_POINTS;
					break;
				default:
					newVal = "Solid";
					pm = Ogre::PM_SOLID;
				}

				mCamera->setPolygonMode(pm);
				mDetailsPanel->setParamValue(10, newVal);
			}
			else if(evt.key == OIS::KC_F5)   // refresh all textures
			{
				Ogre::TextureManager::getSingleton().reloadAll();
			}
			else if (evt.key == OIS::KC_SYSRQ)   // take a screenshot
			{
				mWindow->writeContentsToTimestampedFile("screenshot", ".png");
			}

#ifdef USE_RTSHADER_SYSTEM		
			// Toggle schemes.			
			else if (evt.key == OIS::KC_F2)
			{	
				Ogre::Viewport* mainVP = mCamera->getViewport();
				const Ogre::String& curMaterialScheme = mainVP->getMaterialScheme();

				if (curMaterialScheme == Ogre::MaterialManager::DEFAULT_SCHEME_NAME)
				{							
					mainVP->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
					mDetailsPanel->setParamValue(11, "On");
				}
				else if (curMaterialScheme == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
				{
					mainVP->setMaterialScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
					mDetailsPanel->setParamValue(11, "Off");
				}														
			}			

#ifdef RTSHADER_SYSTEM_BUILD_EXT_SHADERS

			// Toggles per pixel per light model.
			else if (evt.key == OIS::KC_F3)
			{
				static bool usePerPixelLighting = true;					
								
				// Grab the scheme render state.												
				Ogre::RTShader::RenderState* schemRenderState = mShaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

				
				// Add per pixel lighting sub render state to the global scheme render state.
				// It will override the default FFP lighting sub render state.
				if (usePerPixelLighting)
				{
					Ogre::RTShader::SubRenderState* perPixelLightModel = mShaderGenerator->createSubRenderState(Ogre::RTShader::PerPixelLighting::Type);
					
					schemRenderState->addTemplateSubRenderState(perPixelLightModel);					
				}

				// Search the per pixel sub render state and remove it.
				else
				{
					const Ogre::RTShader::SubRenderStateList& subRenderStateList = schemRenderState->getTemplateSubRenderStateList();
					Ogre::RTShader::SubRenderStateListConstIterator it = subRenderStateList.begin();
					Ogre::RTShader::SubRenderStateListConstIterator itEnd = subRenderStateList.end();
						
					for (; it != itEnd; ++it)
					{
						Ogre::RTShader::SubRenderState* curSubRenderState = *it;

						// This is the per pixel sub render state -> remove it.
						if (curSubRenderState->getType() == Ogre::RTShader::PerPixelLighting::Type)
						{
							schemRenderState->removeTemplateSubRenderState(*it);
							break;
						}
					}
				}

				// Invalidate the scheme in order to re-generate all shaders based technique related to this scheme.
				mShaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);


				// Update UI.
				if (usePerPixelLighting)
					mDetailsPanel->setParamValue(12, "Pixel");
				else
					mDetailsPanel->setParamValue(12, "Vertex");
				usePerPixelLighting = !usePerPixelLighting;				
			}	
#endif

			// Switch vertex shader outputs compaction policy.
			else if (evt.key == OIS::KC_F4)   
			{
				switch (mShaderGenerator->getVertexShaderOutputsCompactPolicy())
				{
				case Ogre::RTShader::VSOCP_LOW:
					mShaderGenerator->setVertexShaderOutputsCompactPolicy(Ogre::RTShader::VSOCP_MEDIUM);
					mDetailsPanel->setParamValue(13, "Medium");
					break;

				case Ogre::RTShader::VSOCP_MEDIUM:
					mShaderGenerator->setVertexShaderOutputsCompactPolicy(Ogre::RTShader::VSOCP_HIGH);
					mDetailsPanel->setParamValue(13, "High");
					break;

				case Ogre::RTShader::VSOCP_HIGH:
					mShaderGenerator->setVertexShaderOutputsCompactPolicy(Ogre::RTShader::VSOCP_LOW);
					mDetailsPanel->setParamValue(13, "Low");
					break;
				}
				
				// Invalidate the scheme in order to re-generate all shaders based technique related to this scheme.
				mShaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
			}	
#endif

			mCameraMan->injectKeyDown(evt);
			return true;
		}

		virtual bool keyReleased(const OIS::KeyEvent& evt)
		{
			mCameraMan->injectKeyUp(evt);

			return true;
		}

		/* IMPORTANT: When overriding these following handlers, remember to allow the tray manager
		to filter out any interface-related mouse events before processing them in your scene.
		If the tray manager handler returns true, the event was meant for the trays, not you. */

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
		virtual bool touchMoved(const OIS::MultiTouchEvent& evt)
#else
		virtual bool mouseMoved(const OIS::MouseEvent& evt)
#endif
		{
			if (mTrayMgr->injectMouseMove(evt)) return true;

			mCameraMan->injectMouseMove(evt);

			return true;
		}

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
		virtual bool touchPressed(const OIS::MultiTouchEvent& evt)
		{
			if (mTrayMgr->injectMouseDown(evt)) return true;
            
			if (mDragLook)
			{
				mCameraMan->setStyle(CS_FREELOOK);
				mTrayMgr->hideCursor();
			}
				
			mCameraMan->injectMouseDown(evt);
            
			return true;
		}
#else
		virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
		{
			if (mTrayMgr->injectMouseDown(evt, id)) return true;
            
			if (mDragLook && id == OIS::MB_Left)
			{
				mCameraMan->setStyle(CS_FREELOOK);
				mTrayMgr->hideCursor();
			}

			mCameraMan->injectMouseDown(evt, id);

			return true;
		}
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
		virtual bool touchReleased(const OIS::MultiTouchEvent& evt)
		{
			if (mTrayMgr->injectMouseUp(evt)) return true;
            
			if (mDragLook)
			{
				mCameraMan->setStyle(CS_MANUAL);
				mTrayMgr->showCursor();
			}
            
			mCameraMan->injectMouseUp(evt);
            
			return true;
		}
#else
		virtual bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
		{
			if (mTrayMgr->injectMouseUp(evt, id)) return true;
            
			if (mDragLook && id == OIS::MB_Left)
			{
				mCameraMan->setStyle(CS_MANUAL);
				mTrayMgr->showCursor();
			}

			mCameraMan->injectMouseUp(evt, id);

			return true;
		}
#endif

		/*-----------------------------------------------------------------------------
		| Extended to setup a default tray interface and camera controller.
		-----------------------------------------------------------------------------*/
#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
		virtual void _setup(Ogre::RenderWindow* window, OIS::MultiTouch* mouse, FileSystemLayer* fsLayer)
#else
		virtual void _setup(Ogre::RenderWindow* window, OIS::Keyboard* keyboard, OIS::Mouse* mouse, FileSystemLayer* fsLayer)
#endif
		{
			// assign mRoot here in case Root was initialised after the Sample's constructor ran.
			mRoot = Ogre::Root::getSingletonPtr();
			mWindow = window;
#if OGRE_PLATFORM != OGRE_PLATFORM_IPHONE
			mKeyboard = keyboard;
#endif
			mMouse = mouse;
			mFSLayer = fsLayer;

			locateResources();
			createSceneManager();
			setupView();

			mTrayMgr = new SdkTrayManager("SampleControls", window, mouse, this);  // create a tray interface
			
#ifdef USE_RTSHADER_SYSTEM
			// Initialize shader generator.
			// Must be before resource loading in order to allow parsing extended material attributes.
			bool success = initializeRTShaderSystem(mSceneMgr);
			if (!success) 
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, 
					"Shader Generator Initialization failed - Core shader libs path not found", 
					"SdkSample::_setup");
			}														
#endif
			
			loadResources();
			mResourcesLoaded = true;

			// show stats and logo and hide the cursor
			mTrayMgr->showFrameStats(TL_BOTTOMLEFT);
			mTrayMgr->showLogo(TL_BOTTOMRIGHT);
			mTrayMgr->hideCursor();

			// create a params panel for displaying sample details
			Ogre::StringVector items;
			items.push_back("cam.pX");
			items.push_back("cam.pY");
			items.push_back("cam.pZ");
			items.push_back("");
			items.push_back("cam.oW");
			items.push_back("cam.oX");
			items.push_back("cam.oY");
			items.push_back("cam.oZ");
			items.push_back("");
			items.push_back("Filtering");
			items.push_back("Poly Mode");

#ifdef USE_RTSHADER_SYSTEM
			items.push_back("RT Shaders");
			items.push_back("Lighting Model");
			items.push_back("Compact Policy");
			items.push_back("Generated VS");
			items.push_back("Generated FS");														
#endif

			mDetailsPanel = mTrayMgr->createParamsPanel(TL_NONE, "DetailsPanel", 200, items);
			mDetailsPanel->hide();

			mDetailsPanel->setParamValue(9, "Bilinear");
			mDetailsPanel->setParamValue(10, "Solid");

#ifdef USE_RTSHADER_SYSTEM
			mDetailsPanel->setParamValue(11, "Off");
			mDetailsPanel->setParamValue(12, "Vertex");
			mDetailsPanel->setParamValue(13, "Low");
			mDetailsPanel->setParamValue(14, "0");
			mDetailsPanel->setParamValue(15, "0");															
#endif

			setupContent();
			mContentSetup = true;

			mDone = false;
		}

		virtual void _shutdown()
		{
			Sample::_shutdown();

			if (mTrayMgr) delete mTrayMgr;
			if (mCameraMan) delete mCameraMan;

			// restore settings we may have changed, so as not to affect other samples
			Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
			Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(1);
		}

    protected:

		virtual void setupView()
		{
			// setup default viewport layout and camera
			mCamera = mSceneMgr->createCamera("MainCamera");
			mViewport = mWindow->addViewport(mCamera);
			mCamera->setAspectRatio((Ogre::Real)mViewport->getActualWidth() / (Ogre::Real)mViewport->getActualHeight());
			mCamera->setNearClipDistance(5);

			mCameraMan = new SdkCameraMan(mCamera);   // create a default camera controller
		}

		virtual void setDragLook(bool enabled)
		{
			if (enabled)
			{
				mCameraMan->setStyle(CS_MANUAL);
				mTrayMgr->showCursor();
				mDragLook = true;
			}
			else
			{
				mCameraMan->setStyle(CS_FREELOOK);
				mTrayMgr->hideCursor();
				mDragLook = false;
			}
		}

		Ogre::Viewport* mViewport;    		// main viewport
		Ogre::Camera* mCamera;        		// main camera
		SdkTrayManager* mTrayMgr;     		// tray interface manager
		SdkCameraMan* mCameraMan;     		// basic camera controller
		ParamsPanel* mDetailsPanel;   		// sample details panel
		bool mCursorWasVisible;				// was cursor visible before dialog appeared
		bool mDragLook;                     // click and drag to free-look
    };
}

#endif
