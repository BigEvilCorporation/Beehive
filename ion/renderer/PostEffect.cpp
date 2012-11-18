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
		PostEffect::PostEffect(const char* name)
		{
			mName = name;
			mOgreCompositor = Ogre::CompositorManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}

		void PostEffect::AssignToViewport(Viewport& viewport)
		{
			Ogre::CompositorInstance* compositorInstance = Ogre::CompositorManager::getSingleton().addCompositor(viewport.GetOgreViewportInterface(), mName.c_str());
			ion::debug::Assert(compositorInstance != NULL, "PostEffect::AssignToViewport() - Could not create compositor instance, did its materials load successfully?");
            compositorInstance->setEnabled(true);
		}

		PostEffectTechnique::PostEffectTechnique(PostEffect& postEffect)
		{
			mOgreTechnique = postEffect.mOgreCompositor->createTechnique();
		}

		PostEffectPass::PostEffectPass(PostEffectTechnique& technique, PassType passType)
		{
			switch(passType)
			{
			case Input:
				mOgreTargetPass = technique.mOgreTechnique->createTargetPass();
				mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				break;

			case Composition:
				mOgreTargetPass = technique.mOgreTechnique->createTargetPass();
				mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				mOgrePass = mOgreTargetPass->createPass();
				mOgrePass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				break;

			case Output:
				mOgreTargetPass = technique.mOgreTechnique->getOutputTargetPass();
                mOgreTargetPass->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
                mOgrePass = mOgreTargetPass->createPass();
				mOgrePass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				break;
			};
		}

		void PostEffectPass::SetInput(PostEffectRenderTarget& input)
		{
			mOgrePass->setInput(0, input.GetName());
		}

		void PostEffectPass::SetOutput(PostEffectRenderTarget& output)
		{
			mOgreTargetPass->setOutputName(output.GetName());
		}

		void PostEffectPass::SetMaterial(const char* material)
		{
			mOgrePass->setMaterialName(material);
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