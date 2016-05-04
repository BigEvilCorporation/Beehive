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