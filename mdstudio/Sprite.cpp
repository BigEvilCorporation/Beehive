///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <ion/core/debug/Debug.h>

#include <wx/dialog.h>
#include <wx/msgdlg.h>

#include "Sprite.h"
#include "BMPReader.h"

Sprite::Sprite()
{
	m_widthTiles = 0;
	m_heightTiles = 0;
}

bool Sprite::ImportBitmap(const std::string& filename, const std::string& name, int widthFrames, int heightFrames, int maxFrames)
{
	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		if(reader.GetWidth() % 8 != 0 || reader.GetHeight() % 8 != 0)
		{
			if(wxMessageBox("Bitmap width/height is not multiple of 8", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
			{
				return false;
			}
		}

		//Clear palette
		m_palette = Palette();

		//Import palette
		for(int i = 0; i < reader.GetPaletteSize(); i++)
		{
			m_palette.AddColour(reader.GetPaletteEntry(i));
		}

		const int tileWidth = 8;
		const int tileHeight = 8;
	
		//Get total sprite sheet width/height in tiles
		int spriteSheetWidthTiles = reader.GetWidth() / 8;
		int spriteSheetHeightTiles = reader.GetHeight() / 8;
	
		//Get frame width/height in tiles
		m_widthTiles = spriteSheetWidthTiles / widthFrames;
		m_heightTiles = spriteSheetHeightTiles / heightFrames;
	
		//For each frame
		for(int frameY = 0; frameY < heightFrames; frameY++)
		{
			for(int frameX = 0; frameX < widthFrames; frameX++)
			{
				//Create new frame
				SpriteFrame frame;
	
				//For each tile in frame (Mega Drive order = column major)
				for(int tileX = 0; tileX < m_widthTiles; tileX++)
				{
					for(int tileY = 0; tileY < m_heightTiles; tileY++)
					{
						//Read tile
						Tile tile;

						//Read pixel colours from bitmap
						for(int pixelX = 0; pixelX < tileWidth; pixelX++)
						{
							for(int pixelY = 0; pixelY < tileHeight; pixelY++)
							{
								//Read pixel
								int sourcePixelX = (tileX * tileWidth) + pixelX;
								int sourcePixelY = (tileY * tileHeight) + pixelY;
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
			}
		}

		//Set name
		m_name = name;
	}

	return true;
}

const std::string& Sprite::GetName() const
{
	return m_name;
}

const SpriteFrame& Sprite::GetFrame(int index) const
{
	ion::debug::Assert(index >= 0 && index < GetNumFrames(), "Sprite::GetFrame() - Out of range");
	return m_frames[index];
}

int Sprite::GetNumFrames() const
{
	return m_frames.size();
}

u8 Sprite::GetWidthTiles() const
{
	return m_widthTiles;
}

u8 Sprite::GetHeightTiles() const
{
	return m_heightTiles;
}

const Palette& Sprite::GetPalette() const
{
	return m_palette;
}

void Sprite::Serialise(ion::io::Archive& archive)
{
	archive.Serialise(m_name, "name");
	archive.Serialise(m_frames, "frames");
	archive.Serialise(m_palette, "palette");
	archive.Serialise(m_widthTiles, "width");
	archive.Serialise(m_heightTiles, "height");
}