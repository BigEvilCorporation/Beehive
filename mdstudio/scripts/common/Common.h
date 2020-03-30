typedef unsigned short ComponentHndl;

struct Fixed16
{
	union
	{
		struct
		{
			unsigned short integer;
			unsigned short fraction;
		};

		unsigned int value;
	};

	operator unsigned short() const { return integer; }
};

struct BlockHeader
{
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

struct Engine
{
	short (*FindFloor)(const Entity& entity, const Scene& scene);
};