typedef unsigned short ComponentHndl;

struct BlockHeader
{
	unsigned short flags;
	unsigned short nextBlock;
};

struct Entity : BlockHeader
{
	unsigned short typeDesc;
	unsigned int positionX;
	unsigned int positionY;
	unsigned short extentsX;
	unsigned short extentsY;
};