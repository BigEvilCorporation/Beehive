///////////////////////////////////////////////////
// File:		PostEffectBloom.cpp
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Bloom post effect
///////////////////////////////////////////////////

#include "PostEffectBloom.h"

#if defined ION_RENDERER_OGRE3D
#include <Ogre/OgrePass.h>
#include <Ogre/OgreTechnique.h>
#endif

namespace ion
{
	namespace renderer
	{
		#if defined ION_RENDERER_OGRE3D
		PostEffectBloomListener::PostEffectBloomListener(u32 blurHPassId, u32 blurVPassId, u32 blendPassId)
		{
			mBlurHPassId = blurHPassId;
			mBlurVPassId = blurVPassId;
			mBlendPassId = blendPassId;
			mBlendAlpha = 0.5f;
			mBlurWidth = 0.01f;
		}

		void PostEffectBloomListener::notifyMaterialSetup(u32 passId, Ogre::MaterialPtr& ogreMaterial)
		{
			Ogre::Pass* ogrePass = ogreMaterial->getTechnique(0)->getPass(0);

			if(passId == mBlendPassId)
			{
				ogrePass->setDiffuse(0.0f, 0.0f, 0.0f, mBlendAlpha);
			}
			else if(passId == mBlurHPassId || passId == mBlurVPassId)
			{
				Ogre::GpuProgramParametersSharedPtr params = ogrePass->getFragmentProgramParameters();
				params->setNamedConstant("blurWidth", mBlurWidth);
			}
		}

		void PostEffectBloomListener::notifyMaterialRender(u32 passId, Ogre::MaterialPtr& ogreMaterial)
		{
			Ogre::Pass* ogrePass = ogreMaterial->getTechnique(0)->getPass(0);

			if(passId == mBlendPassId)
			{
				ogrePass->setDiffuse(0.0f, 0.0f, 0.0f, mBlendAlpha);
			}
			else if(passId == mBlurHPassId || passId == mBlurVPassId)
			{
				Ogre::GpuProgramParametersSharedPtr params = ogrePass->getFragmentProgramParameters();
				params->setNamedConstant("blurWidth", mBlurWidth);
			}
		}
		#endif

		PostEffectBloom::PostEffectBloom()
			: PostEffect("Bloom")
		{
			mTechnique = new PostEffectTechnique(*this);
			mRenderTarget0 = new PostEffectRenderTarget(*mTechnique, "bloomRT0", 1024, 1024, PostEffectRenderTarget::A8R8G8B8);
			mRenderTarget1 = new PostEffectRenderTarget(*mTechnique, "bloomRT1", 1024, 1024, PostEffectRenderTarget::A8R8G8B8);

			mPassInput = new PostEffectPass(*mTechnique, PostEffectPass::Input);
			mPassComposition1 = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			mPassComposition2 = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			mPassOutput = new PostEffectPass(*mTechnique, PostEffectPass::Output);

			#if defined ION_RENDERER_OGRE3D
			mPostEffectListener = new PostEffectBloomListener(mPassComposition1->GetPassId(), mPassComposition2->GetPassId(), mPassOutput->GetPassId());
			mOgreCompositorListener = mPostEffectListener;
			#endif

			mPassComposition1->SetMaterial("Ogre/Compositor/Blur0");
			mPassComposition2->SetMaterial("Ogre/Compositor/Blur1");
			mPassOutput->SetMaterial("Ogre/Compositor/BloomBlend");

			mPassInput->SetOutput(*mRenderTarget1);
			mPassComposition1->SetInput(*mRenderTarget1);
			mPassComposition1->SetOutput(*mRenderTarget0);
			mPassComposition2->SetInput(*mRenderTarget0);
			mPassComposition2->SetOutput(*mRenderTarget1);
			mPassOutput->SetInput(*mRenderTarget1);
		}

		void PostEffectBloom::SetBlurWidth(float blurWidth)
		{
			#if defined ION_RENDERER_OGRE3D
			mPostEffectListener->mBlurWidth = blurWidth;
			#endif
		}

		void PostEffectBloom::SetBlendAlpha(float blendAlpha)
		{
			#if defined ION_RENDERER_OGRE3D
			mPostEffectListener->mBlendAlpha = blendAlpha;
			#endif
		}
	}
}