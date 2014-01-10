///////////////////////////////////////////////////
// File:		RendererOpenGL.h
// Date:		4th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL renderer implementation
///////////////////////////////////////////////////

#pragma once

#include "renderer/Renderer.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <SDL/SDL.h>

namespace ion
{
	namespace render
	{
		class RendererOpenGL : public Renderer
		{
		public:
			RendererOpenGL(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen);
			virtual ~RendererOpenGL();

			virtual bool Update(float deltaTime);
			virtual void SetWindowTitle(const std::string& title);
			virtual void OnResize(int width, int height);

			//Fixed function transforms
			virtual void SetMatrix(const Matrix4& matrix);
			virtual Matrix4 GetProjectionMatrix();

			//Rendering - general
			virtual void SwapBuffers();
			virtual void ClearColour();
			virtual void ClearDepth();
			virtual void SetClearColour(const Colour& colour);

			//Render states
			virtual void SetAlphaBlending(AlphaBlendType alphaBlendType);
			virtual void SetFaceCulling(CullingMode cullingMode);

			//Vertex buffer drawing
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer);
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer);

			//Share main GL context with current thread
			static void SetThreadGLContext();

			//SDL window context
			SDL_Window* mSDLWindow;

			//Main SDL OpenGL context
			SDL_GLContext mSDLGLContext;
		};
	}
}