///////////////////////////////////////////////////
// File:		Light.cpp
// Date:		23rd August 2011
// Authors:		Matt Phillips
// Description:	Spot/directional light source
///////////////////////////////////////////////////

#include "core/Types.h"
#include "core/maths/Vector.h"
#include "core/Debug.h"
#include "renderer/Light.h"
#include "renderer/Scene.h"
#include "renderer/Factory.h"

namespace ion
{
	namespace renderer
	{
		Light* Light::Create()
		{
			return new LightImpl();
		}

		void Light::Release(Light* light)
		{
			delete light;
		}

		Light::Light()
		{
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
			mType = type;
		}

		void Light::SetPosition(const Vector3& position)
		{
			mPosition = position;
		}

		void Light::SetDirection(const Vector3& direction)
		{
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

		void Light::SetDiffuse(const ColourRGB& diffuse)
		{
			mDiffuse = diffuse;
		}

		void Light::SetSpecular(const ColourRGB& specular)
		{
			mSpecular = specular;
		}

		void Light::SetAttenuation(float maxDistance)
		{
			mMaxDistance = maxDistance;
		}

		void Light::SetSpotRange(float minAngle, float maxAngle)
		{
			mMinConeAngle = minAngle;
			mMaxConeAngle = maxAngle;
		}

		void Light::CastShadows(bool enabled)
		{
			mCastShadows = (char)enabled;
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
	}
}