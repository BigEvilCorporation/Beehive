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

#include "StampAnimation.h"
#include "BMPReader.h"
#include "Project.h"

const u32 AnimTrackTileFrame::GetValue(float time) const
{
	u32 intValue = 0;

	if(const ion::render::Keyframe<u32>* keyframeA = GetPrevKeyframe(time))
	{
		intValue = keyframeA->GetValue();
	}

	return intValue;
}

void AnimTrackTileFrame::Export(std::stringstream& stream) const
{
	stream << "\tdc.b ";

	for(int i = 0; i < GetNumKeyframes(); i++)
	{
		u32 value = GetKeyframe(i).GetValue();
		stream << "0x" << value;

		if(i < GetNumKeyframes() - 1)
			stream << ", ";
	}

	stream << std::endl;
}

void AnimTrackTileFrame::Export(ion::io::File& file) const
{

}

TileFrame::TileFrame()
{
	m_width = 0;
	m_height = 0;
}

TileFrame::TileFrame(const TileFrame& rhs)
{
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_tiles = rhs.m_tiles;
}

TileFrame::TileFrame(const std::vector<std::pair<TileId, Tile*>>& tiles, int width, int height)
{
	m_tiles.reserve(tiles.size());
	m_width = width;
	m_height = height;

	for(int i = 0; i < tiles.size(); i++)
	{
		m_tiles.push_back(std::make_pair(tiles[i].first, *tiles[i].second));
	}
}

bool TileFrame::Import(const std::string filename, Project& project)
{
	const int tileWidth = project.GetPlatformConfig().tileWidth;
	const int tileHeight = project.GetPlatformConfig().tileHeight;

	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		int tilesWidth = (int)ion::maths::Ceil((float)reader.GetWidth() / (float)tileWidth);
		int tilesHeight = (int)ion::maths::Ceil((float)reader.GetHeight() / (float)tileHeight);

		if(m_tiles.size() != (tilesWidth * tilesHeight))
		{
			//TODO: wx message handler in ion::debug
			//wxMessageBox("Bitmap width/height does not match original frame", "Error", wxOK | wxICON_ERROR);
			return false;
		}

		//Read all tiles
		for(int x = 0; x < tilesWidth; x++)
		{
			for(int y = 0; y < tilesHeight; y++)
			{
				//Get existing tile
				std::pair<TileId, Tile>& tile = m_tiles[(y * tilesWidth) + x];

				//Get palette
				const Palette* palette = project.GetPalette(tile.second.GetPaletteId());

				//Read pixel colours from bitmap
				for(int pixelX = 0; pixelX < tileWidth; pixelX++)
				{
					for(int pixelY = 0; pixelY < tileHeight; pixelY++)
					{
						int sourcePixelX = (x * tileWidth) + pixelX;
						int sourcePixelY = (y * tileHeight) + pixelY;

						BMPReader::Colour bmpColour = reader.GetPixel(sourcePixelX, sourcePixelY);
						Colour colour(bmpColour.r, bmpColour.g, bmpColour.b);

						int colourIdx = 0;
						if(!palette->GetNearestColourIdx(colour, Palette::eExact, colourIdx))
						{
							//TODO: wx message handler in ion::debug
							//wxMessageBox("Palette mismatch", "Error", wxOK | wxICON_ERROR);
							return false;
						}

						tile.second.SetPixelColour(pixelX, pixelY, colourIdx);
					}
				}
			}
		}
	}

	return true;
}

void TileFrame::Serialise(ion::io::Archive& archive)
{

}

StampAnimation::StampAnimation()
{
	m_speed = 10;
}

StampAnimation::StampAnimation(const std::string& name)
{
	m_speed = 10;
	m_name = name;
}

void StampAnimation::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& StampAnimation::GetName() const
{
	return m_name;
}

void StampAnimation::AddTileFrame(const TileFrame& frame)
{
	m_tileFrames.push_back(frame);
}

const TileFrame& StampAnimation::GetTileFrame(int index)
{
	return m_tileFrames[index];
}

int StampAnimation::GetTileFrameCount() const
{
	return m_tileFrames.size();
}

void StampAnimation::SetSpeed(int speed)
{
	m_speed = speed;
}

int StampAnimation::GetSpeed() const
{
	return m_speed;
}

void StampAnimation::Serialise(ion::io::Archive& archive)
{

}
