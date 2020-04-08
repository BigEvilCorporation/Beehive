typedef unsigned short ComponentHndl;

struct Fixed16
{
	union
	{
		struct
		{
			//Big endian order
			unsigned short integer;
			unsigned short fraction;
		};

		unsigned int value;
	};

	operator unsigned short() const { return integer; }
};

struct BlockHeader
{
#if defined _DEBUG
	static const int ENTITY_DEBUG_NAME_LEN = 16;
	char debugName[ENTITY_DEBUG_NAME_LEN];
#endif

	unsigned short flags;
	unsigned short nextBlock;
};

struct EntityTypeDesc
{
	unsigned short numComponents;
	unsigned short componentSlots;
	unsigned short constructor;
	unsigned short destructor;
	unsigned short update;
};

struct Entity : BlockHeader
{
	unsigned short typeDesc;
	Fixed16 positionX;
	Fixed16 positionY;
	unsigned short extentsX;
	unsigned short extentsY;
};

struct ComponentBase : BlockHeader
{
	unsigned short ownerEntity;
};

struct ComponentSlotDesc
{
	unsigned short addrOffset;
	unsigned short spawnDataOffset;
	unsigned int linkedList;
	unsigned short constructor;
	unsigned short destructor;
};

struct Scene
{
	void* gfxTileset;
	void* gfxStampset;
	void* gfxMapFg;
	void* gfxMapBg;
	void* colTileset;
	void* colStampset;
	void* colMap;
	void* palettes;
	void* staticEntities;
	void* dynamicEntities;
	unsigned short gfxTileCount;
	unsigned short gfxStampCount;
	unsigned short gfxMapFgWidthStamps;
	unsigned short gfxMapFgHeightStamps;
	unsigned short gfxMapBgWidthStamps;
	unsigned short gfxMapBgHeightStamps;
	unsigned short colTileCount;
	unsigned short colStampCount;
	unsigned short colMapWidthStamps;
	unsigned short colMapHeightStamps;
	unsigned short paletteCount;
	unsigned short staticEntityCount;
	unsigned short dynamicEntityCount;
};

enum TerrainFlags
{
	COLLISION_FLAG_BIT_TERRAIN_B	= 3,
	COLLISION_FLAG_BIT_TERRAIN_W	= COLLISION_FLAG_BIT_TERRAIN_B + 8,
	COLLISION_FLAG_BIT_SOLID_B		= 5,
	COLLISION_FLAG_BIT_SOLID_W		= COLLISION_FLAG_BIT_SOLID_B + 8,
};

struct Engine
{
	//Find floor under an entity, returns floor Y position relative to entity centre
	short (*FindFloor)(const Entity& entity, const Scene& scene, short& terrainFlags);
};