#include "Tile.h"
#include "Tileset.h"

class Stamp
{
public:
	Stamp(int width, int height);

	int GetWidth() const;
	int GetHeight() const;

	void SetTile(int x, int y, TileId tile);
	TileId GetTile(int x, int y) const;

	void SetTileFlags(int x, int y, u32 flags);
	u32 GetTileFlags(int x, int y) const;

	void Serialise(ion::io::Archive& archive);

private:
	int m_width;
	int m_height;

	struct TileDesc
	{
		TileDesc() { m_id = 0; m_flags = 0; }

		void Serialise(ion::io::Archive& archive)
		{
			archive.Serialise(m_id);
			archive.Serialise(m_flags);
		}

		TileId m_id;
		u32 m_flags;
	};

	std::vector<TileDesc> m_tiles;
};