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

#include "ToolMapSelector.h"

MapSelector::MapSelector(Project& project, bool allowMultipleSelection, bool allowBoxSelection)
	: m_project(project)
	, m_allowMultipleSelection(allowMultipleSelection)
	, m_allowBoxSelection(allowBoxSelection)
	, m_inMultipleSelection(false)
	, m_inBoxSelection(false)
	, m_prevMouseBits(0)
	, m_needsRedraw(0)
{

}

void MapSelector::OnKeyboard(wxKeyEvent& event)
{
	m_inMultipleSelection = m_allowMultipleSelection && event.ControlDown();
}

void MapSelector::OnMouseTileEvent(ion::Vector2i mousePos, ion::Vector2i mouseDelta, ion::Vector2i tileDelta, int buttonBits, int x, int y)
{
	m_needsRedraw = false;
	Map& map = m_project.GetEditingMap();

	const int mapWidth = map.GetWidth();
	const int mapHeight = map.GetHeight();
	const int tileWidth = m_project.GetPlatformConfig().tileWidth;
	const int tileHeight = m_project.GetPlatformConfig().tileHeight;
	const ion::Vector2i coords(x, y);

	//Check in map range
	bool inMapRange = ((x >= 0) && (x < mapWidth) && (y >= 0) && (y < mapHeight));
	if (inMapRange)
	{
		m_hoverPos = coords;

		if (buttonBits & eMouseLeft)
		{
			m_needsRedraw = (coords != m_selectionStart) || (coords != m_selectionEnd);

			if (!m_inMultipleSelection)
				m_selections.clear();

			if (m_prevMouseBits & eMouseLeft)
			{
				m_selectionEnd = coords;
			}
			else
			{
				m_selectionStart = coords;
				m_selectionEnd = coords;
			}
			
			//If dragged > 1 tile, start box selection
			m_inBoxSelection = m_allowBoxSelection && (coords != m_selectionStart);
		}
		else if(m_prevMouseBits & eMouseLeft)
		{
			//Add tile range
			ion::Vector2i topLeft;
			ion::Vector2i bottomRight;
			SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);
			m_selections.push_back(MapSelection(topLeft, bottomRight));

			//Reset current selection
			m_selectionStart = ion::Vector2i(-1, -1);
			m_selectionEnd = ion::Vector2i(-1, -1);
			m_inBoxSelection = false;
			m_needsRedraw = true;
		}
	}

	m_prevMouseBits = buttonBits;
}

void MapSelector::OnRender(ion::render::Renderer& renderer, RenderResources& renderResources, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	const Map& map = m_project.GetEditingMap();
	const float tileWidth = m_project.GetPlatformConfig().tileWidth;
	const float tileHeight = m_project.GetPlatformConfig().tileHeight;
	const float mapWidth = map.GetWidth() * tileWidth;
	const float mapHeight = map.GetHeight() * tileHeight;
	const ion::Vector2i tileScale(tileWidth, tileHeight);
	const ion::Vector2i vecOne(1, 1);

	ion::render::Material* material = renderResources.GetMaterial(RenderResources::eMaterialFlatColour);
	const ion::Colour& colour = renderResources.GetColour(RenderResources::eColourSelected);
	ion::render::Primitive* primitive = renderResources.GetPrimitive(RenderResources::ePrimitiveUnitQuad);
	material->SetDiffuseColour(colour);

	ion::render::Shader* shader = renderResources.GetShader(RenderResources::eShaderFlatColour);
	ion::render::Shader::ParamHndl<ion::Matrix4> worldViewProjParam = shader->CreateParamHndl<ion::Matrix4>("gWorldViewProjectionMatrix");

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::Translucent);
	renderer.BindMaterial(*material, ion::Matrix4(), cameraInverseMtx, projectionMtx);

	//Draw current region
	if (m_selectionStart.x != -1)
	{
		ion::Vector2i topLeft;
		ion::Vector2i bottomRight;
		SanitiseBoxOrder(m_selectionStart, m_selectionEnd, topLeft, bottomRight);

		ion::Matrix4 worldViewProjMtx = CalcBoxDrawMatrix(topLeft * tileScale, (bottomRight + vecOne) * tileScale, mapWidth, mapHeight, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	}

	//Draw selected regions
	for (auto selection : m_selections)
	{
		ion::Matrix4 worldViewProjMtx = CalcBoxDrawMatrix(selection.topLeft * tileScale, (selection.bottomRight + vecOne) * tileScale, mapWidth, mapHeight, z) * cameraInverseMtx * projectionMtx;
		worldViewProjParam.SetValue(worldViewProjMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
	}

	renderer.SetAlphaBlending(ion::render::Renderer::AlphaBlendType::None);
	renderer.UnbindMaterial(*material);

	m_needsRedraw = false;
}

void MapSelector::SanitiseBoxOrder(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, ion::Vector2i& topLeft, ion::Vector2i& bottomRight)
{
	topLeft.x = ion::maths::Min(boxStart.x, boxEnd.x);
	topLeft.y = ion::maths::Min(boxStart.y, boxEnd.y);
	bottomRight.x = ion::maths::Max(boxStart.x, boxEnd.x);
	bottomRight.y = ion::maths::Max(boxStart.y, boxEnd.y);
}

ion::Matrix4 MapSelector::CalcBoxDrawMatrix(const ion::Vector2i& boxStart, const ion::Vector2i& boxEnd, float mapWidth, float mapHeight, float z)
{
	const float x = ion::maths::Min(boxStart.x, boxEnd.x);
	const float y = ion::maths::Min(boxStart.y, boxEnd.y);
	const float y_inv = mapHeight - 1 - y;
	const float width = (float)abs(boxEnd.x - boxStart.x);
	const float height = (float)abs(boxEnd.y - boxStart.y);
	const ion::Vector2 mapCentre(mapWidth / 2.0f, mapHeight / 2.0f);
	const ion::Vector2 boxCentre(width / 2.0f, height / 2.0f);

	ion::Vector3 boxScale(width, height, 0.0f);
	ion::Vector3 boxPos(floor((x - mapCentre.x + boxCentre.x)), floor((y_inv - mapCentre.y - boxCentre.y)), z);

	ion::Matrix4 boxMtx;
	boxMtx.SetTranslation(boxPos);
	boxMtx.SetScale(boxScale);

	return boxMtx;
}
