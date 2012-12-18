///////////////////////////////////////////////////
// File:		PostEffectBloom.h
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Bloom post effect
///////////////////////////////////////////////////

#pragma once

#include "PostEffect.h"

namespace ion
{
	namespace renderer
	{
		class PostEffectBloomListener : public Ogre::CompositorInstance::Listener
		{
		public:

			PostEffectBloomListener(u32 blurHPassId, u32 blurVPassId, u32 blendPassId);

			virtual void notifyMaterialSetup(u32 passId, Ogre::MaterialPtr& ogreMaterial);
			virtual void notifyMaterialRender(u32 passId, Ogre::MaterialPtr& ogreMaterial);

			float mBlurWidth;
			float mBlendAlpha;

		private:
			u32 mBlurHPassId;
			u32 mBlurVPassId;
			u32 mBlendPassId;
		};

		class PostEffectBloom : public PostEffect
		{
		public:
			PostEffectBloom();

			void SetBlurWidth(float blurWidth);
			void SetBlendAlpha(float blendAlpha);

		private:
			PostEffectBloomListener* mPostEffectListener;
		};
	}
}