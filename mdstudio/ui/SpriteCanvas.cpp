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

#include "SpriteCanvas.h"

SpriteCanvas::SpriteCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, NULL, pos, size, style, name)
	, m_viewport(128, 128, ion::render::Viewport::eOrtho2DAbsolute)
{
	m_gridPrimitive = NULL;
	m_boundsPrimitive = NULL;
	m_cameraZoom = 1.0f;
	m_gridColour = ion::Colour(0.0f, 0.0f, 0.0f, 1.0f);
	m_drawPreview = false;
	m_drawPreviewMaxFrames = 0;
	m_drawGrid = false;
	m_drawSpriteSheet = InvalidSpriteSheetId;
	m_drawSpriteSheetFrame = 0;
	m_tileFramePrimitive = NULL;

	m_terrainCanvasPrimitive = NULL;
	m_primitiveBezierPoints = NULL;
	m_primitiveBezierHandles = NULL;
	m_highlightedBezier = NULL;
	m_currentBezier = NULL;
	m_currentBezierControlIdx = -1;

	//Set viewport clear colour
	m_viewport.SetClearColour(ion::Colour(0.3f, 0.3f, 0.3f));

	//Subscribe to events
	Bind(wxEVT_PAINT, &SpriteCanvas::EventHandlerPaint, this, GetId());
	Bind(wxEVT_SIZE, &SpriteCanvas::EventHandlerResize, this, GetId());
	Bind(wxEVT_MOUSEWHEEL, &SpriteCanvas::EventHandlerMouse, this, GetId());
}

SpriteCanvas::~SpriteCanvas()
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;
	if(m_boundsPrimitive)
		delete m_boundsPrimitive;
	if (m_primitiveBezierPoints)
		delete m_primitiveBezierPoints;
	if (m_primitiveBezierHandles)
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
}

void SpriteCanvas::SetupRendering(ion::render::Renderer* renderer, wxGLContext* glContext, RenderResources* renderResources)
{
	m_renderer = renderer;
	m_renderResources = renderResources;
	SetCurrent(*glContext);
}

void SpriteCanvas::SetSpriteSheetDimentionsPixels(const ion::Vector2i& spriteSheetDimentions)
{
	m_spriteSheetDimentions = spriteSheetDimentions;

	CreateGrid(m_spriteSheetDimentions.x, m_spriteSheetDimentions.y, m_spriteSheetDimentionsCells.x, m_spriteSheetDimentionsCells.y);

	Refresh();
}

void SpriteCanvas::SetSpriteSheetDimentionsCells(const ion::Vector2i& spriteSheetDimentionsCells)
{
	m_spriteSheetDimentionsCells = spriteSheetDimentionsCells;

	CreateGrid(m_spriteSheetDimentions.x, m_spriteSheetDimentions.y, m_spriteSheetDimentionsCells.x, m_spriteSheetDimentionsCells.y);

	Refresh();
}

void SpriteCanvas::SetGridColour(const ion::Colour& colour)
{
	m_gridColour = colour;
	Refresh();
}

void SpriteCanvas::SetDrawPreview(bool drawPreview, u32 maxFrames)
{
	m_drawPreview = drawPreview;
	m_drawPreviewMaxFrames = maxFrames;
	Refresh();
}

void SpriteCanvas::SetDrawGrid(bool drawGrid)
{
	m_drawGrid = drawGrid;
	Refresh();
}

void SpriteCanvas::SetDrawSpriteSheet(SpriteSheetId spriteSheet, u32 frame, const ion::Vector2i& size, const ion::Vector2i& offset, const ion::Vector2i& topLeft, const ion::Vector2i& bottomRight)
{
	m_drawSpriteSheet = spriteSheet;
	m_drawSpriteSheetFrame = frame;
	m_drawOffset = offset;
	m_topLeft = topLeft;
	m_bottomRight = bottomRight;

	if(m_boundsPrimitive)
		delete m_boundsPrimitive;

	ion::Vector2 boundsHalfExtents((float)(bottomRight.x - topLeft.x) / 2.0f, (float)(bottomRight.y - topLeft.y) / 2.0f);
	ion::Vector2 boundsOffset((float)((-size.x / 2.0f) + boundsHalfExtents.x + topLeft.x), (float)((size.y / 2.0f) - boundsHalfExtents.y - topLeft.y));
	m_boundsPrimitive = new ion::render::LineQuad(ion::render::LineQuad::xy, boundsHalfExtents, boundsOffset);

	Refresh();
}

