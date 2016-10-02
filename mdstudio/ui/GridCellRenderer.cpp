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

#include "GridCellRenderer.h"

#include <wx/dc.h>

GridCellBitmapRenderer::GridCellBitmapRenderer(wxImageList* imageList)
{
	m_imageList = imageList;
	m_selectionEdge = eDrawSelectionEdgeNone;
}

GridCellBitmapRenderer::~GridCellBitmapRenderer()
{

}

void GridCellBitmapRenderer::SetImageList(wxImageList* imageList)
{
	m_imageList = imageList;
}

wxImageList* GridCellBitmapRenderer::GetImageList() const
{
	return m_imageList;
}

void GridCellBitmapRenderer::SetDrawSelectionEdge(SelectionEdge selectionEdge)
{
	m_selectionEdge = selectionEdge;
}

void GridCellBitmapRenderer::Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected)
{
	wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

	if(m_imageList && col < m_imageList->GetImageCount())
	{
		dc.DrawBitmap(m_imageList->GetBitmap(col), rect.x + s_iconBorderX, rect.y + s_iconBorderY);
	}

	const int penWidth = 4;

	wxPen pen;
	pen.SetColour(wxColour(0, 0, 0, 255));
	pen.SetWidth(penWidth);
	dc.SetPen(pen);

	if(m_selectionEdge == eDrawSelectionEdgeLeft)
	{
		dc.DrawLine(rect.x + (penWidth / 2), rect.y, rect.x + (penWidth / 2), rect.y + rect.GetHeight());
	}
	else if(m_selectionEdge == eDrawSelectionEdgeRight)
	{
		dc.DrawLine(rect.x + rect.GetWidth() - (penWidth / 2), rect.y, rect.x + rect.GetWidth() - (penWidth / 2), rect.y + rect.GetHeight());
	}
}