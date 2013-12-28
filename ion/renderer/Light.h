///////////////////////////////////////////////////
// File:		Light.h
// Date:		23rd August 2011
// Authors:		Matt Phillips
// Description:	Spot/directional light source
///////////////////////////////////////////////////

#pragma once

#include "core/maths/Vector.h"
#include "core/Colour.h"
#include "core/Archive.h"

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

			static Light* Create();
			static void Release(Light* light);

			virtual void SetType(Type type);

			virtual void SetPosition(const Vector3& position);
			virtual void SetDirection(const Vector3& direction);

			const Vector3& GetPosition() const;
			const Vector3& GetDirection() const;

			virtual void SetDiffuse(const ColourRGB& diffuse);
			virtual void SetSpecular(const ColourRGB& specular);

			virtual void SetAttenuation(float maxDistance);
			virtual void SetSpotRange(float minAngle, float maxAngle);

			virtual void CastShadows(bool enabled);

			//Serialisation
			void Serialise(serialise::Archive& archive);
			
			static const int sSerialiseVersion;

		protected:
			Light();
			virtual ~Light();

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