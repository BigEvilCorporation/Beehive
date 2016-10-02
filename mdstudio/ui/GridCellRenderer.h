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

#include "UIBase.h"

class GridCellBitmapRenderer : public wxGridCellStringRenderer
{
public:
	enum SelectionEdge
	{
		eDrawSelectionEdgeNone,
		eDrawSelectionEdgeLeft,
		eDrawSelectionEdgeRight
	};

	static const int s_iconBorderX = 16;
	static const int s_iconBorderY = 2;

	GridCellBitmapRenderer(wxImageList* imageList);
	virtual ~GridCellBitmapRenderer();

	void SetImageList(wxImageList* imageList);
	wxImageList* GetImageList() const;

	void SetDrawSelectionEdge(SelectionEdge selectionEdge);

	virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected);

private:
	wxImageList* m_imageList;
	SelectionEdge m_selectionEdge;
};