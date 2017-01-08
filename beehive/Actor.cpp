#include "Actor.h"

#define HEX1(val) std::hex << std::setfill('0') << std::setw(1) << std::uppercase << (int)##val
#define HEX2(val) std::hex << std::setfill('0') << std::setw(2) << std::uppercase << (int)##val
#define HEX4(val) std::hex << std::setfill('0') << std::setw(4) << std::uppercase << (int)##val

Actor::Actor()
{

}

const std::string& Actor::GetName() const
{
	return m_name;
}

void Actor::SetName(const std::string& name)
{
	m_name = name;
}

SpriteSheetId Actor::CreateSpriteSheet()
{
	SpriteSheetId spriteSheetId = ion::GenerateUUID64();
	m_spriteSheets.insert(std::make_pair(spriteSheetId, SpriteSheet()));
	return spriteSheetId;
}

void Actor::DeleteSpriteSheet(SpriteSheetId spriteSheetId)
{
	TSpriteSheetMap::iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		m_spriteSheets.erase(it);
	}
}

SpriteSheet* Actor::GetSpriteSheet(SpriteSheetId spriteSheetId)
{
	SpriteSheet* spriteSheet = NULL;

	TSpriteSheetMap::iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		spriteSheet = &it->second;
	}

	return spriteSheet;
}

const SpriteSheet* Actor::GetSpriteSheet(SpriteSheetId spriteSheetId) const
{
	const SpriteSheet* spriteSheet = NULL;

	TSpriteSheetMap::const_iterator it = m_spriteSheets.find(spriteSheetId);
	if(it != m_spriteSheets.end())
	{
		spriteSheet = &it->second;
	}

	return spriteSheet;
}

const TSpriteSheetMap::const_iterator Actor::SpriteSheetsBegin() const
{
	return m_spriteSheets.begin();
}

const TSpriteSheetMap::const_iterator Actor::SpriteSheetsEnd() const
{
	return m_spriteSheets.end();
}

int Actor::GetSpriteSheetCount() const
{
	return m_spriteSheets.size();
}

void Actor::SetMasterPalette(SpriteSheetId spriteSheetId)
{
	m_masterPaletteHolder = spriteSheetId;
}

void Actor::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_masterPaletteHolder, "masterPaletteHolder");
	archive.Serialise(m_spriteSheets, "spriteSheets");
}

