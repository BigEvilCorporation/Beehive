///////////////////////////////////////////////////////
// MD Studio: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "Project.h"

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