///////////////////////////////////////////////////
// File:		PostEffectMotionBlur.cpp
// Date:		18th December 2012
// Authors:		Matt Phillips
// Description:	Motion blur post effect
///////////////////////////////////////////////////

#include "PostEffectMotionBlur.h"

#if defined ION_RENDERER_OGRE3D
#include <Ogre/OgrePass.h>
#include <Ogre/OgreTechnique.h>
#endif

namespace ion
{
	namespace renderer
	{
		#if defined ION_RENDERER_OGRE3D
		PostEffectMotionBlurListener::PostEffectMotionBlurListener(u32 blurPassId)
		{
			mBlurPassId = blurPassId;
			mBlurWeight = 0.5f;
		}

		void PostEffectMotionBlurListener::notifyMaterialSetup(u32 passId, Ogre::MaterialPtr& ogreMaterial)
		{
			Ogre::Pass* ogrePass = ogreMaterial->getTechnique(0)->getPass(0);

			if(passId == mBlurPassId)
			{
				Ogre::GpuProgramParametersSharedPtr params = ogrePass->getFragmentProgramParameters();
				params->setNamedConstant("blur", mBlurWeight);
			}
		}

		void PostEffectMotionBlurListener::notifyMaterialRender(u32 passId, Ogre::MaterialPtr& ogreMaterial)
		{
			Ogre::Pass* ogrePass = ogreMaterial->getTechnique(0)->getPass(0);

			if(passId == mBlurPassId)
			{
				Ogre::GpuProgramParametersSharedPtr params = ogrePass->getFragmentProgramParameters();
				params->setNamedConstant("blur", mBlurWeight);
			}
		}
		#endif

		PostEffectMotionBlur::PostEffectMotionBlur()
			: PostEffect("Motion Blur")
		{
			PostEffectTechnique* mTechnique = new PostEffectTechnique(*this);
			PostEffectRenderTarget* mRenderTargetScene = new PostEffectRenderTarget(*mTechnique, "blurRTScene", 0, 0, PostEffectRenderTarget::A8R8G8B8);
			PostEffectRenderTarget* mRenderTargetSum = new PostEffectRenderTarget(*mTechnique, "blurRTSum", 0, 0, PostEffectRenderTarget::A8R8G8B8);
			PostEffectRenderTarget* mRenderTargetTemp = new PostEffectRenderTarget(*mTechnique, "blurRTTemp", 0, 0, PostEffectRenderTarget::A8R8G8B8);

			PostEffectPass* mPassInput = new PostEffectPass(*mTechnique, PostEffectPass::Input);
			PostEffectPass* mPassSum = new PostEffectPass(*mTechnique, PostEffectPass::Input, true);
			PostEffectPass* mPassBlur = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			PostEffectPass* mPassCopy = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			PostEffectPass* mPassOutput = new PostEffectPass(*mTechnique, PostEffectPass::Output);

			#if defined ION_RENDERER_OGRE3D
			mPostEffectListener = new PostEffectMotionBlurListener(mPassBlur->GetPassId());
			mOgreCompositorListener = mPostEffectListener;
			#endif

			mPassBlur->SetMaterial("Ogre/Compositor/Combine");
			mPassCopy->SetMaterial("Ogre/Compositor/Copyback");
			mPassOutput->SetMaterial("Ogre/Compositor/MotionBlur");

			mPassInput->SetOutput(*mRenderTargetScene);
			mPassSum->SetOutput(*mRenderTargetSum);
			mPassBlur->SetInput(*mRenderTargetScene, 0);
			mPassBlur->SetInput(*mRenderTargetSum, 1);
			mPassBlur->SetOutput(*mRenderTargetTemp);
			mPassCopy->SetInput(*mRenderTargetTemp);
			mPassCopy->SetOutput(*mRenderTargetSum);
			mPassOutput->SetInput(*mRenderTargetSum);
		}

		void PostEffectMotionBlur::SetBlurWeight(float weight)
		{
			#if defined ION_RENDERER_OGRE3D
			mPostEffectListener->mBlurWeight = weight;
			#endif
		}
	}
}