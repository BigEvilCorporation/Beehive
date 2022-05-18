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

#include "MapToolManipulator.h"

struct MapObjIdentifierStamp
{
	MapObjIdentifierStamp()
		: stampId(InvalidStampId)
		, stamp(nullptr)
		, stampFlags(0)
		, stampMapEntryIdx(0)
	{ }

	bool IsValid() const
	{
		return stamp != nullptr;
	}

	bool operator == (const MapObjIdentifierStamp& rhs) const
	{
		return stampPos == rhs.stampPos;
	}

	ion::maths::Bounds2i GetBounds() const
	{
		ion::maths::Bounds2i bounds;
		bounds.AddPoint(stampPos);
		bounds.AddPoint(stampPos + ion::Vector2i(1, 1));
		return bounds;
	}

	StampId stampId;
	Stamp* stamp;
	ion::Vector2i stampPos;
	u32 stampFlags;
	u32 stampMapEntryIdx;
};

class MapToolManipulatorStamp : public MapToolManipulator<MapObjIdentifierStamp>
{
public:
	MapToolManipulatorStamp(Project& project, MapPanel& mapPanel, TUndoStack& undoStack);
	virtual ~MapToolManipulatorStamp();

	virtual std::string GetName();
	virtual std::string GetDescription();
	virtual std::string GetStatusText();
	virtual std::string GetTooltipText(const MapObjIdentifierStamp& object, const ion::Vector2i& cursorPosPx);

	virtual ion::Vector2i GetUnitSizePx();
	virtual int GetRegionSelectorFlags();

	virtual void FindObjects(const Map& map, const std::vector<MapRegion>& regions, std::vector<MapObjIdentifierStamp>& selection);
	virtual void MoveObjects(Map& map, std::vector<MapObjIdentifierStamp>& selection, const ion::Vector2i& delta);
	virtual void DeleteObjects(Map& map, std::vector<MapObjIdentifierStamp>& selection);

	void OnContextMenuEditCollision(int id, std::vector<MapObjIdentifierStamp>& objects);

private:
	ion::Vector2i m_unitSizePx;
};
