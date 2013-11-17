///////////////////////////////////////////////////
// File:		PostEffect.cpp
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Post effect base class
///////////////////////////////////////////////////

#include "PostEffect.h"
#include "../core/Debug.h"

namespace ion
{
	namespace renderer
	{
		u32 PostEffectPass::sCurrPassId = 0;

		PostEffect::PostEffect(const char* name)
		{
			mName = name;

			#if defined ION_OGRE
			mOgreCompositor = Ogre::CompositorManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mOgreCompositorListener = NULL;
			#endif
		}

		void PostEffect::AssignToViewport(Viewport& viewport)
		{
			#if defined ION_OGRE
			Ogre::CompositorInstance* compositorInstance = Ogre::CompositorManager::getSingleton().addCompositor(viewport.GetOgreViewportInterface(), mName.c_str());
			ion::debug::Assert(compositorInstance != NULL, "PostEffect::AssignToViewport() - Could not create compositor instance, did its materials load successfully?");
			compositorInstance->setEnabled(true);

			if(mOgreCompositorListener)
			{
				compositorInstance->addListener(mOgreCompositorListener);
			}
			#endif
		}

		PostEffectTechnique::PostEffectTechnique(PostEffect& postEffect)
		{
			#if defined ION_OGRE
			mOgreTechnique = postEffect.mOgreCompositor->createTechnique();
			#endif
		}

		PostEffectPass::PostEffectPass(PostEffectTechnique& technique, PassType passType, bool initialOnly)
		{
			#if defined ION_OGRE
			switch(passType)
			{
			case Input:
				mOgreTargetPass = technique.mOgreTechnique->createTargetPass();
				mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				mOgreTargetPass->setOnlyInitial(initialOnly);
				mOgrePassId = 0;
				break;

			case Composition:
				mOgreTargetPass = technique.mOgreTechnique->createTargetPass();
				mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				mOgrePass = mOgreTargetPass->createPass();
				mOgrePass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				mOgrePassId = sCurrPassId++;
				mOgrePass->setIdentifier(mOgrePassId);
				break;

			case Output:
				mOgreTargetPass = technique.mOgreTechnique->getOutputTargetPass();
				mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				mOgrePass = mOgreTargetPass->createPass();
				mOgrePass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				mOgrePassId = sCurrPassId++;
				mOgrePass->setIdentifier(mOgrePassId);
				break;
			};
			#endif
		}

		void PostEffectPass::SetInput(PostEffectRenderTarget& input, int index)
		{
			#if defined ION_OGRE
			mOgrePass->setInput(index, input.GetName());
			#endif
		}

		void PostEffectPass::SetOutput(PostEffectRenderTarget& output)
		{
			#if defined ION_OGRE
			mOgreTargetPass->setOutputName(output.GetName());
			#endif
		}

		void PostEffectPass::SetMaterial(const char* material)
		{
			#if defined ION_OGRE
			mOgrePass->setMaterialName(material);
			#endif
		}

		u32 PostEffectPass::GetPassId() const
		{
			#if defined ION_OGRE
			return mOgrePassId;
			#else
			return 0;
			#endif
		}

		PostEffectRenderTarget::PostEffectRenderTarget(PostEffectTechnique& technique, const char* name, int width, int height, PixelFormat pixelFormat)
		{
			mName = name;

			#if defined ION_OGRE
			mTextureDef = technique.mOgreTechnique->createTextureDefinition(name);
			mTextureDef->width = width;
			mTextureDef->height = height;

			//TODO
			Ogre::PixelFormat ogreFormat = Ogre::PF_A8R8G8B8;
			mTextureDef->formatList.push_back(ogreFormat);
			#endif
		}

		const char* PostEffectRenderTarget::GetName() const
		{
			return mName.c_str();
		}
	}
}