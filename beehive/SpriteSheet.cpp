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

#include <ion/core/debug/Debug.h>

#include "SpriteSheet.h"
#include "BMPReader.h"

SpriteSheet::SpriteSheet()
{
	m_widthTiles = 0;
	m_heightTiles = 0;
}

bool SpriteSheet::ImportBitmap(const std::string& filename, const std::string& name, int tileWidth, int tileHeight, int widthFrames, int heightFrames, int maxFrames)
{
	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		if(reader.GetWidth() % tileWidth != 0 || reader.GetHeight() % tileHeight != 0)
		{
			//TODO: wx message handler in ion::debug
			//if(wxMessageBox("Bitmap width/height is not multiple of target platform tile width/height", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return false;
			}
		}

		//Clear palette
		m_palette = Palette();

		//Import palette
		for(int i = 0; i < reader.GetPaletteSize(); i++)
		{
			BMPReader::Colour bmpColour = reader.GetPaletteEntry(i);
			m_palette.AddColour(Colour(bmpColour.r, bmpColour.g, bmpColour.b));
		}
	
		//Get total spriteSheet sheet width/height in tiles
		int spriteSheetWidthTiles = reader.GetWidth() / tileWidth;
		int spriteSheetHeightTiles = reader.GetHeight() / tileHeight;
	
		//Get frame width/height in tiles
		m_widthTiles = spriteSheetWidthTiles / widthFrames;
		m_heightTiles = spriteSheetHeightTiles / heightFrames;
	
		//For each frame
		u32 frameCount = 0;

		for(int frameY = 0; frameY < heightFrames && frameCount < maxFrames; frameY++)
		{
			for(int frameX = 0; frameX < widthFrames && frameCount < maxFrames; frameX++)
			{
				//Create new frame
				SpriteSheetFrame frame;
	
				//For each tile in frame (Mega Drive order = column major)
				for(int tileX = 0; tileX < m_widthTiles; tileX++)
				{
					for(int tileY = 0; tileY < m_heightTiles; tileY++)
					{
						//Read tile
						Tile tile(tileWidth, tileHeight);

						//Read pixel colours from bitmap
						for(int pixelX = 0; pixelX < tileWidth; pixelX++)
						{
							for(int pixelY = 0; pixelY < tileHeight; pixelY++)
							{
								//Read pixel
								int sourcePixelX = (frameX * m_widthTiles * tileWidth) + (tileX * tileWidth) + pixelX;
								int sourcePixelY = (frameY * m_heightTiles * tileHeight) + (tileY * tileHeight) + pixelY;
								u8 colourIndex = reader.GetColourIndex(sourcePixelX, sourcePixelY);
								tile.SetPixelColour(pixelX, pixelY, colourIndex);
							}
						}

						//Add tile
						frame.push_back(tile);
					}
				}

				//Add frame
				m_frames.push_back(frame);

				frameCount++;
			}
		}

		//Set name
		m_name = name;
	}

	return true;
}

const std::string& SpriteSheet::GetName() const
{
	return m_name;
}

const SpriteSheetFrame& SpriteSheet::GetFrame(int index) const
{
	ion::debug::Assert(index >= 0 && index < GetNumFrames(), "SpriteSheet::GetFrame() - eOut of range");
	return m_frames[index];
}

int SpriteSheet::GetNumFrames() const
{
	return m_frames.size();
}

SpriteAnimId SpriteSheet::AddAnimation()
{
	SpriteAnimId animId = ion::GenerateUUID64();
	m_animations.insert(std::make_pair(animId, SpriteAnimation()));
	return animId;
}

void SpriteSheet::DeleteAnimation(SpriteAnimId animId)
{
	m_animations.erase(animId);
}

SpriteAnimation* SpriteSheet::GetAnimation(SpriteAnimId animId)
{
	SpriteAnimation* anim = NULL;

	TSpriteAnimMap::iterator it = m_animations.find(animId);
	if(it != m_animations.end())
	{
		anim = &it->second;
	}

	return anim;
}

int SpriteSheet::GetNumAnimations() const
{
	return m_animations.size();
}

TSpriteAnimMap::const_iterator SpriteSheet::AnimationsBegin() const
{
	return m_animations.begin();
}

TSpriteAnimMap::const_iterator SpriteSheet::AnimationsEnd() const
{
	return m_animations.end();
}

u8 SpriteSheet::GetWidthTiles() const
{
	return m_widthTiles;
}

u8 SpriteSheet::GetHeightTiles() const
{
	return m_heightTiles;
}

void SpriteSheet::GetWidthSubsprites(u8& total, u8& whole, u8& remainder) const
{
	total = ion::maths::Ceil((float)m_widthTiles / (float)subSpriteWidthTiles);
	whole = m_widthTiles / subSpriteWidthTiles;
	remainder = m_widthTiles % subSpriteWidthTiles;
}

void SpriteSheet::GetHeightSubsprites(u8& total, u8& whole, u8& remainder) const
{
	total = ion::maths::Ceil((float)m_heightTiles / (float)subSpriteHeightTiles);
	whole = m_heightTiles / subSpriteHeightTiles;
	remainder = m_heightTiles % subSpriteHeightTiles;
}

