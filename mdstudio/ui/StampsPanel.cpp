///////////////////////////////////////////////////////
// Beehive: A complete SEGA Mega Drive content tool
//
// (c) 2015 Matt Phillips, Big Evil Corporation
///////////////////////////////////////////////////////

#include "StampsPanel.h"
#include "MainWindow.h"

StampsPanel::StampsPanel(MainWindow* mainWindow, ion::render::Renderer& renderer, wxGLContext* glContext, ion::render::Texture* tilesetTexture, wxWindow *parent, wxWindowID winid, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: ViewPanel(mainWindow, renderer, glContext, tilesetTexture, parent, winid, pos, size, style, name)
{
	m_selectedStamp = InvalidStampId;
	m_hoverStamp = InvalidStampId;

	//Custom zoom/pan handling
	EnableZoom(false);
	EnablePan(false);

	//Colours
	m_selectColour = ion::Colour(0.1f, 0.5f, 0.7f, 0.8f);
	m_hoverColour = ion::Colour(0.1f, 0.2f, 0.5f, 0.4f);

	//Load shaders
	m_selectionVertexShader = ion::render::Shader::Create();
	if(!m_selectionVertexShader->Load("shaders/flat_v.ion.shader"))
	{
		ion::debug::Error("Error loading vertex shader");
	}

	m_selectionPixelShader = ion::render::Shader::Create();
	if(!m_selectionPixelShader->Load("shaders/flat_p.ion.shader"))
	{
		ion::debug::Error("Error loading pixel shader");
	}

	//Create selection material
	m_selectionMaterial = new ion::render::Material();
	m_selectionMaterial->SetDiffuseColour(m_selectColour);
	m_selectionMaterial->SetVertexShader(m_selectionVertexShader);
	m_selectionMaterial->SetPixelShader(m_selectionPixelShader);

	//Create selection quad
	m_selectionPrimitive = new ion::render::Quad(ion::render::Quad::xy, ion::Vector2(4.0f, 4.0f));
}

StampsPanel::~StampsPanel()
{

}

void StampsPanel::OnMouse(wxMouseEvent& event, const ion::Vector2& mouseDelta)
{
	ViewPanel::OnMouse(event, mouseDelta);

	//Camera pan Y (if canvas is taller than panel)
	wxSize panelSize = GetClientSize();
	if((m_canvasSize.y * 8.0f) > (panelSize.y / m_cameraZoom))
	{
		float panDeltaY = 0.0f;

		if(event.Dragging() && event.ButtonIsDown(wxMOUSE_BTN_MIDDLE))
		{
			panDeltaY = mouseDelta.y;
		}
		else if(event.GetWheelRotation() != 0)
		{
			panDeltaY = (float)event.GetWheelRotation();
		}

		if(panDeltaY != 0.0f)
		{
			ion::Vector3 cameraPos = m_camera.GetPosition();
			cameraPos.y += panDeltaY * m_cameraPanSpeed / m_cameraZoom;

			//Clamp to size
			float halfCanvas = (m_canvasSize.y * 4.0f);
			float minY = -halfCanvas;
			float maxY = halfCanvas - ((float)panelSize.y / m_cameraZoom);

			if(cameraPos.y > maxY)
				cameraPos.y = maxY;
			else if(cameraPos.y < minY)
				cameraPos.y = minY;

			m_camera.SetPosition(cameraPos);

			Refresh();
		}
	}
}

void StampsPanel::OnKeyboard(wxKeyEvent& event)
{
	ViewPanel::OnKeyboard(event);
}

void StampsPanel::OnResize(wxSizeEvent& event)
{
	ViewPanel::OnResize(event);

	wxSize panelSize = event.GetSize();

	if(panelSize.x > 8 && panelSize.y > 8)
	{
		//Rearrange stamps (calculates canvas size)
		ArrangeStamps(ion::Vector2(panelSize.x, panelSize.y));

		//Recreate canvas
		CreateCanvas(m_canvasSize.x, m_canvasSize.y);

		//Fill with invalid tile
		FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasSize.x - 1, m_canvasSize.y - 1));

		//Redraw stamps on canvas
		PaintStamps();

		//Recreate grid
		CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / m_project->GetGridSize(), m_canvasSize.y / m_project->GetGridSize());

		//Reset zoom/pan
		ResetZoomPan();
	}
}

