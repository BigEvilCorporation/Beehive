///////////////////////////////////////////////////
// File:		Light.cpp
// Date:		23rd August 2011
// Authors:		Matt Phillips
// Description:	Spot/directional light source
///////////////////////////////////////////////////

#include "Light.h"
#include "Scene.h"
#include "../core/Types.h"
#include "../core/maths/Vector.h"
#include "../core/BinaryFile.h"
#include "../core/Debug.h"

namespace ion
{
	namespace renderer
	{
		Light::Light()
		{
		}

		Light::Light(Type type, Scene& scene)
		{
			#if defined ION_OGRE
			mOgreLight = scene.GetOgreSceneMgrIFace()->createLight();
			mOgreLight->setCastShadows(true);
			#endif

			SetType(type);
			SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			SetDiffuse(ColourRGB(1.0f, 1.0f, 1.0f));
			SetSpecular(ColourRGB(1.0f, 1.0f, 1.0f));
		}

		Light::Light(Scene& scene)
		{
			#if defined ION_OGRE
			mOgreLight = scene.GetOgreSceneMgrIFace()->createLight();
			#endif

			SetType(Point);
			SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			SetDiffuse(ColourRGB(1.0f, 1.0f, 1.0f));
			SetSpecular(ColourRGB(1.0f, 1.0f, 1.0f));
		}

		Light::~Light()
		{
		}

		void Light::SetType(Type type)
		{
			#if defined ION_OGRE
			switch(type)
			{
			case Point:
				mOgreLight->setType(Ogre::Light::LT_POINT);
				break;
			case Spot:
				mOgreLight->setType(Ogre::Light::LT_SPOTLIGHT);
				break;
			case Directional:
				mOgreLight->setType(Ogre::Light::LT_DIRECTIONAL);
				break;
			};
			#endif

			mType = type;
		}

		void Light::SetPosition(const Vector3& position)
		{
			#if defined ION_OGRE
			mOgreLight->setPosition(Ogre::Vector3(position.x, position.y, position.z));
			#endif

			mPosition = position;
		}

		void Light::SetDirection(const Vector3& direction)
		{
			#if defined ION_OGRE
			mOgreLight->setDirection(direction.x, direction.y, direction.z);
			#endif

			mDirection = direction;
		}

		const Vector3& Light::GetPosition() const
		{
			return mPosition;
		}

		const Vector3& Light::GetDirection() const
		{
			return mDirection;
		}

		void Light::SetDiffuse(ColourRGB& diffuse)
		{
			#if defined ION_OGRE
			mOgreLight->setDiffuseColour(Ogre::ColourValue(diffuse.r, diffuse.g, diffuse.b));
			#endif

			mDiffuse = diffuse;
		}

		void Light::SetSpecular(ColourRGB& specular)
		{
			#if defined ION_OGRE
			mOgreLight->setSpecularColour(Ogre::ColourValue(specular.r, specular.g, specular.b));
			#endif

			mSpecular = specular;
		}

		void Light::SetAttenuation(float maxDistance)
		{
			#if defined ION_OGRE
			mOgreLight->setAttenuation(maxDistance, 1.0f, 4.5f / maxDistance, 75.0f / (maxDistance * maxDistance));
			#endif

			mMaxDistance = maxDistance;
			//mConstant = constant;
			//mLinear = linear;
		}

		void Light::SetSpotRange(float minAngle, float maxAngle)
		{
			#if defined ION_OGRE
			mOgreLight->setSpotlightRange(Ogre::Degree(minAngle), Ogre::Degree(maxAngle));
			#endif

			mMinConeAngle = minAngle;
			mMaxConeAngle = maxAngle;
		}

		void Light::CastShadows(bool enabled)
		{
			#if defined ION_OGRE
			mOgreLight->setCastShadows(enabled);
			#endif

			mCastShadows = (char)enabled;
		}

		void Light::Finalise()
		{

		}

		void Light::Serialise(serialise::Archive& archive)
		{
			archive.Serialise(mPosition);
			archive.Serialise(mDirection);
			archive.Serialise(mType);
			archive.Serialise(mDiffuse);
			archive.Serialise(mSpecular);
			archive.Serialise(mMaxDistance);
			archive.Serialise(mMinConeAngle);
			archive.Serialise(mMaxConeAngle);
			archive.Serialise(mCastShadows);
		}

		#if defined ION_OGRE
		Ogre::Light* Light::GetOgreLightIFace()
		{
			return mOgreLight;
		}
		#endif
	}
}