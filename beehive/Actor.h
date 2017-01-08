///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
// http://www.bigevilcorporation.co.uk
// mattphillips@mail.com
// @big_evil_corp
//
// Licensed under GPLv3, see http://www.gnu.org/licenses/gpl-3.0.html
///////////////////////////////////////////////////////

#pragma once

#include "SpriteSheet.h"

typedef std::map<SpriteSheetId, SpriteSheet> TSpriteSheetMap;

class Actor
{
public:
	Actor();

	const std::string& GetName() const;
	void SetName(const std::string& name);

	//Sprite sheets
	SpriteSheetId CreateSpriteSheet();
	void DeleteSpriteSheet(SpriteSheetId spriteSheetId);
	SpriteSheet* GetSpriteSheet(SpriteSheetId spriteSheetId);
	const SpriteSheet* GetSpriteSheet(SpriteSheetId SpriteSheetId) const;
	const TSpriteSheetMap::const_iterator SpriteSheetsBegin() const;
	const TSpriteSheetMap::const_iterator SpriteSheetsEnd() const;
	int GetSpriteSheetCount() const;

	void SetMasterPalette(SpriteSheetId spriteSheetId);

	//Serialise
	void Serialise(ion::io::Archive& archive);
	void ExportSpriteSheets(const PlatformConfig& config, std::stringstream& stream) const;
	void ExportSpriteAnims(const PlatformConfig& config, std::stringstream& stream) const;
	void ExportSpritePalettes(const PlatformConfig& config, std::stringstream& stream) const;
	void ExportSpriteSheets(const PlatformConfig& config, ion::io::File& file) const;
	void ExportSpriteAnims(const PlatformConfig& config, ion::io::File& file) const;

private:
	std::string m_name;
	std::map<SpriteSheetId, SpriteSheet> m_spriteSheets;
	SpriteSheetId m_masterPaletteHolder;
};