void StampsPanel::OnMouseTileEvent(ion::Vector2 mouseDelta, int buttonBits, int x, int y)
{
	ViewPanel::OnMouseTileEvent(mouseDelta, buttonBits, x, y);

	StampId selectedStamp = InvalidStampId;
	ion::Vector2i stampTopLeft;

	//If in range, get stamp under mouse cursor
	if(x >= 0 && y >= 0 && x < m_canvasSize.x && y < m_canvasSize.y)
	{
		//TODO: Per-tile stamp map
		//Brute force search for stamp ID
		ion::Vector2i mousePos(x, y);

		for(int i = 0; i < m_stampPosMap.size() && !selectedStamp; i++)
		{
			StampId stampId = m_stampPosMap[i].first;
			if(Stamp* stamp = m_project->GetStamp(stampId))
			{
				stampTopLeft = m_stampPosMap[i].second;
				const ion::Vector2i& stampBottomRight = stampTopLeft + ion::Vector2i(stamp->GetWidth(), stamp->GetHeight());
				if(mousePos.x >= stampTopLeft.x && mousePos.y >= stampTopLeft.y
					&& mousePos.x < stampBottomRight.x && mousePos.y < stampBottomRight.y)
				{
					selectedStamp = stampId;
				}
			}
		}
	}

	//Set mouse hover stamp
	m_hoverStamp = selectedStamp;
	m_hoverStampPos = stampTopLeft;

	if((buttonBits & eMouseLeft) && !(m_prevMouseBits & eMouseLeft))
	{
		//Left click, set current stamp
		m_selectedStamp = selectedStamp;
		m_selectedStampPos = stampTopLeft;

		//Set as current painting stamp
		m_project->SetPaintStamp(selectedStamp);

		//Set stamp paint tool
		m_mainWindow->SetMapTool(MapPanel::eToolPaintStamp);
	}

	//Redraw
	Refresh();
}

void StampsPanel::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render canvas
	RenderCanvas(renderer, cameraInverseMtx, projectionMtx, z);

	z += zOffset;

	//Render selected stamp
	if(m_selectedStamp)
	{
		Stamp* stamp = m_project->GetStamp(m_selectedStamp);
		ion::debug::Assert(stamp, "Invalid stamp");
		ion::Vector2 size(stamp->GetWidth(), stamp->GetHeight());
		RenderBox(m_selectedStampPos, size, m_selectColour, renderer, cameraInverseMtx, projectionMtx, z);
	}

	z += zOffset;

	//Render mouse hover stamp
	if(m_hoverStamp && m_hoverStamp != m_selectedStamp)
	{
		Stamp* stamp = m_project->GetStamp(m_hoverStamp);
		ion::debug::Assert(stamp, "Invalid stamp");
		ion::Vector2 size(stamp->GetWidth(), stamp->GetHeight());
		RenderBox(m_hoverStampPos, size, m_hoverColour, renderer, cameraInverseMtx, projectionMtx, z);
	}

	z += zOffset;

	//Render grid
	if(m_project->GetShowGrid())
	{
		RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	}
}

void StampsPanel::SetProject(Project* project)
{
	//Creates tileset texture
	ViewPanel::SetProject(project);

	//Refresh
	Refresh();
}

void StampsPanel::Refresh(bool eraseBackground, const wxRect *rect)
{
	if(m_project)
	{
		wxSize panelSize = GetClientSize();

		//If stamps invalidated
		if(m_project->StampsAreInvalidated() && panelSize.x > 8)
		{
			//Rearrange stamps (calculates canvas size)
			ArrangeStamps(ion::Vector2(panelSize.x, panelSize.y));

			//Recreate canvas
			CreateCanvas(m_canvasSize.x, m_canvasSize.y);

			//Fill with invalid tile
			FillTiles(InvalidTileId, ion::Vector2i(0, 0), ion::Vector2i(m_canvasSize.x - 1, m_canvasSize.y - 1));

			//Recreate grid
			CreateGrid(m_canvasSize.x, m_canvasSize.y, m_canvasSize.x / m_project->GetGridSize(), m_canvasSize.y / m_project->GetGridSize());

			//Redraw stamps on canvas
			PaintStamps();

			//Reset zoom/pan
			ResetZoomPan();
		}
	}

	ViewPanel::Refresh(eraseBackground, rect);
}

