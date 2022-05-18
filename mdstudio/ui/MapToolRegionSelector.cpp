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

#include "MapToolRegionSelector.h"

MapToolRegionSelector::MapToolRegionSelector(Project& project, const ion::Vector2i& unitScale, int flags)
	: m_project(project)
	, m_unitScale(unitScale)
	, m_flags(flags)
	, m_inMultipleSelection(false)
	, m_inBoxSelection(false)
	, m_prevMouseBits(0)
	, m_needsRedraw(0)
	, m_selectionStart(-1, -1)
	, m_selectionEnd(-1, -1)
{

}

bool MapToolRegionSelector::OnKeyboard(wxKeyEvent& event)
{
	m_inMultipleSelection = (m_flags & Flags::AllowMultipleSelection) && event.ControlDown();
	return false;
}

bool MapToolRegionSelector::OnMouse(ion::Vector2i mousePos, int buttonBits)
{
	bool selectionChanged = false;
	m_needsRedraw = false;
	m_cursorPos = ion::Vector2i(-1, -1);

	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSize(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	const ion::Vector2i mapSizePx = ion::Vector2i(map.GetWidth(), map.GetHeight()) * tileSize;
	const ion::Vector2i coords = mousePos / m_unitScale;

	//If first click and not in multiple selection, clear
	if ((buttonBits & eMouseLeft) && !m_inMultipleSelection && m_selections.size())
	{
		m_selections.clear();
		selectionChanged = true;
		m_needsRedraw = true;
	}

	if ((m_flags & Flags::DrawCursor) && coords != m_cursorPos)
	{
		m_needsRedraw = true;
	}

	//Check in map range
	bool inMapRange = ((mousePos.x >= 0) && (mousePos.x < mapSizePx.x) && (mousePos.y >= 0) && (mousePos.y < mapSizePx.y));
	if (inMapRange)
	{
		m_cursorPos = coords;

		if (buttonBits & eMouseLeft)
		{
			m_needsRedraw |= (coords != m_selectionStart) || (coords != m_selectionEnd);

			if (m_prevMouseBits & eMouseLeft)
			{
				//If selection started outside map region, update selection start now
				if (m_inMultipleSelection && (m_flags & Flags::AllowBoxSelection))
				{
					if (m_selectionStart.x == -1)
						m_selectionStart = coords;
					else
						m_selectionEnd = coords;
				}
			}
			else
			{
				//First click, start selection
				m_selectionStart = coords;
				m_selectionEnd = coords;
			}
			
			//If dragged > 1 tile, start box selection
			m_inBoxSelection = m_inMultipleSelection && (m_flags & Flags::AllowBoxSelection) && (coords != m_selectionStart);
		}
		else if(m_prevMouseBits & eMouseLeft)
		{
			if (m_selectionEnd.x != -1)
			{
				//Add region
				ion::Vector2i topLeft;
				ion::Vector2i bottomRight;
				SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);
				m_selections.push_back(MapRegion(topLeft, bottomRight));

				//Reset current selection
				m_selectionStart = ion::Vector2i(-1, -1);
				m_selectionEnd = ion::Vector2i(-1, -1);
				m_inBoxSelection = false;
				selectionChanged = true;
				m_needsRedraw = true;
			}
		}

		m_prevMouseBits = buttonBits;
	}

	return selectionChanged;
}

void MapToolRegionSelector::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSizePx(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	const ion::Vector2i mapSizePx = (ion::Vector2i(map.GetWidth(), map.GetHeight()) * tileSizePx);
	const ion::Vector2i vecOne(1, 1);

	ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& cursorColour = renderResources.GetColour(RenderResources::eColourHighlight);
	const ion::Colour& selectedColour = renderResources.GetColour(RenderResources::eColourSelected);
	ion::render::Primitive* primitive = renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);

	ion::render::Shader* shader = renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParam = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);

	//Draw cursor
	if ((m_flags & Flags::DrawCursor) && m_cursorPos.x != -1)
	{
		ion::Matrix4 worldViewProjMtx = CalcBoxDrawMatrix(m_cursorPos * m_unitScale, (m_cursorPos + vecOne) * m_unitScale, mapSizePx, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		material->SetDiffuseColour(cursorColour);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	}

	//Draw current region
	if (m_selectionStart.x != -1)
	{
		ion::Vector2i topLeft;
		ion::Vector2i bottomRight;
		SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);

		ion::Matrix4 worldViewProjMtx = CalcBoxDrawMatrix(topLeft * m_unitScale, (bottomRight + vecOne) * m_unitScale, mapSizePx, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		material->SetDiffuseColour(selectedColour);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	}

	//Draw selected regions
	for (auto selection : m_selections)
	{
		ion::Matrix4 worldViewProjMtx = CalcBoxDrawMatrix(selection.topLeft * m_unitScale, (selection.bottomRight + vecOne) * m_unitScale, mapSizePx, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		material->SetDiffuseColour(selectedColour);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	}

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	renderer.UnbindMaterial(*material);

	m_needsRedraw = false;
}

void MapToolRegionSelector::SanitiseBoxOrder(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, ion::Vector2i& topLeft, ion::Vector2i& bottomRight)
{
	topLeft.x = ion::maths::Min(boxStart.x, boxEnd.x);
	topLeft.y = ion::maths::Min(boxStart.y, boxEnd.y);
	bottomRight.x = ion::maths::Max(boxStart.x, boxEnd.x);
	bottomRight.y = ion::maths::Max(boxStart.y, boxEnd.y);
}

ion::Matrix4 MapToolRegionSelector::CalcBoxDrawMatrix(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, const ion::Vector2i& mapSizePx, float z)
{
	const float x = ion::maths::Min(boxStart.x, boxEnd.x);
	const float y = ion::maths::Min(boxStart.y, boxEnd.y);
	const float y_inv = mapSizePx.y - y;
	const float width = (float)abs(boxEnd.x - boxStart.x);
	const float height = (float)abs(boxEnd.y - boxStart.y);
	const ion::Vector2 mapCentre(mapSizePx.x / 2.0f, mapSizePx.y / 2.0f);
	const ion::Vector2 boxCentre(width / 2.0f, height / 2.0f);

	ion::Vector3 boxScale(width, height, 0.0f);
	ion::Vector3 boxPos(x - mapCentre.x + boxCentre.x, y_inv - mapCentre.y - boxCentre.y, z);

	ion::Matrix4 boxMtx;
	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	return boxMtx;
}
