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

			mParams.mType = type;
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

			mParams.mDiffuse = diffuse;
		}

		void Light::SetSpecular(ColourRGB& specular)
		{
			#if defined ION_OGRE
			mOgreLight->setSpecularColour(Ogre::ColourValue(specular.r, specular.g, specular.b));
			#endif

			mParams.mSpecular = specular;
		}

		void Light::SetAttenuation(float maxDistance)
		{
			#if defined ION_OGRE
			mOgreLight->setAttenuation(maxDistance, 1.0f, 4.5f / maxDistance, 75.0f / (maxDistance * maxDistance));
			#endif

			mParams.mMaxDistance = maxDistance;
			//mParams.mConstant = constant;
			//mParams.mLinear = linear;
		}

		void Light::SetSpotRange(float minAngle, float maxAngle)
		{
			#if defined ION_OGRE
			mOgreLight->setSpotlightRange(Ogre::Degree(minAngle), Ogre::Degree(maxAngle));
			#endif

			mParams.mMinConeAngle = minAngle;
			mParams.mMaxConeAngle = maxAngle;
		}

		void Light::CastShadows(bool enabled)
		{
			#if defined ION_OGRE
			mOgreLight->setCastShadows(enabled);
			#endif

			mParams.mCastShadows = (char)enabled;
		}

		bool Light::Read(io::BinaryFile::Chunk& binaryChunk)
		{
			for(io::BinaryFile::Chunk rootChunk = binaryChunk.Begin(), rootEnd = binaryChunk.End(); rootChunk != rootEnd; rootChunk = rootChunk.Next())
			{
				if(rootChunk.GetID() == ChunkId_Params)
				{		
					//If this assert is caught, might be time to look at better serialisation techniques, since struct packing between platforms will be different
					ion::debug::Assert(rootChunk.GetDataSize() == sizeof(Params), "Light::Read() - Incorrect chunk data size");

					//Read params data
					rootChunk.ReadData(&mParams, sizeof(Params));
				}
				else if(rootChunk.GetID() == ChunkId_Position)
				{
					io::BinaryFile::ChunkT<Vector3>& posChunk = (io::BinaryFile::ChunkT<Vector3>&)rootChunk;
					Vector3 position = posChunk.ReadData();
					SetPosition(position);
				}
				else if(rootChunk.GetID() == ChunkId_Direction)
				{
					io::BinaryFile::ChunkT<Vector3>& dirChunk = (io::BinaryFile::ChunkT<Vector3>&)rootChunk;
					Vector3 direction = dirChunk.ReadData();
					SetDirection(direction);
				}
			}

			//Set all params
			SetType((Type)mParams.mType);
			SetDiffuse(mParams.mDiffuse);
			SetSpecular(mParams.mSpecular);
			//SetAttenuation(mParams.mMaxDistance, mParams.mConstant, mParams.mLinear);
			CastShadows(mParams.mCastShadows != 0);

			if(mParams.mType == Spot)
			{
				SetSpotRange(mParams.mMinConeAngle, mParams.mMaxConeAngle);
			}

			return true;
		}

		u64 Light::Write(io::BinaryFile::Chunk& binaryChunk)
		{
			//Create params chunk
			io::BinaryFile::Chunk lightParamsChunk;
			lightParamsChunk.SetID(ChunkId_Params);
			lightParamsChunk.SetData(&mParams, sizeof(Params), 1);

			//Create position chunk
			io::BinaryFile::ChunkT<Vector3> positionChunk;
			positionChunk.SetData(GetPosition());

			//Create direction chunk
			io::BinaryFile::ChunkT<Vector3> directionChunk;
			directionChunk.SetData(GetDirection());

			//Add chunks
			binaryChunk.AddChild(lightParamsChunk);
			binaryChunk.AddChild(positionChunk);
			binaryChunk.AddChild(directionChunk);

			return binaryChunk.GetChunkSize();
		}

		#if defined ION_OGRE
		Ogre::Light* Light::GetOgreLightIFace()
		{
			return mOgreLight;
		}
		#endif
	}
}