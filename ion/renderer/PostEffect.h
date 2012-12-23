///////////////////////////////////////////////////
// File:		PostEffect.h
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Post effect base class
///////////////////////////////////////////////////

#pragma once

#include "Viewport.h"
#include "../core/Types.h"

#if defined ION_OGRE
#include <Ogre/OgreCompositorManager.h>
#include <Ogre/OgreCompositor.h>
#include <Ogre/OgreCompositorInstance.h>
#include <Ogre/OgreCompositionPass.h>
#include <Ogre/OgreCompositionTargetPass.h>
#include <Ogre/OgreCompositionTechnique.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class PostEffectPass;
		class PostEffectRenderTarget;
		class PostEffectTechnique;

		class PostEffect
		{
		public:
			PostEffect(const char* name);
			void AssignToViewport(Viewport& viewport);

		protected:
			std::string mName;
			Ogre::CompositorPtr mOgreCompositor;
			Ogre::CompositorInstance::Listener* mOgreCompositorListener;

			friend class PostEffectTechnique;
		};

		class PostEffectTechnique
		{
		public:
			PostEffectTechnique(PostEffect& postEffect);

		protected:
			Ogre::CompositionTechnique* mOgreTechnique;

			friend class PostEffectPass;
			friend class PostEffectRenderTarget;
		};

		class PostEffectPass
		{
		public:
			enum PassType { Input, Composition, Output };

			PostEffectPass(PostEffectTechnique& technique, PassType passType, bool initialOnly = false);

			void SetInput(PostEffectRenderTarget& input, int index = 0);
			void SetOutput(PostEffectRenderTarget& output);

			//TODO: Accept an ion::Material ptr
			void SetMaterial(const char* material);

			u32 GetPassId() const;

		protected:
			PassType mPassType;
			Ogre::CompositionPass* mOgrePass;
			Ogre::CompositionTargetPass* mOgreTargetPass;
			u32 mOgrePassId;

			static u32 sCurrPassId;
		};

		class PostEffectRenderTarget
		{
		public:
			enum PixelFormat { A8R8G8B8 };

			PostEffectRenderTarget(PostEffectTechnique& technique, const char* name, int width, int height, PixelFormat pixelFormat);
			const char* GetName() const;

		protected:
			std::string mName;
			Ogre::CompositionTechnique::TextureDefinition* mTextureDef;
		};
	}
}