///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTypePanel.h"
#include "MainWindow.h"
#include <wx/App.h>
#include <maths/Vector.h>

CollisionTypePanel::CollisionTypePanel(	MainWindow* mainWindow,
								wxWindow *parent,
								wxWindowID id,
								const wxPoint& pos,
								const wxSize& size,
								long style,
								const wxString& name)
	: wxPanel(parent, id, pos, size, style, name)
{
	m_project = NULL;
	m_mainWindow = mainWindow;
	m_orientation = eVertical;

	Bind(wxEVT_LEFT_DOWN,		&CollisionTypePanel::OnMouse, this, GetId());
	Bind(wxEVT_LEFT_DCLICK,		&CollisionTypePanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			&CollisionTypePanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND,&CollisionTypePanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			&CollisionTypePanel::OnResize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void CollisionTypePanel::OnMouse(wxMouseEvent& event)
{
	if(m_project)
	{
		//Get mouse position in map space
		wxClientDC clientDc(this);
		wxPoint mouseCanvasPosWx = event.GetLogicalPosition(clientDc);
		ion::Vector2 mousePosMapSpace(mouseCanvasPosWx.x, mouseCanvasPosWx.y);

		//Get panel size
		wxSize panelSize = GetClientSize();

		const int maxCollisionTypesPerLayer = 16;

		float x = (m_orientation == eVertical) ? mousePosMapSpace.y : mousePosMapSpace.x;
		float y = (m_orientation == eVertical) ? mousePosMapSpace.x : mousePosMapSpace.y;
		float rectSize = (m_orientation == eVertical) ? (panelSize.y / maxCollisionTypesPerLayer) : (panelSize.x / maxCollisionTypesPerLayer);

		//Get current selection
		unsigned int collisionTypeId = (unsigned int)floor(x / rectSize);
		unsigned int collisionLayerId = (unsigned int)floor(y / rectSize);

		//TODO: Multiple layers
		const int numCollisionLayers = 1;

		if(collisionLayerId < numCollisionLayers && collisionTypeId < m_project->GetCollisionTypeCount())
		{
			if(CollisionType* collisionType = m_project->GetCollisionType(1 << collisionTypeId))
			{
				if(event.LeftDClick())
				{
					//TODO: Show type editor dialog (modal)
				}

				if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
				{
					//Set current collision type
					m_project->SetPaintCollisionType(collisionType);
				}
			}
		}
	}

	event.Skip();
}

void CollisionTypePanel::OnPaint(wxPaintEvent& event)
{
	//Double buffered dest dc
	wxAutoBufferedPaintDC destDC(this);

	//Get renderable client rect
	wxSize clientSize = GetClientSize();
	wxRect clientRect(0, 0, clientSize.x, clientSize.y);

	//Clear dest rect
	destDC.SetBrush(*wxBLACK_BRUSH);
	destDC.DrawRectangle(clientRect);

	//No outline
	destDC.SetPen(wxNullPen);

	const int maxCollisionTypesPerLayer = 16;

	//TODO: Multiple layers
	const int numCollisionLayers = 1;

	float rectSize = (m_orientation == eVertical) ? (clientSize.y / maxCollisionTypesPerLayer) : (clientSize.x / maxCollisionTypesPerLayer);

	for(int i = 0; i < numCollisionLayers; i++)
	{
		for(int j = 0; j < maxCollisionTypesPerLayer; j++)
		{
			int x = (m_orientation == eVertical) ? i : j;
			int y = (m_orientation == eVertical) ? j : i;

			wxBrush brush;
			CollisionType* collisionType = m_project->GetCollisionType(1 << j);

			if(collisionType && collisionType->iconData.size() > 0)
			{
				wxImage image(sIconWidth, sIconHeight, (unsigned char*)&collisionType->iconData[0], true);
				wxBitmap bitmap(image);
				destDC.DrawBitmap(bitmap, 0, 0);
			}
			else
			{
				brush.SetStyle(wxBRUSHSTYLE_CROSSDIAG_HATCH);
				brush.SetColour(wxColour(100, 100, 100, 50));
			}

			destDC.SetBrush(brush);
			destDC.DrawRectangle(x * rectSize, y * rectSize, rectSize, rectSize);
		}
	}
}

void CollisionTypePanel::OnErase(wxEraseEvent& event)
{
	//Ignore event
}

void CollisionTypePanel::OnResize(wxSizeEvent& event)
{
	if(m_project)
	{
		wxSize newSize = event.GetSize();

		const int maxCollisionTypesPerLayer = 16;

		if(newSize.x > newSize.y)
		{
			//Set new orientation
			m_orientation = eHorizontal;

			//Limit height
			int rectSize = (newSize.x / maxCollisionTypesPerLayer);
			SetMinSize(wxSize(1, rectSize * m_project->GetCollisionTypeCount()));
		}
		else
		{
			//Set new orientation
			m_orientation = eVertical;

			//Limit width
			int rectSize = (newSize.y / maxCollisionTypesPerLayer);
			SetMinSize(wxSize(rectSize * m_project->GetCollisionTypeCount(), 1));
		}
	}

	Refresh();
}