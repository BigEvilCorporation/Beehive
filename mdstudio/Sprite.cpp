///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2016 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include <ion/core/debug/Debug.h>

#include "Sprite.h"
#include "BMPReader.h"

Sprite::Sprite()
{

}

bool Sprite::ImportBitmap(const std::string& filename, int widthFrames, int heightFrames, int maxFrames)
{
	//Read BMP
	//BMPReader reader;
	//if(reader.Read(filename))
	//{
	//	if(reader.GetWidth() % 8 != 0 || reader.GetHeight() % 8 != 0)
	//	{
	//		if(wxMessageBox("Bitmap width/height is not multiple of 8", "Warning", wxOK | wxCANCEL | wxICON_WARNING) == wxCANCEL)
	//		{
	//			return false;
	//		}
	//	}
	//
	//	//Get width/height in tiles
	//	int widthTiles = reader.GetWidth() / 8;
	//	int heightTiles = reader.GetHeight() / 8;
	//
	//	//Get frame width/height in tiles
	//	int frameWidthTiles = widthTiles / widthFrame;
	//	int frameHeightTiles = heightTiles / heightFrame;
	//
	//	//For each frame
	//	for(int frameY = 0; frameY < heightframes; frameY++)
	//	{
	//		for(int frameX = 0; frameX < widthframes; frameX++)
	//		{
	//			//Create new frame
	//			SpriteFrame frame;
	//
	//			//For each tile in frame (Mega Drive order = column major)
	//			for(int tileX = 0; tileX < frameWidthTiles; tileX++)
	//			{
	//				for(int tileY = 0; tileY < frameHeightTiles; tileY++)
	//				{
	//					//Read tile
	//					Tile tile;
	//
	//				}
	//			}
	//		}
	//	}
	//}

	return true;
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

const Palette& Sprite::GetPalette() const
{
	return m_palette;
}