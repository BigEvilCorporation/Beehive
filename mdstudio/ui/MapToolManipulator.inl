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

template <typename T> MapToolManipulator<T>::MapToolManipulator(Project& project, MapPanel& mapPanel, TUndoStack& undoStack)
	: Tool(StaticType(), project, mapPanel, undoStack)
	, m_mapSelector(nullptr)
{
	AddContextMenuItem(0, "Delete selected", std::bind(&MapToolManipulator<T>::OnContextMenuDelete, this, std::placeholders::_1, std::placeholders::_2));
}

template <typename T> MapToolManipulator<T>::~MapToolManipulator()
{
	if(m_mapSelector)
		delete m_mapSelector;
}

template <typename T> void MapToolManipulator<T>::OnKeyboard(wxKeyEvent& event)
{
	m_mapSelector->OnKeyboard(event);
}

template <typename T> void MapToolManipulator<T>::OnMousePixelEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int tileX, int tileY)
{
	if(!m_mapSelector)
		m_mapSelector = new MapToolRegionSelector(m_project, GetUnitSizePx(), GetRegionSelectorFlags());

	//TODO: if gizmo not highlighted
	bool selectionChanged = m_mapSelector->OnMouse(mousePos, buttonBits);

	if (m_mapSelector->NeedsRedraw())
	{
		Redraw();
	}

	if (selectionChanged)
	{
		EnumerateSelection();
		SetupGizmo();
	}

	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);

	//Find first object under cursor
	std::vector<T> objects;
	MapRegion cursorRegion;
	cursorRegion.topLeft = m_mapSelector->GetCursorPos();
	cursorRegion.bottomRight = m_mapSelector->GetCursorPos();
	FindObjects(map, { cursorRegion }, objects);

	if (objects.size())
		m_objUnderCursor = objects[0];
	else
		m_objUnderCursor = T();

	if (m_objUnderCursor.IsValid() && buttonBits == 0)
		m_mapPanel.SetToolTip(GetTooltipText(m_objUnderCursor, mousePos).c_str());
	else
		m_mapPanel.UnsetToolTip();

	if (buttonBits & eMouseRight)
	{
		//Nothing selected? Take object under cursor
		if (m_selectedObjs.size() == 0 && m_objUnderCursor.IsValid())
			m_selectedObjs.push_back(m_objUnderCursor);

		//Right-click menu
		wxMenu contextMenu;

		for (int i = 0; i < m_contextMenuItems.size(); i++)
		{
			wxMenuItem* item = contextMenu.Append(i, wxString(m_contextMenuItems[i].name));
			if (m_selectedObjs.size() == 0)
				item->Enable(false);
		}

		contextMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&MapToolManipulator::OnContextMenuClick, NULL, this);
		m_mapPanel.PopupMenu(&contextMenu);
	}
}

template <typename T> void MapToolManipulator<T>::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	const ion::Vector2i mapSizePx = ion::Vector2i(map.GetWidth(), map.GetHeight()) * tileSize;

	m_mapSelector->OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
	m_gizmo.OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, mapSizePx);
	Tool::OnRender(renderer, renderResources, cameraInverseMtx, projectionMtx, z, zOffset);
}

template <typename T> void MapToolManipulator<T>::EnumerateSelection()
{
	Map& map = m_project.GetEditingMap();

	m_selectedObjs.clear();
	m_selectedBoundsPx.Clear();

	FindObjects(map, m_mapSelector->GetSelectedRegions(), m_selectedObjs);

	for (auto object : m_selectedObjs)
	{
		ion::maths::Bounds2i unitBounds = object.GetBounds();
		m_selectedBoundsPx.AddBounds(unitBounds * GetUnitSizePx());
	}
}

template <typename T> void MapToolManipulator<T>::SetupGizmo()
{
	Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	const ion::Vector2i mapSizePx = ion::Vector2i(map.GetWidth(), map.GetHeight()) * tileSize;

	if (m_selectedObjs.size() > 0)
	{
		ion::Vector2i position(m_selectedBoundsPx.GetCentre().x, mapSizePx.y - m_selectedBoundsPx.GetCentre().y);
		m_gizmo.SetPosition(position);
		m_gizmo.SetEnabled(true);
	}
	else
	{
		m_gizmo.SetEnabled(false);
	}
}

template <typename T> void MapToolManipulator<T>::AddContextMenuItem(int id, const std::string& name, std::function<void(int, std::vector<T>&)> const& callback)
{
	m_contextMenuItems.push_back({ name, id, callback });
}

template <typename T> void MapToolManipulator<T>::OnContextMenuClick(wxCommandEvent& event)
{
	ContextMenuItem& menuItem = m_contextMenuItems[event.GetId()];
	menuItem.callback(menuItem.id, m_selectedObjs);

	/*
	Map& map = m_project.GetEditingMap();

	if (event.GetId() == (int)ContextMenu::Delete)
	{
		DeleteObjects(m_selectedObjs);
		m_selectedObjs.clear();
		Redraw();
	}
	else if (event.GetId() == (int)ContextMenu::EditCollision)
	{
		//Show collision editor dialog
		if (m_selectedStamps.size())
		{
			for (auto stamp : m_selectedStamps)
			{
				m_mapPanel.EditStampCollisionDlg(*stamp.stamp);
			}
		}
		else if (m_cursor.stamp)
		{
			m_mapPanel.EditStampCollisionDlg(*m_cursor.stamp);
		}
	}
	*/
}

template <typename T> void MapToolManipulator<T>::OnContextMenuDelete(int id, std::vector<T>& objects)
{
	Map& map = m_project.GetEditingMap();
	DeleteObjects(map, objects);
	objects.clear();
	Redraw();
}