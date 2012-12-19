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
			mOgreCompositor = Ogre::CompositorManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mOgreCompositorListener = NULL;
		}

		void PostEffect::AssignToViewport(Viewport& viewport)
		{
			Ogre::CompositorInstance* compositorInstance = Ogre::CompositorManager::getSingleton().addCompositor(viewport.GetOgreViewportInterface(), mName.c_str());
			ion::debug::Assert(compositorInstance != NULL, "PostEffect::AssignToViewport() - Could not create compositor instance, did its materials load successfully?");
            compositorInstance->setEnabled(true);

			if(mOgreCompositorListener)
			{
				compositorInstance->addListener(mOgreCompositorListener);
			}
		}

		PostEffectTechnique::PostEffectTechnique(PostEffect& postEffect)
		{
			mOgreTechnique = postEffect.mOgreCompositor->createTechnique();
		}

		PostEffectPass::PostEffectPass(PostEffectTechnique& technique, PassType passType, bool initialOnly)
		{
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
		}

		void PostEffectPass::SetInput(PostEffectRenderTarget& input, int index)
		{
			mOgrePass->setInput(index, input.GetName());
		}

		void PostEffectPass::SetOutput(PostEffectRenderTarget& output)
		{
			mOgreTargetPass->setOutputName(output.GetName());
		}

		void PostEffectPass::SetMaterial(const char* material)
		{
			mOgrePass->setMaterialName(material);
		}

		u32 PostEffectPass::GetPassId() const
		{
			return mOgrePassId;
		}

		PostEffectRenderTarget::PostEffectRenderTarget(PostEffectTechnique& technique, const char* name, int width, int height, PixelFormat pixelFormat)
		{
			mName = name;
			mTextureDef = technique.mOgreTechnique->createTextureDefinition(name);
			mTextureDef->width = width;
            mTextureDef->height = height;

			//TODO
			Ogre::PixelFormat ogreFormat = Ogre::PF_A8R8G8B8;
			mTextureDef->formatList.push_back(ogreFormat);
		}

		const char* PostEffectRenderTarget::GetName() const
		{
			return mName.c_str();
		}
	}
}