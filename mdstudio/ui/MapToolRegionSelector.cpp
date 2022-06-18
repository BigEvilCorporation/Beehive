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
#include "MapDrawTools.h"

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
				drawtools::SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);
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

void MapToolRegionSelector::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, const ion::Vector2i& selectionOffsetUnit, float& z, float zOffset)
{
	const Map& map = m_project.GetEditingMap();
	const ion::Vector2i tileSizePx(m_project.GetPlatformConfig().tileWidth, m_project.GetPlatformConfig().tileHeight);
	const ion::Vector2i mapSizePx = (ion::Vector2i(map.GetWidth(), map.GetHeight()) * tileSizePx);
	const ion::Vector2i vecOne(1, 1);

	ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& cursorColour = renderResources.GetColour(RenderResources::eColourHighlight);
	const ion::Colour& selectedColour = renderResources.GetColour(RenderResources::eColourSelected);
	const ion::Colour& outlineColour = renderResources.GetColour(RenderResources::eColourOutline);
	ion::render::Primitive* primitiveBox = renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);
	ion::render::Primitive* primitiveOutline = renderResources.GetPrimitive(RenderResources::ePrimitiveUnitLineQuad);

	ion::render::Shader* shader = renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParam = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");
	ion::render::Shader::ParamHndl<ion::Colour> diffuseParam = shader->CreateParamHndl<ion::Colour>("gDiffuseColour");

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);

	//Draw cursor
	if ((m_flags & Flags::DrawCursor) && m_cursorPos.x != -1)
	{
		//Draw on top of grid
		const float outlineZOffset = 0.1f;
		ion::Matrix4 worldViewProjMtx = drawtools::CalcBoxDrawMatrix(m_cursorPos * m_unitScale, (m_cursorPos + vecOne) * m_unitScale, mapSizePx, z + outlineZOffset) * cameraInverseMtx * projectionMtx;

		//Outline
		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
		worldViewProjParam.SetValue(worldViewProjMtx);
		diffuseParam.SetValue(outlineColour);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(primitiveOutline->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	}

	//Draw current region
	if (m_selectionStart.x != -1)
	{
		ion::Vector2i topLeft;
		ion::Vector2i bottomRight;
		drawtools::SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);

		ion::Matrix4 worldViewProjMtx = drawtools::CalcBoxDrawMatrix(topLeft * m_unitScale, (bottomRight + vecOne) * m_unitScale, mapSizePx, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		diffuseParam.SetValue(selectedColour);
		renderer.DrawVertexBuffer(primitiveBox->GetVertexBuffer(), primitiveBox->GetIndexBuffer());
	}

	//Draw selected regions
	for (auto selection : m_selections)
	{
		ion::Matrix4 worldViewProjMtx = drawtools::CalcBoxDrawMatrix((selection.topLeft + selectionOffsetUnit) * m_unitScale, (selection.bottomRight + selectionOffsetUnit + vecOne) * m_unitScale, mapSizePx, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		diffuseParam.SetValue(selectedColour);
		renderer.DrawVertexBuffer(primitiveBox->GetVertexBuffer(), primitiveBox->GetIndexBuffer());
	}

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	renderer.UnbindMaterial(*material);

	m_needsRedraw = false;
}