void SpriteCanvas::SetDrawStamp(Stamp& stamp, const ion::Vector2i& offset)
{
	m_drawOffset = offset;

	if(m_tileFramePrimitive)
		delete m_tileFramePrimitive;
	if (m_terrainCanvasPrimitive)
		delete m_terrainCanvasPrimitive;

	const int tileWidth = m_project->GetPlatformConfig().tileWidth;
	const int tileHeight = m_project->GetPlatformConfig().tileHeight;
	const int width = stamp.GetWidth();
	const int height = stamp.GetHeight();

	m_canvasSize.x = width;
	m_canvasSize.y = height;

	m_tileFramePrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)width * (tileWidth / 2.0f), (float)height * (tileHeight / 2.0f)), width, height, true);
	m_terrainCanvasPrimitive = new ion::render::Chessboard(ion::render::Chessboard::xy, ion::Vector2((float)(width * tileWidth) / 2.0f, (float)(height * tileHeight) / 2.0f), width, height, true);

	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			TileId tileId = stamp.GetTile(x, y);
			int y_inv = height - 1 - y;

			//Set texture coords for cell
			ion::render::TexCoord coords[4];
			m_renderResources->GetTileTexCoords(tileId, coords, 0);
			m_tileFramePrimitive->SetTexCoords((y_inv * width) + x, coords);
		}
	}

	Refresh();
}

void SpriteCanvas::Refresh(bool eraseBackground, const wxRect *rect)
{
	wxGLCanvas::Refresh(eraseBackground, rect);
}

void SpriteCanvas::CreateGrid(int width, int height, int cellsX, int cellsY)
{
	if(m_gridPrimitive)
		delete m_gridPrimitive;

	m_gridPrimitive = new ion::render::Grid(ion::render::Grid::xy, ion::Vector2((float)width / 2, (float)height / 2), cellsX, cellsY);
}

void SpriteCanvas::OnResize(wxSizeEvent& event)
{
	wxSize clientSize = event.GetSize();
	m_panelSize.x = clientSize.x;
	m_panelSize.y = clientSize.y;

	if(m_panelSize.x > 1 && m_panelSize.y > 1)
	{
		//Filter out superflous resize events (wx sends them if UI thread doesn't respond during saving/loading)
		if(m_prevPanelSize.x != m_panelSize.x || m_prevPanelSize.y != m_panelSize.y)
		{
			m_prevPanelSize = m_panelSize;
			m_viewport.Resize(m_panelSize.x, m_panelSize.y);
			
			//Centre camera
			ion::Vector3 cameraPos(-(m_panelSize.x / 2.0f), -(m_panelSize.y / 2.0f), 0.0f);
			m_camera.SetPosition(cameraPos);
		}
	}

	Refresh();
}

void SpriteCanvas::OnMouse(wxMouseEvent& event, const ion::Vector2i& mouseDelta)
{
	if(event.GetWheelRotation() != 0)
	{
		//Zoom camera
		int wheelDelta = event.GetWheelRotation();
		float zoomSpeed = 1.0f;

		//Reduce speed for <1.0f
		if((wheelDelta < 0 && m_cameraZoom <= 1.0f) || (wheelDelta > 0 && m_cameraZoom < 1.0f))
		{
			zoomSpeed = 0.2f;
		}

		//One notch at a time
		if(wheelDelta > 0)
			m_cameraZoom += zoomSpeed;
		else if(wheelDelta < 0)
			m_cameraZoom -= zoomSpeed;

		m_cameraZoom = ion::maths::Clamp(m_cameraZoom, 0.1f, 10.0f);

		Refresh();
	}
}