void StampsPanel::ArrangeStamps(const ion::Vector2& panelSize)
{
	//Fit canvas to panel
	m_canvasSize.x = ion::maths::Ceil(panelSize.x / 8.0f);
	m_canvasSize.y = ion::maths::Ceil(panelSize.y / 8.0f);

	//Clear stamp position map
	m_stampPosMap.clear();

	if(m_project)
	{
		//Find widest stamp first
		for(TStampMap::const_iterator it = m_project->StampsBegin(), end = m_project->StampsEnd(); it != end; ++it)
		{
			//If wider than current canvas width, grow canvas
			m_canvasSize.x = max(m_canvasSize.x, it->second.GetWidth());
		}

		ion::Vector2i currPos;
		int rowHeight = 1;

		for(TStampMap::const_iterator it = m_project->StampsBegin(), end = m_project->StampsEnd(); it != end; ++it)
		{
			const Stamp& stamp = it->second;
			ion::Vector2i stampSize(stamp.GetWidth(), stamp.GetHeight());
			ion::Vector2i stampPos;

			if(currPos.x + stampSize.x > m_canvasSize.x)
			{
				//Can't fit on this line, advance Y by tallest stamp on current row
				currPos.y += rowHeight;

				//Reset column
				currPos.x = stampSize.x;

				//Reset current row height
				rowHeight = stampSize.y;

				//Set stamp pos
				stampPos.x = 0;
				stampPos.y = currPos.y;
			}
			else
			{
				//Set stamp pos
				stampPos = currPos;

				//Next column
				currPos.x += stampSize.x;
			}

			//Record tallest stamp on current row
			rowHeight = max(rowHeight, stampSize.y);

			//If Y pos + height extends beyond canvas height, grow canvas
			m_canvasSize.y = max(m_canvasSize.y, currPos.y + rowHeight);

			//Add stamp to position map
			m_stampPosMap.push_back(std::make_pair(stamp.GetId(), stampPos));

			//If this is the currently selected stamp, update position
			if(stamp.GetId() == m_selectedStamp)
			{
				m_selectedStampPos = stampPos;
			}
		}
	}
}

void StampsPanel::PaintStamps()
{
	for(int i = 0; i < m_stampPosMap.size(); i++)
	{
		Stamp* stamp = m_project->GetStamp(m_stampPosMap[i].first);
		ion::debug::Assert(stamp, "Invalid stamp");
		PaintStamp(*stamp, m_stampPosMap[i].second.x, m_stampPosMap[i].second.y, 0);
	}
}

void StampsPanel::RenderBox(const ion::Vector2i& pos, const ion::Vector2& size, const ion::Colour& colour, ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	const int tileWidth = 8;
	const int tileHeight = 8;
	const int quadHalfExtentsX = 4;
	const int quadHalfExtentsY = 4;

	float bottom = (m_canvasSize.y - (pos.y + size.y));

	ion::Matrix4 boxMtx;
	ion::Vector3 boxScale(size.x, size.y, 0.0f);
	ion::Vector3 boxPos(floor((pos.x - (m_canvasSize.x / 2.0f) + (boxScale.x / 2.0f)) * tileWidth),
						floor((bottom - (m_canvasSize.y / 2.0f) + (boxScale.y / 2.0f)) * tileHeight), z);

	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	renderer.SetAlphaBlending(ion::render::Renderer::Translucent);
	m_selectionMaterial->SetDiffuseColour(colour);
	m_selectionMaterial->Bind(boxMtx, cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_selectionPrimitive->GetVertexBuffer(), m_selectionPrimitive->GetIndexBuffer());
	m_selectionMaterial->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::NoBlend);
}

void StampsPanel::ResetZoomPan()
{
	wxSize panelSize = GetClientSize();
	m_cameraZoom = (float)panelSize.x / (m_canvasSize.x * 8.0f);
	ion::Vector3 cameraPos(-(panelSize.x / 2.0f / m_cameraZoom), -(panelSize.y / 2.0f / m_cameraZoom), 0.0f);

	m_camera.SetZoom(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
	m_camera.SetPosition(cameraPos);
}