///////////////////////////////////////////////////
// File:		Light.h
// Date:		23rd August 2011
// Authors:		Matt Phillips
// Description:	Spot/directional light source
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"
#include "../core/Colour.h"
#include "../core/Archive.h"

#if defined ION_OGRE
#include <Ogre/OgreLight.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Scene;

		class Light
		{
		public:
			enum Type { Point, Spot, Directional };

			Light();
			Light(Scene& scene);
			Light(Type type, Scene& scene);
			~Light();

			void SetType(Type type);

			void SetPosition(const Vector3& position);
			void SetDirection(const Vector3& direction);
			const Vector3& GetPosition() const;
			const Vector3& GetDirection() const;

			void SetDiffuse(ColourRGB& diffuse);
			void SetSpecular(ColourRGB& specular);

			void SetAttenuation(float maxDistance);
			void SetSpotRange(float minAngle, float maxAngle);

			void CastShadows(bool enabled);

			void Finalise();

			//Serialisation
			void Serialise(serialise::Archive& archive);
			
			static const int sSerialiseVersion;

			#if defined ION_OGRE
			Ogre::Light* GetOgreLightIFace();
			#endif

		private:
			#if defined ION_OGRE
			Ogre::Light* mOgreLight;
			#endif

			Vector3 mPosition;
			Vector3 mDirection;

			char mType;
			ColourRGB mDiffuse;
			ColourRGB mSpecular;
			float mMaxDistance;
			float mMinConeAngle;
			float mMaxConeAngle;
			char mCastShadows;
		};
	}
}