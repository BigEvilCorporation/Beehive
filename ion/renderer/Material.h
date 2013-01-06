///////////////////////////////////////////////////
// File:		Material.h
// Date:		13th December 2011
// Authors:		Matt Phillips
// Description:	Material class and file loader
///////////////////////////////////////////////////

#pragma once

#include "../core/Types.h"
#include "../core/Colour.h"
#include "../core/BinaryFile.h"

#include <string>
#include <vector>

#if defined ION_OGRE
#include <Ogre/OgreMaterial.h>
#include <Ogre/OgreMaterialManager.h>
#include <Ogre/OgreTechnique.h>
#include <Ogre/OgrePass.h>
#endif

namespace ion
{
	namespace renderer
	{
		//Forward declaration
		class Texture;

		class Material
		{
		public:
			Material();
			~Material();

			bool Load(std::string filename);
			u64 Save(std::string filename);

			enum BlendMode
			{
				Additive,
				Modulative,
				Replace,
				TransparentAlpha,
				TransparentColour
			};

			enum LightingMode
			{
				Flat,
				Gouraud,
				Phong
			};

			enum CullMode
			{
				None,
				Clockwise,
				CounterClockwise
			};

			enum ColourType
			{
				Ambient,
				Diffuse,
				Specular,
				Emissive
			};

			//Blend mode
			void SetBlendMode(BlendMode blendMode);
			BlendMode GetBlendMode() const;

			//Colour
			void SetAmbientColour(const Colour& ambient);
			void SetDiffuseColour(const Colour& diffuse);
			void SetSpecularColour(const Colour& specular);
			void SetEmissiveColour(const Colour& emissive);

			const Colour& GetAmbientColour() const;
			const Colour& GetDiffuseColour() const;
			const Colour& GetSpecularColour() const;
			const Colour& GetEmissiveColour() const;

			//Texture maps
			void AddDiffuseMap(Texture* diffuse);
			void SetNormalMap(Texture* normal);
			void SetSpecularMap(Texture* specular);
			void SetOpacityMap(Texture* opacity);

			const Texture* GetDiffuseMap(int diffuseMapIdx) const;
			const Texture* GetNormalMap() const;
			const Texture* GetSpecularMap() const;
			const Texture* GetOpacityMap() const;

			int GetNumDiffuseMaps() const;

			//Vertex colours
			void AssignVertexColour(ColourType colourType);

			//Lighting and shadows
			void SetLightingEnabled(bool lighting);
			void SetLightingMode(LightingMode mode);
			void SetReceiveShadows(bool shadows);

			bool GetLightingEnabled() const;
			LightingMode GetLightingMode() const;
			bool GetReceiveShadows() const;

			//Depth and culling
			void SetDepthTest(bool enabled);
			void SetDepthWrite(bool enabled);
			void SetCullMode(CullMode cullMode);

			#if defined ION_OGRE
			const std::string& GetOgreMaterialName() { return mOgreMaterialName; }
			#endif

		private:

			enum ChunkIds
			{
				ChunkId_Root,
					ChunkId_BlendMode,
					ChunkId_ColourAmbient,
					ChunkId_ColourDiffuse,
					ChunkId_ColourSpecular,
					ChunkId_ColourEmissive,
					ChunkId_MapDiffuse,
					ChunkId_MapNormal,
					ChunkId_MapSpecular,
					ChunkId_MapOpacity,

				NumChunkIds
			}; 

			static const u32 sChunkIds[NumChunkIds];

			static const int sMinFileVersion;
			static const int sCurrentFileVersion;
			static const char* sFileType;

			//For unique name generation
			static int sMaterialIndex;

			Colour mAmbientColour;
			Colour mDiffuseColour;
			Colour mSpecularColour;
			Colour mEmissiveColour;

			std::vector<Texture*> mDiffuseMaps;
			Texture* mNormalMap;
			Texture* mSpecularMap;
			Texture* mOpacityMap;

			bool mLightingEnabled;
			bool mReceiveShadows;
			LightingMode mLightingMode;

			BlendMode mBlendMode;

			#if defined ION_OGRE
			Ogre::MaterialPtr mOgreMaterial;
			Ogre::Technique* mOgreTechnique;
			Ogre::Pass* mOgrePass;
			std::string mOgreMaterialName;
			#endif
		};
	}
}