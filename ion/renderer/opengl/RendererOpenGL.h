///////////////////////////////////////////////////
// File:		RendererOpenGL.h
// Date:		4th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL renderer implementation
///////////////////////////////////////////////////

#pragma once

#include "core/thread/CriticalSection.h"
#include "core/thread/LocalStorage.h"
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

			//Set OpenGL context for current thread
			static void SetGLThreadContext();

		protected:
			//SDL window context - TODO: Support for multiple windows
			static SDL_Window* sSDLWindow;

			//Thread OpenGL contexts
			static const int sMaxGLThreadContexts;
			static std::vector<SDL_GLContext> sGLThreadContexts;
			static thread::LocalStorage sGLThreadContextStorage;
			static thread::CriticalSection sGLThreadContextCriticalSection;

			class GLThreadContext : public thread::StorageData
			{
			public:
				GLThreadContext(SDL_GLContext& context, SDL_Window& window);
				~GLThreadContext();
				void Set();

			private:
				SDL_Window* mSDLWindow;
				SDL_GLContext mGLContext;
			};
		};
	}
}