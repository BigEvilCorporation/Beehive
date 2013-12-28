///////////////////////////////////////////////////
// File:		LightOgre.cpp
// Date:		28th November 2013
// Authors:		Matt Phillips
// Description:	Ogre3d light implementation
///////////////////////////////////////////////////

#include "LightOgre.h"

namespace ion
{
	namespace renderer
	{
		namespace ogre
		{
				LightOgre::LightOgre()
				{
					mOgreLight = NULL;
				}

				LightOgre::~LightOgre()
				{
				}

				void LightOgre::SetType(Type type)
				{
					if(mOgreLight)
					{
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
					}
				}

				void LightOgre::SetPosition(const Vector3& position)
				{
					if(mOgreLight)
					{
						mOgreLight->setPosition(Ogre::Vector3(position.x, position.y, position.z));
					}
				}

				void LightOgre::SetDirection(const Vector3& direction)
				{
					if(mOgreLight)
					{
						mOgreLight->setDirection(direction.x, direction.y, direction.z)
					}
				}

				void LightOgre::SetDiffuse(const ColourRGB& diffuse)
				{
					if(mOgreLight)
					{
						mOgreLight->setDiffuseColour(Ogre::ColourValue(diffuse.r, diffuse.g, diffuse.b));
					}
				}

				void LightOgre::SetSpecular(const ColourRGB& specular)
				{
					if(mOgreLight)
					{
						mOgreLight->setSpecularColour(Ogre::ColourValue(specular.r, specular.g, specular.b));
					}
				}

				void LightOgre::SetAttenuation(float maxDistance)
				{
					if(mOgreLight)
					{
						mOgreLight->setAttenuation(maxDistance, 1.0f, 4.5f / maxDistance, 75.0f / (maxDistance * maxDistance));
					}
				}

				void LightOgre::SetSpotRange(float minAngle, float maxAngle)
				{
					if(mOgreLight)
					{
						mOgreLight->setSpotlightRange(Ogre::Degree(minAngle), Ogre::Degree(maxAngle));
					}
				}

				void LightOgre::CastShadows(bool enabled)
				{
					if(mOgreLight)
					{
						mOgreLight->setCastShadows(enabled);
					}
				}

				void LightOgre::AddToOgreScene(Ogre::SceneManager* sceneManager)
				{
					mOgreLight = sceneManager->createLight();

					SetType((Type)mType);
					SetPosition(mPosition);
					SetDirection(mDirection);
					SetDiffuse(mDiffuse);
					SetSpecular(mSpecular);
					SetAttenuation(mMaxDistance);
					SetSpotRange(mMinConeAngle, mMaxConeAngle);
					CastShadows(mCastShadows);
				}
		}
	}
}