void Actor::ExportSpriteSheets(const PlatformConfig& config, std::stringstream& stream) const
{
	u32 frameIndex = 0;

	stream << "actor_" << m_name << ":" << std::endl << std::endl;

	//Export sprite dimensions header
	if(m_spriteSheets.size() > 0)
	{
		const SpriteSheet& spriteSheet = m_spriteSheets.begin()->second;

		std::stringstream label;
		label << "sprite_" << m_name;

		u8 widthTotal;
		u8 widthWhole;
		u8 widthRemainder;
		u8 heightTotal;
		u8 heightWhole;
		u8 heightRemainder;

		std::vector<ion::Vector2i> subSprDimensionsTiles;
		std::vector<ion::Vector2i> subSprOffsetsUnflipped;
		std::vector<ion::Vector2i> subSprOffsetsFlippedX;

		spriteSheet.GetWidthSubsprites(widthTotal, widthWhole, widthRemainder);
		spriteSheet.GetHeightSubsprites(heightTotal, heightWhole, heightRemainder);
		spriteSheet.GetSubspriteDimensions(subSprDimensionsTiles, config.tileWidth, config.tileHeight);
		spriteSheet.GetSubspritePosOffsets(subSprOffsetsUnflipped, config.tileWidth, config.tileHeight);
		spriteSheet.GetSubspritePosOffsetsFlippedX(subSprOffsetsFlippedX, config.tileWidth, config.tileHeight);

		int sizeTiles = spriteSheet.GetWidthTiles() * spriteSheet.GetHeightTiles();

		stream << label.str() << "_size_b\t\tequ 0x" << HEX4(sizeTiles * 32) << "\t; Size in bytes" << std::endl;
		stream << label.str() << "_size_t\t\tequ 0x" << HEX4(sizeTiles) << "\t; Size in tiles" << std::endl;
		stream << label.str() << "_size_subsprites\t\tequ 0x" << HEX4(widthTotal * heightTotal) << "\t; Size in subsprites" << std::endl;
		stream << label.str() << "_widthheight_subsprites\t\tequ 0x" << HEX2(widthTotal) << HEX2(heightTotal) << "\t; Width/height (bb) in subsprites" << std::endl;

		stream << std::endl;

		stream << "; Subsprite offsets from 0,0 (in pixels) - unflipped (bb) and flipped X (bb)" << std::endl;
		stream << label.str() << "_subsprite_pos_offsets:" << std::endl;

		for(int i = 0; i < subSprOffsetsUnflipped.size(); i++)
		{
			stream << "\tdc.w 0x"
				<< HEX2(subSprOffsetsUnflipped[i].x)
				<< HEX2(subSprOffsetsUnflipped[i].y)
				<< ", 0x"
				<< HEX2(subSprOffsetsFlippedX[i].x)
				<< HEX2(subSprOffsetsFlippedX[i].y)
				<< std::endl;
		}

		stream << "\tEven" << std::endl << std::endl;

		stream << "; Subsprite dimension bits (for sprite descs)" << std::endl;
		stream << label.str() << "_subsprite_dimensions_bits:" << std::endl;

		for(int i = 0; i < subSprDimensionsTiles.size(); i++)
		{
			u8 bits = ((subSprDimensionsTiles[i].x - 1) << 2) | (subSprDimensionsTiles[i].y - 1);
			stream << "\tdc.b 0x" << HEX1(bits) << std::endl;
		}

		stream << "\tEven" << std::endl << std::endl;

		stream << "; Num tiles per subsprite (for sprite descs)" << std::endl;
		stream << label.str() << "_numtiles_per_subsprite:" << std::endl;

		for(int i = 0; i < subSprDimensionsTiles.size(); i++)
		{
			stream << "\tdc.b 0x" << HEX2(subSprDimensionsTiles[i].x * subSprDimensionsTiles[i].y) << std::endl;
		}

		stream << "\tEven" << std::endl << std::endl;

		stream << std::dec;
		stream <<  std::endl;
	}

	//Export sprite sheet size headers
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		std::stringstream label;
		label << "spritesheet_" << m_name << "_" << it->second.GetName();

		u32 size = it->second.GetBinarySizeTiles();

		stream << label.str() << "_frameoffset\tequ 0x" << HEX2(frameIndex) << "\t; Offset to first frame in sprite sheet" << std::endl;
		stream << label.str() << "_size_b\t\tequ 0x" << HEX2(size) << "\t; Size in bytes" << std::endl;
		stream << label.str() << "_size_w\t\tequ 0x" << HEX2(size/2) << "\t; Size in words" << std::endl;
		stream << label.str() << "_size_l\t\tequ 0x" << HEX2(size/4) << "\t; Size in longwords" << std::endl;
		stream << label.str() << "_size_t\t\tequ 0x" << HEX2(size/32) << "\t; Size in tiles" << std::endl;
		stream << std::dec;

		stream << std::endl << std::endl;

		frameIndex += it->second.GetNumFrames();
	}

	stream << "spritesheets_" << m_name << ":" << std::endl << std::endl;

	//Export sprite sheet tile data
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		std::stringstream label;
		label << "spritesheet_" << m_name << "_" << it->second.GetName();

		stream << label.str() << ":" << std::endl << std::endl;

		it->second.ExportTiles(config, stream);

		stream << std::endl << std::endl;
	}
}

void Actor::ExportSpriteSheets(const PlatformConfig& config, ion::io::File& file) const
{

}

void Actor::ExportSpriteAnims(const PlatformConfig& config, std::stringstream& stream) const
{
	stream << "spriteanims_" << m_name << ":" << std::endl << std::endl;

	//Export sprite anim data
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		it->second.ExportAnims(config, stream, m_name);
		stream << std::endl;
	}
}

void Actor::ExportSpriteAnims(const PlatformConfig& config, ion::io::File& file) const
{

}

void Actor::ExportSpritePalettes(const PlatformConfig& config, std::stringstream& stream) const
{
	if(const SpriteSheet* spriteSheet = GetSpriteSheet(m_masterPaletteHolder))
	{
		//Export palette from palette master
		stream << "palette_" << m_name << ":" << std::endl;
		spriteSheet->ExportPalette(config, stream);
		stream << std::endl;
	}
	else if(m_spriteSheets.size() > 0)
	{
		//Use first palette
		stream << "palette_" << m_name << ":" << std::endl;
		m_spriteSheets.begin()->second.ExportPalette(config, stream);
		stream << std::endl;
	}
}