void SpriteCanvas::OnRender(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float& z, float zOffset)
{
	//Render preview
	RenderPreview(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render SpriteSheet
	RenderSpriteSheet(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render tile frame
	RenderTileFrame(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render grid
	RenderGrid(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;

	//Render bounding box
	RenderBounds(renderer, cameraInverseMtx, projectionMtx, z);
	z += zOffset;
}

void SpriteCanvas::RenderSpriteSheet(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_drawSpriteSheet != InvalidSpriteSheetId)
	{
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Render spriteSheet
		RenderResources::SpriteSheetRenderResources* spriteSheetResources = m_renderResources->GetSpriteSheetResources(m_drawSpriteSheet);
		ion::debug::Assert(spriteSheetResources, "SpriteCanvas::RenderSpriteSheet() - Missing spriteSheet render resources");
		ion::debug::Assert(spriteSheetResources->m_frames.size() > m_drawSpriteSheetFrame, "SpriteCanvas::RenderSpriteSheet() - Invalid frame");

		ion::render::Primitive* primitive = spriteSheetResources->m_primitive;
		ion::render::Material* material = spriteSheetResources->m_frames[m_drawSpriteSheetFrame].material;
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

		ion::Matrix4 boxMtx;
		ion::Vector3 boxScale(m_cameraZoom, m_cameraZoom, 0.0f);
		ion::Vector3 boxPos(m_drawOffset.x * m_cameraZoom, m_drawOffset.y * m_cameraZoom, z);

		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderTileFrame(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_tileFramePrimitive)
	{
		const int tileWidth = m_project->GetPlatformConfig().tileWidth;
		const int tileHeight = m_project->GetPlatformConfig().tileHeight;

		//Render spriteSheet
		ion::render::Primitive* primitive = m_tileFramePrimitive;
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialTileset);
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));

		ion::Matrix4 boxMtx;
		ion::Vector3 boxScale(m_cameraZoom, m_cameraZoom, 0.0f);
		ion::Vector3 boxPos(m_drawOffset.x * m_cameraZoom, m_drawOffset.y * m_cameraZoom, z);

		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderPreview(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_drawPreview)
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialSpriteSheet);
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveUnitQuad);

		ion::Matrix4 boxMtx;
		ion::Vector3 boxScale(m_spriteSheetDimentions.x * m_cameraZoom, m_spriteSheetDimentions.y * m_cameraZoom, 0.0f);
		ion::Vector3 boxPos(0.0f, 0.0f, z);

		boxMtx.SetTranslation(boxPos);
		boxMtx.SetScale(boxScale);

		material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		if(m_drawPreviewMaxFrames > 0)
		{
			//Grey out invalid frames
			material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);
			material->SetDiffuseColour(ion::Colour(0.3f, 0.3f, 0.3f));

			int lastFrame = m_spriteSheetDimentionsCells.x * m_spriteSheetDimentionsCells.y;
			int firstFrame = m_drawPreviewMaxFrames;

			float frameWidth = (m_spriteSheetDimentions.x / m_spriteSheetDimentionsCells.x) * m_cameraZoom;
			float frameHeight = (m_spriteSheetDimentions.y / m_spriteSheetDimentionsCells.y) * m_cameraZoom;
			float sheetWidth = m_spriteSheetDimentions.x * m_cameraZoom;
			float sheetHeight = m_spriteSheetDimentions.y * m_cameraZoom;

			for(int i = firstFrame; i < lastFrame; i++)
			{
				int frameX = m_spriteSheetDimentionsCells.x - 1 - (i % m_spriteSheetDimentionsCells.x);
				int frameY = i / m_spriteSheetDimentionsCells.x;

				boxScale = ion::Vector3(frameWidth, frameHeight, 0.0f);
				boxPos = ion::Vector3((sheetWidth / 2.0f) - (frameWidth / 2.0f) - (frameX * frameWidth), (sheetHeight / 2.0f) - (frameHeight / 2.0f) - (frameY * frameHeight), z);

				boxMtx.SetTranslation(boxPos);
				boxMtx.SetScale(boxScale);

				material->Bind(boxMtx, cameraInverseMtx, projectionMtx);
				renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
				material->Unbind();
			}
		}
	}
}

