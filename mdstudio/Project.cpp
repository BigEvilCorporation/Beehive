///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Project.h"
#include "BMPReader.h"

Project::Project()
{
	m_paintTile = 0;
	m_eraseTile = 0;
	m_mapInvalidated = true;
}

void Project::SetPaintTile(TileId tile)
{
	m_paintTile = tile;
}

TileId Project::GetPaintTile() const
{
	return m_paintTile;
}

void Project::SetEraseTile(TileId tile)
{
	m_eraseTile = tile;
}

TileId Project::GetEraseTile() const
{
	return m_eraseTile;
}

bool Project::ImportBitmap(const std::string& filename, u8 importFlags)
{
	if(importFlags & BMPImportReplaceAll)
	{
		//TODO: Clear map, tiles and palettes
	}

	//Read BMP
	BMPReader reader;
	if(reader.Read(filename))
	{
		//For all 8x8 tiles
			//Read each pixel, map unique colours, assign an index to each unique colour
			//Warn if >15 unique colours in tile
			//Map 15 colour combination, if unique copy to new palette and assign tile a palette id
			//Warn if >4 palettes
			//Calc tile hash and find duplicate
			//Assign tile id if dupe, add new tile if not

			if(importFlags & BMPImportDrawToMap)
			{
				//Set tile id on map
			}
	}

	return true;
}