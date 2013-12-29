///////////////////////////////////////////////////
// File:		Renderer.h
// Date:		26th July 2011
// Authors:		Matt Phillips
// Description:	Renderer base class
///////////////////////////////////////////////////

#pragma once

#include "maths/Matrix.h"
#include "maths/Vector.h"

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

		class Renderer
		{
		public:
			//Alpha blending types
			enum AlphaBlendType { NoBlend, Additive, Subtractive, Translucent };

			//Culling modes
			enum CullingMode { NoCull, Clockwise, CounterClockwise };

			//Render perspectives
			enum PerspectiveMode { Perspective3D, Ortho2DNormalised, Ortho2DAbsolute };

			//Render vertex pattern type
			enum VertexPattern { Triangles, Quads };

			static Renderer* Create(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen);
			virtual ~Renderer();

			//Update renderer
			virtual bool Update(float deltaTime) = 0;

			//The the window title
			virtual void SetWindowTitle(const std::string& title) = 0;

			//Resize callback
			virtual void OnResize(int width, int height) = 0;

			//Get dimensions
			int GetWidth() { return mWindowWidth; }
			int GetHeight() { return mWindowHeight; }

			//Fixed function transforms
			virtual void SetMatrix(const Matrix4& matrix) = 0;
			virtual Matrix4 GetProjectionMatrix() = 0;

			//Rendering - general
			virtual void SwapBuffers() = 0;
			virtual void ClearColour() = 0;
			virtual void ClearDepth() = 0;
			virtual void SetClearColour(const Colour& colour) = 0;

			//Render states
			virtual void SetAlphaBlending(AlphaBlendType alphaBlendType) = 0;
			virtual void SetFaceCulling(CullingMode cullingMode) = 0;

			//Vertex buffer drawing
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer) = 0;
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer) = 0;

		protected:
			Renderer(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen);

			ShaderManager* mShaderManager;

			int mWindowWidth;
			int mWindowHeight;
		};
	}
}