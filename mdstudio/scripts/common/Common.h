typedef unsigned short ComponentHndl;

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
	unsigned int positionX;
	unsigned int positionY;
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