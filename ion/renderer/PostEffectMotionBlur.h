///////////////////////////////////////////////////
// File:		PostEffectMotionBlur.h
// Date:		18th December 2012
// Authors:		Matt Phillips
// Description:	Motion blur post effect
///////////////////////////////////////////////////

#pragma once

#include "PostEffect.h"

namespace ion
{
	namespace renderer
	{
		class PostEffectMotionBlurListener : public Ogre::CompositorInstance::Listener
		{
		public:
			PostEffectMotionBlurListener(u32 blurPassId);

			virtual void notifyMaterialSetup(u32 passId, Ogre::MaterialPtr& ogreMaterial);
			virtual void notifyMaterialRender(u32 passId, Ogre::MaterialPtr& ogreMaterial);

			float mBlurWeight;

		private:
			u32 mBlurPassId;
		};

		class PostEffectMotionBlur : public PostEffect
		{
		public:
			PostEffectMotionBlur();

			void SetBlurWeight(float weight);

		private:
			PostEffectMotionBlurListener* mPostEffectListener;
		};
	}
}