void SpriteCanvas::RenderGrid(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_gridPrimitive && m_drawGrid)
	{
		//Draw grid
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);

		ion::Matrix4 gridMtx;
		gridMtx.SetTranslation(ion::Vector3(0.0f, 0.0f, z));
		gridMtx.SetScale(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));
		material->SetDiffuseColour(m_gridColour);
		material->Bind(gridMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_gridPrimitive->GetVertexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderBounds(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	if(m_boundsPrimitive)
	{
		ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourOutline);

		ion::Matrix4 outlineMtx;
		ion::Vector2i size = (m_bottomRight - m_topLeft);
		outlineMtx.SetTranslation(ion::Vector3(m_drawOffset.x * m_cameraZoom, m_drawOffset.y * m_cameraZoom, z));
		outlineMtx.SetScale(ion::Vector3(m_cameraZoom, m_cameraZoom, 1.0f));

		material->SetDiffuseColour(colour);
		material->Bind(outlineMtx, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(m_boundsPrimitive->GetVertexBuffer());
		material->Unbind();
	}
}

void SpriteCanvas::RenderCollisionBeziers(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialFlatColour);

	const Map& map = m_project->GetEditingMap();
	const float mapWidth = map.GetWidth();
	const float mapHeight = map.GetHeight();
	const float tileWidth = m_project->GetPlatformConfig().tileWidth;
	const float tileHeight = m_project->GetPlatformConfig().tileHeight;

	ion::Matrix4 bezierMatrix;
	bezierMatrix.SetTranslation(ion::Vector3(-(mapWidth * tileWidth) / 2.0f, -(mapHeight * tileHeight) / 2.0f, z));

	//Draw curves
	if (m_primitiveBeziers.size() > 0)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.4f, 0.4f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(3.0f);

		for (int i = 0; i < m_primitiveBeziers.size(); i++)
		{
			renderer.DrawVertexBuffer(m_primitiveBeziers[i]->GetVertexBuffer());
		}

		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve points
	if (m_primitiveBezierPoints)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierPoints->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw curve control handles
	if (m_primitiveBezierHandles)
	{
		material->SetDiffuseColour(ion::Colour(1.0f, 0.6f, 0.6f, 1.0f));
		material->Bind(bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.SetLineWidth(2.0f);
		renderer.DrawVertexBuffer(m_primitiveBezierHandles->GetVertexBuffer());
		renderer.SetLineWidth(1.0f);
		material->Unbind();
	}

	//Draw selected handle
	if (m_currentBezierControlIdx != -1)
	{
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);

		const float x = m_currentBezierControlPos.x;
		const float y = m_currentBezierControlPos.y;
		const float width = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;
		const float height = m_currentBezierControlHndl == eBezierPosition ? 1.0f : 0.5f;

		ion::Vector3 previewScale(width, height, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	//Draw selected bezier
	if (m_highlightedBezier)
	{
		ion::render::Primitive* primitive = m_renderResources->GetPrimitive(RenderResources::ePrimitiveTileQuad);
		const ion::Colour& colour = m_renderResources->GetColour(RenderResources::eColourSelected);

		renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
		material->SetDiffuseColour(colour);

		ion::Vector2 boundsMin;
		ion::Vector2 boundsMax;

		m_highlightedBezier->GetBounds(boundsMin, boundsMax);

		//Ensure bounds are at least 2 tiles thick
		if ((ion::maths::Abs(boundsMax.x - boundsMin.x) / (float)tileWidth) < 2.0f)
		{
			boundsMin.x -= (float)tileWidth;
			boundsMax.x += (float)tileWidth;
		}

		if ((ion::maths::Abs(boundsMax.y - boundsMin.y) / (float)tileHeight) < 2.0f)
		{
			boundsMin.y -= (float)tileHeight;
			boundsMax.y += (float)tileHeight;
		}

		ion::Vector2 size = boundsMax - boundsMin;

		const float x = boundsMin.x + (size.x / 2.0f);
		const float y = boundsMin.y + (size.y / 2.0f);

		ion::Vector3 previewScale(size.x / (float)tileWidth, size.y / (float)tileWidth, 1.0f);
		ion::Matrix4 previewMtx;
		ion::Vector3 previewPos(x, y, z);
		previewMtx.SetTranslation(previewPos);
		previewMtx.SetScale(previewScale);

		material->Bind(previewMtx * bezierMatrix, cameraInverseMtx, projectionMtx);
		renderer.DrawVertexBuffer(primitive->GetVertexBuffer(), primitive->GetIndexBuffer());
		material->Unbind();

		renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);
	}

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void SpriteCanvas::RenderTerrainCanvas(ion::render::Renderer& renderer, const ion::Matrix4& cameraInverseMtx, const ion::Matrix4& projectionMtx, float z)
{
	//No depth test (stops grid cells Z fighting)
	renderer.SetDepthTest(ion::render::Renderer::eAlways);

	ion::render::Material* material = m_renderResources->GetMaterial(RenderResources::eMaterialTerrainTileset);

	//Draw map
	renderer.SetAlphaBlending(ion::render::Renderer::eTranslucent);
	material->SetDiffuseColour(ion::Colour(1.0f, 1.0f, 1.0f, 1.0f));
	material->Bind(ion::Matrix4(), cameraInverseMtx, projectionMtx);
	renderer.DrawVertexBuffer(m_terrainCanvasPrimitive->GetVertexBuffer(), m_terrainCanvasPrimitive->GetIndexBuffer());
	material->Unbind();
	renderer.SetAlphaBlending(ion::render::Renderer::eNoBlend);

	renderer.SetDepthTest(ion::render::Renderer::eLessEqual);
}

void SpriteCanvas::EventHandlerPaint(wxPaintEvent& event)
{
	//Begin rendering to current viewport
	m_renderer->BeginFrame(m_viewport, GetHDC());
	m_renderer->ClearColour();
	m_renderer->ClearDepth();

	ion::Matrix4 cameraInverseMtx = m_camera.GetTransform().GetInverse();
	ion::Matrix4 projectionMtx = m_renderer->GetProjectionMatrix();

	//Z order
	const float zOffset = 0.0001f;
	float z = 0.0f;

	//Render callback
	OnRender(*m_renderer, cameraInverseMtx, projectionMtx, z, zOffset);

	//End rendering
	m_renderer->SwapBuffers();
	m_renderer->EndFrame();

	event.Skip();
}

void SpriteCanvas::EventHandlerResize(wxSizeEvent& event)
{
	OnResize(event);
	event.Skip();
}

void SpriteCanvas::EventHandlerMouse(wxMouseEvent& event)
{
	//Get mouse delta
	ion::Vector2i mousePosScreenSpace(event.GetX(), event.GetY());
	ion::Vector2i mouseDelta = mousePosScreenSpace - m_mousePrevPos;
	m_mousePrevPos = mousePosScreenSpace;

	OnMouse(event, mouseDelta);
	event.Skip();
}

void SpriteCanvas::PaintCollisionStamp(const Stamp& stamp)
{
	int width = stamp.GetWidth();
	int height = stamp.GetHeight();

	//Paint all collision tiles
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//Get terrain tile and collision flags
			TerrainTileId terrainTileId = stamp.GetTerrainTile(x, y);
			u16 collisionFlags = stamp.GetCollisionTileFlags(x, y);

			//Invert Y for OpenGL
			int yInv = height - 1 - y;

			//Paint tile
			PaintCollisionTile(terrainTileId, collisionFlags, x, yInv);
		}
	}
}

