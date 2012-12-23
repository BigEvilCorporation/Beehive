///////////////////////////////////////////////////
// File:		Light.h
// Date:		23rd August 2011
// Authors:		Matt Phillips
// Description:	Spot/directional light source
///////////////////////////////////////////////////

#pragma once

#include "../core/maths/Vector.h"
#include "../core/Colour.h"
#include "../core/BinaryFile.h"

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

			bool Read(io::BinaryFile::Chunk& binaryChunk);
			u64 Write(io::BinaryFile::Chunk& binaryChunk);

			#if defined ION_OGRE
			Ogre::Light* GetOgreLightIFace();
			#endif

		private:
			#if defined ION_OGRE
			Ogre::Light* mOgreLight;
			#endif

			enum ChunkIds
			{
				ChunkId_Params		= 10,
				ChunkId_Position	= 20,
				ChunkId_Direction	= 30
			};

			struct Params
			{
				char mType;
				ColourRGB mDiffuse;
				ColourRGB mSpecular;
				float mMaxDistance;
				float mConstant;
				float mLinear;
				float mMinConeAngle;
				float mMaxConeAngle;
				char mCastShadows;
			};

			Params mParams;
			Vector3 mPosition;
			Vector3 mDirection;
		};
	}
}