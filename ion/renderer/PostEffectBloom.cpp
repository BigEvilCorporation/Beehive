///////////////////////////////////////////////////
// File:		PostEffectBloom.cpp
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Bloom post effect
///////////////////////////////////////////////////

#include "PostEffectBloom.h"

namespace ion
{
	namespace renderer
	{
		PostEffectBloom::PostEffectBloom()
			: PostEffect("Bloom")
		{
			PostEffectTechnique* mTechnique = new PostEffectTechnique(*this);
			PostEffectRenderTarget* mRenderTarget0 = new PostEffectRenderTarget(*mTechnique, "bloomRT0", 128, 128, PostEffectRenderTarget::A8R8G8B8);
			PostEffectRenderTarget* mRenderTarget1 = new PostEffectRenderTarget(*mTechnique, "bloomRT0", 128, 128, PostEffectRenderTarget::A8R8G8B8);

			PostEffectPass* mPassInput = new PostEffectPass(*mTechnique, PostEffectPass::Input);
			PostEffectPass* mPassComposition1 = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			PostEffectPass* mPassComposition2 = new PostEffectPass(*mTechnique, PostEffectPass::Composition);
			PostEffectPass* mPassOutput = new PostEffectPass(*mTechnique, PostEffectPass::Output);

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
	}
}