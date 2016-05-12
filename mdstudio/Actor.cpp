#include "Actor.h"

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

void Actor::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_spriteSheets, "spriteSheets");
}

void Actor::ExportSpriteSheets(std::stringstream& stream) const
{
	u32 frameIndex = 0;

	stream << "actor_" << m_name << ":" << std::endl << std::endl;

	//Export sprite sheet size headers
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		std::stringstream label;
		label << "spritesheet_" << m_name << "_" << it->second.GetName();

		u32 size = it->second.GetBinarySizeTiles();

		stream << std::hex << std::setfill('0') << std::uppercase;
		stream << label.str() << "_frameoffset\tequ 0x" << frameIndex << "\t; Offset to first frame in sprite sheet" << std::endl;
		stream << label.str() << "_size_b\t\tequ 0x" << size << "\t; Size in bytes" << std::endl;
		stream << label.str() << "_size_w\t\tequ 0x" << size/2 << "\t; Size in words" << std::endl;
		stream << label.str() << "_size_l\t\tequ 0x" << size/4 << "\t; Size in longwords" << std::endl;
		stream << label.str() << "_size_t\t\tequ 0x" << size/32 << "\t; Size in tiles" << std::endl;
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

		it->second.ExportTiles(stream);

		stream << std::endl << std::endl;
	}
}

void Actor::ExportSpriteSheets(ion::io::File& file) const
{

}

void Actor::ExportSpriteAnims(std::stringstream& stream) const
{
	stream << "spriteanims_" << m_name << ":" << std::endl << std::endl;

	//Export sprite anim data
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		it->second.ExportAnims(stream, m_name);
		stream << std::endl;
	}
}

void Actor::ExportSpriteAnims(ion::io::File& file) const
{

}