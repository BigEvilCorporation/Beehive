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

void Actor::Export(std::stringstream& stream) const
{
	for(TSpriteSheetMap::const_iterator it = m_spriteSheets.begin(), end = m_spriteSheets.end(); it != end; ++it)
	{
		std::stringstream label;
		label << "spritesheet_" << m_name << "_" << it->second.GetName();

		//AnimFrame_NymnWalk equ((*-Tiles_Nymn) / tiles_nymn_size_b)

		stream << label.str() << ":" << std::endl << std::endl;

		it->second.Export(stream);

		stream << std::endl;

		stream << label.str() << "_end" << std::endl;
		stream << label.str() << "_size_b\tequ (" << label.str() << "_end-" << label.str() << ")\t; Size in bytes" << std::endl;
		stream << label.str() << "_size_w\tequ (" << label.str() << "_size_b/2)\t; Size in words" << std::endl;
		stream << label.str() << "_size_l\tequ (" << label.str() << "_size_b/4)\t; Size in longwords" << std::endl;
		stream << label.str() << "_size_t\tequ (" << label.str() << "_size_b/32)\t; Size in tiles" << std::endl;
		//stream << label.str() << "_tileid\tequ (" << ;

		stream << std::endl << std::endl;
	}
}

void Actor::Export(ion::io::File& file) const
{

}