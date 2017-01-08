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

#include "Tile.h"
#include "Palette.h"
#include "SpriteAnimation.h"

#include <ion/core/cryptography/UUID.h>

typedef ion::UUID64 SpriteSheetId;
typedef std::vector<Tile> SpriteSheetFrame;
typedef std::map<SpriteAnimId, SpriteAnimation> TSpriteAnimMap;

static const SpriteSheetId InvalidSpriteSheetId = 0;

class SpriteSheet
{
public:
	static const int subSpriteWidthTiles = 4;
	static const int subSpriteHeightTiles = 4;

	SpriteSheet();

	bool ImportBitmap(const std::string& filename, const std::string& name, int tileWidth, int tileHeight, int widthFrames, int heightFrames, int maxFrames);

	const std::string& GetName() const;
	const SpriteSheetFrame& GetFrame(int index) const;
	int GetNumFrames() const;

	SpriteAnimId AddAnimation();
	void DeleteAnimation(SpriteAnimId animId);
	SpriteAnimation* GetAnimation(SpriteAnimId animId);
	int GetNumAnimations() const;
	TSpriteAnimMap::const_iterator AnimationsBegin() const;
	TSpriteAnimMap::const_iterator AnimationsEnd() const;

	u8 GetWidthTiles() const;
	u8 GetHeightTiles() const;

	void GetWidthSubsprites(u8& total, u8& whole, u8& remainder) const;
	void GetHeightSubsprites(u8& total, u8& whole, u8& remainder) const;

	void GetSubspriteDimensions(std::vector<ion::Vector2i>& dimensions, int tileWidth, int tileHeight) const;

	void GetSubspritePosOffsets(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const;
	void GetSubspritePosOffsetsFlippedX(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const;
	void GetSubspritePosOffsetsFlippedY(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const;
	void GetSubspritePosOffsetsFlippedXY(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const;

	const Palette& GetPalette() const;

	//Serialise
	void Serialise(ion::io::Archive& archive);
	void ExportTiles(const PlatformConfig& config, std::stringstream& stream) const;
	void ExportTiles(const PlatformConfig& config, ion::io::File& file) const;
	void ExportAnims(const PlatformConfig& config, std::stringstream& stream, const std::string& actorName) const;
	void ExportAnims(const PlatformConfig& config, ion::io::File& file) const;
	void ExportPalette(const PlatformConfig& config, std::stringstream& stream) const;
	u32 GetBinarySizeTiles() const;

private:
	std::string m_name;
	std::vector<SpriteSheetFrame> m_frames;
	TSpriteAnimMap m_animations;
	Palette m_palette;
	u8 m_widthTiles;
	u8 m_heightTiles;
};