///////////////////////////////////////////////////
// File:		PostEffect.h
// Date:		18th November 2012
// Authors:		Matt Phillips
// Description:	Post effect base class
///////////////////////////////////////////////////

#pragma once

#include "Viewport.h"

#if !defined ION_PLUGIN
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

			PostEffectPass(PostEffectTechnique& technique, PassType passType);

			void SetInput(PostEffectRenderTarget& input);
			void SetOutput(PostEffectRenderTarget& output);

			//TODO: Accept an ion::Material ptr
			void SetMaterial(const char* material);

		protected:
			PassType mPassType;
			Ogre::CompositionPass* mOgrePass;
			Ogre::CompositionTargetPass* mOgreTargetPass;
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