void SpriteCanvas::PaintCollisionTile(TerrainTileId terrainTileId, u16 collisionFlags, int x, int y)
{
	//Set texture coords for terrain cell
	ion::render::TexCoord coords[4];
	m_renderResources->GetTerrainTileTexCoords(terrainTileId, coords);
	m_terrainCanvasPrimitive->SetTexCoords((y * m_canvasSize.x) + x, coords);
}

void SpriteCanvas::PaintTerrainBeziers(Project& project)
{
	for (int i = 0; i < m_primitiveBeziers.size(); i++)
	{
		delete m_primitiveBeziers[i];
	}

	m_primitiveBeziers.clear();

	if (m_primitiveBezierPoints)
	{
		delete m_primitiveBezierPoints;
		m_primitiveBezierPoints = NULL;
	}

	if (m_primitiveBezierHandles)
	{
		delete m_primitiveBezierHandles;
		m_primitiveBezierHandles = NULL;
	}

	for (int i = 0; i < project.GetEditingCollisionMap().GetNumTerrainBeziers(); i++)
	{
		ion::gamekit::BezierPath* bezier = project.GetEditingCollisionMap().GetTerrainBezier(i);
		m_primitiveBeziers.push_back(m_renderResources->CreateBezierPrimitive(*bezier));
	}

	if (m_currentBezier && m_currentBezier->GetNumPoints() > 0)
	{
		m_primitiveBezierPoints = m_renderResources->CreateBezierPointsPrimitive(*m_currentBezier, 2.0f);
		m_primitiveBezierHandles = m_renderResources->CreateBezierHandlesPrimitive(*m_currentBezier, 1.0f);
	}
}