///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "CollisionTypePanel.h"
#include "MainWindow.h"
#include <wx/App.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <maths/Vector.h>

//Default icon
#include "../FormBuilderProj/qmark_16_16.xpm"

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
	Bind(wxEVT_MOTION,			&CollisionTypePanel::OnMouse, this, GetId());
	Bind(wxEVT_PAINT,			&CollisionTypePanel::OnPaint, this, GetId());
	Bind(wxEVT_ERASE_BACKGROUND,&CollisionTypePanel::OnErase, this, GetId());
	Bind(wxEVT_SIZE,			&CollisionTypePanel::OnResize, this, GetId());

	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void CollisionTypePanel::SetProject(Project* project)
{
	m_project = project;

	//Refresh icons
	m_icons.clear();

	if(m_project)
	{
		m_icons.resize(m_project->GetCollisionTypeCount() + 1);

		for(TCollisionTypeMap::const_iterator it = m_project->CollisionTypesBegin(), end = m_project->CollisionTypesEnd(); it != end; ++it)
		{
			wxImage image(sIconWidth, sIconHeight, (unsigned char*)&it->second.iconData[0], true);

			int index = -1;

			//Cheap and lazy bit scan
			for(int i = 0; i < sizeof(u8) * 8 && index < 0; i++)
			{
				if((1 << i) & it->second.bit)
				{
					index = i;
				}
			}

			m_icons[index] = wxBitmap(image);
		}
	}
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
		unsigned int collisionTypeIndex = (unsigned int)floor(x / rectSize);
		unsigned int collisionLayerId = (unsigned int)floor(y / rectSize);

		//TODO: Multiple layers
		const int numCollisionLayers = 1;

		int iconX = (m_orientation == eVertical) ? collisionTypeIndex : collisionLayerId;
		int iconY = (m_orientation == eVertical) ? collisionLayerId : collisionTypeIndex;
		wxRect rect(iconX * rectSize, iconY * rectSize, rectSize, rectSize);

		u32 collisionBit = 1 << collisionTypeIndex;

		if(collisionLayerId < numCollisionLayers && collisionTypeIndex < maxCollisionTypesPerLayer)
		{
			CollisionType* collisionType = m_project->GetCollisionType(collisionBit);

			if(event.LeftDClick())
			{
				//Create new
				if(!collisionType)
				{
					collisionType = m_project->AddCollisionType(collisionBit);
				}

				//Set default image
				wxImage image(qmark_16_16_xpm);

				//Open image file
				wxFileDialog dialogue(this, _("Open Image file"), "", "", "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
				if(dialogue.ShowModal() == wxID_OK)
				{
					if(image.LoadFile(dialogue.GetPath()))
					{
						if(image.GetWidth() != sIconWidth || image.GetHeight() != sIconHeight)
						{
							wxMessageBox("Image must be 16x16", "Error", wxOK | wxICON_ERROR);
						}
					}
				}

				//Set icon data
				const int bytesPerPixel = 3;
				int imageSize = sIconWidth * sIconHeight * bytesPerPixel;
				collisionType->iconData.resize(imageSize);
				unsigned char* imageData = image.GetData();
				ion::memory::MemCopy(&collisionType->iconData[0], imageData, imageSize);

				//Create bitmap for rendering
				if(m_icons.size() < collisionTypeIndex + 1)
					m_icons.resize(collisionTypeIndex + 1);
				m_icons[collisionTypeIndex] = wxBitmap(image, wxBITMAP_SCREEN_DEPTH);

				//Recreate collision types texture
				m_mainWindow->RefreshCollisionTypes();

				//Invalidate collision types
				m_project->InvalidateCollisionTypes(true);

				//Refresh this panel
				Refresh();

				//Refresh collision tile editor panel
				m_mainWindow->RefreshPanel(MainWindow::ePanelTerrainTileEditor);
			}

			if(collisionTypeIndex < m_project->GetCollisionTypeCount())
			{
				if(CollisionType* collisionType = m_project->GetCollisionType(1 << collisionTypeIndex))
				{
					if(!m_tooltip.get())
					{
						//m_tooltip = new wxTipWindow(this, "Test Tooltip", 100, NULL, &rect);
						//m_tooltip->Show();
					}

					if(event.ButtonIsDown(wxMOUSE_BTN_LEFT))
					{
						//Set current collision type
						m_project->SetPaintCollisionType(collisionType);
					}
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

	wxBitmap bitmap(sIconWidth, sIconHeight, wxBITMAP_SCREEN_DEPTH);
	wxMemoryDC bitmapDC(bitmap);

	for(int i = 0; i < numCollisionLayers; i++)
	{
		for(int j = 0; j < maxCollisionTypesPerLayer; j++)
		{
			int x = (m_orientation == eVertical) ? i : j;
			int y = (m_orientation == eVertical) ? j : i;

			wxBrush brush;
			CollisionType* collisionType = m_project->GetCollisionType(1 << j);
			if(collisionType)
			{
				bitmapDC.DrawBitmap(m_icons[(i * maxCollisionTypesPerLayer) + j], 0, 0);
				destDC.StretchBlit(x * rectSize, y * rectSize, rectSize, rectSize, &bitmapDC, 0, 0, sIconWidth, sIconHeight);
			}
			else
			{
				brush.SetStyle(wxBRUSHSTYLE_CROSSDIAG_HATCH);
				brush.SetColour(wxColour(100, 100, 100, 50));
				destDC.SetBrush(brush);
				destDC.DrawRectangle(x * rectSize, y * rectSize, rectSize, rectSize);
			}
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