///////////////////////////////////////////////////
// File:		Renderer.h
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Renderer base class
///////////////////////////////////////////////////

#pragma once

#include "maths/Matrix.h"
#include "maths/Vector.h"
#include "renderer/Window.h"
#include "renderer/Viewport.h"

#include <string>

namespace ion
{
	//Forward declaration
	class Colour;

	namespace render
	{
		//Forward declaration
		class VertexBuffer;
		class IndexBuffer;
		class Material;
		class Texture;
		class ShaderManager;

		typedef HWND WindowHandle;
		typedef HDC DeviceContext;

		class Renderer
		{
		public:
			//Alpha blending types
			enum AlphaBlendType { NoBlend, Additive, Subtractive, Translucent };

			//Culling modes
			enum CullingMode { NoCull, Clockwise, CounterClockwise };

			//Depth test type
			enum DepthTest { Disabled, Always, LessEqual };

			//Render vertex pattern type
			enum VertexPattern { Triangles, Quads };

			static Renderer* Create(DeviceContext globalDC);

			virtual ~Renderer();

			//Update renderer
			virtual bool Update(float deltaTime) = 0;

			//Resize callback
			virtual void OnResize(int width, int height) = 0;

			//Fixed function transforms
			virtual void SetMatrix(const Matrix4& matrix) = 0;
			virtual Matrix4 GetProjectionMatrix() = 0;

			//Rendering - general
			virtual void BeginFrame(const Viewport& viewport, const DeviceContext& deviceContext) = 0;
			virtual void EndFrame() = 0;
			virtual void SwapBuffers() = 0;
			virtual void SetClearColour(const Colour& colour) = 0;
			virtual void ClearColour() = 0;
			virtual void ClearDepth() = 0;

			//Render states
			virtual void SetAlphaBlending(AlphaBlendType alphaBlendType) = 0;
			virtual void SetFaceCulling(CullingMode cullingMode) = 0;
			virtual void SetDepthTest(DepthTest depthTest) = 0;

			//Vertex buffer drawing
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer) = 0;
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) = 0;

		protected:
			Renderer();

			ShaderManager* mShaderManager;
		};
	}
}