void SpriteSheet::GetSubspriteDimensions(std::vector<ion::Vector2i>& dimensions, int tileWidth, int tileHeight) const
{
	int width = ion::maths::Ceil((float)m_widthTiles / (float)subSpriteWidthTiles);
	int height = ion::maths::Ceil((float)m_heightTiles / (float)subSpriteHeightTiles);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			int subSprWidth = ion::maths::Min(subSpriteWidthTiles, m_widthTiles - (subSpriteWidthTiles * x));
			int subSprHeight = ion::maths::Min(subSpriteHeightTiles, m_heightTiles - (subSpriteHeightTiles * y));
			dimensions.push_back(ion::Vector2i(subSprWidth, subSprHeight));
		}
	}
}

void SpriteSheet::GetSubspritePosOffsets(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const
{
	int width = ion::maths::Ceil((float)m_widthTiles / (float)subSpriteWidthTiles);
	int height = ion::maths::Ceil((float)m_heightTiles / (float)subSpriteHeightTiles);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			int subSprX = (x * subSpriteWidthTiles * tileWidth);
			int subSprY = (y * subSpriteHeightTiles * tileHeight);
			offsets.push_back(ion::Vector2i(subSprX, subSprY));
		}
	}
}

void SpriteSheet::GetSubspritePosOffsetsFlippedX(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const
{
	int width = ion::maths::Ceil((float)m_widthTiles / (float)subSpriteWidthTiles);
	int height = ion::maths::Ceil((float)m_heightTiles / (float)subSpriteHeightTiles);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			int subSprWidth = ion::maths::Min(subSpriteWidthTiles, m_widthTiles - (subSpriteWidthTiles * x));
			int subSprX = (m_widthTiles - (subSprWidth + (x * subSpriteWidthTiles))) * tileWidth;
			int subSprY = (y * subSpriteHeightTiles * tileHeight);
			offsets.push_back(ion::Vector2i(subSprX, subSprY));
		}
	}
}

void SpriteSheet::GetSubspritePosOffsetsFlippedY(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const
{

}

void SpriteSheet::GetSubspritePosOffsetsFlippedXY(std::vector<ion::Vector2i>& offsets, int tileWidth, int tileHeight) const
{

}

const Palette& SpriteSheet::GetPalette() const
{
	return m_palette;
}

void SpriteSheet::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_frames, "frames");
	archive.Serialise(m_animations, "animations");
	archive.Serialise(m_palette, "palette");
	archive.Serialise(m_widthTiles, "width");
	archive.Serialise(m_heightTiles, "height");
}

void SpriteSheet::ExportTiles(const PlatformConfig& config, std::stringstream& stream) const
{
	for(std::vector<SpriteSheetFrame>::const_iterator it = m_frames.begin(), end = m_frames.end(); it != end; ++it)
	{
		//Split into subsprites
		const int widthSubSprites = ion::maths::Ceil((float)m_widthTiles / (float)subSpriteWidthTiles);
		const int heightSubSprites = ion::maths::Ceil((float)m_heightTiles / (float)subSpriteHeightTiles);

		//Subsprites column major
		for(int subSprX = 0; subSprX < widthSubSprites; subSprX++)
		{
			for(int subSprY = 0; subSprY < heightSubSprites; subSprY++)
			{
				int subSprOffsetY = subSpriteHeightTiles * subSprY;
				int subSprOffsetX = m_heightTiles * subSpriteWidthTiles * subSprX;
				int topLeft = subSprOffsetY + subSprOffsetX;

				//Tiles column major
				for(int x = 0; x < ion::maths::Min(subSpriteWidthTiles, m_widthTiles - (subSpriteWidthTiles * subSprX)); x++)
				{
					for(int y = 0; y < ion::maths::Min(subSpriteHeightTiles, m_heightTiles - (subSpriteHeightTiles * subSprY)); y++)
					{
						(*it)[topLeft + (x * m_heightTiles) + y].Export(config, stream);

						stream << std::endl;
					}
				}
			}
		}
	}
}

void SpriteSheet::ExportTiles(const PlatformConfig& config, ion::io::File& file) const
{

}

void SpriteSheet::ExportAnims(const PlatformConfig& config, std::stringstream& stream, const std::string& actorName) const
{
	stream << std::hex << std::setfill('0') << std::uppercase;

	for(TSpriteAnimMap::const_iterator it = m_animations.begin(), end = m_animations.end(); it != end; ++it)
	{
		std::stringstream label;
		label << "spriteanim_" << actorName << "_" << it->second.GetName();
		
		stream << label.str() << ":" << std::endl;
		stream << label.str() << "_numframes: equ 0x" << it->second.m_trackSpriteFrame.GetNumKeyframes() << std::endl;
		stream << label.str() << "_speed: equ 0x" << it->second.GetSpeed() << std::endl;

		stream << label.str() << "_track_frames:" << std::endl;
		it->second.m_trackSpriteFrame.Export(stream);

		stream << label.str() << "_track_posx:" << std::endl;
		it->second.m_trackPosition.ExportX(stream);

		stream << label.str() << "_track_posy:" << std::endl;
		it->second.m_trackPosition.ExportY(stream);

		stream << std::endl << "\tEven" << std::endl;
	}

	stream << std::dec;
}

void SpriteSheet::ExportAnims(const PlatformConfig& config, ion::io::File& file) const
{

}

void SpriteSheet::ExportPalette(const PlatformConfig& config, std::stringstream& stream) const
{
	m_palette.Export(stream);
}

u32 SpriteSheet::GetBinarySizeTiles() const
{
	u32 size = 0;

	for(std::vector<SpriteSheetFrame>::const_iterator it = m_frames.begin(), end = m_frames.end(); it != end; ++it)
	{
		for(int i = 0; i < it->size(); i++)
		{
			size += (*it)[i].GetBinarySize();
		}
	}

	return size;
}