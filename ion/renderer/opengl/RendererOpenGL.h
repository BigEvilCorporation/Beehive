///////////////////////////////////////////////////
// File:		RendererOpenGL.h
// Date:		4th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL renderer implementation
///////////////////////////////////////////////////

#pragma once

#include "core/thread/CriticalSection.h"
#include "renderer/Renderer.h"
#include "renderer/win32/WindowWin32.h"

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

namespace ion
{
	namespace render
	{
		class RendererOpenGL : public Renderer
		{
		public:
			RendererOpenGL(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen);
			virtual ~RendererOpenGL();

			//Get window
			virtual Window* GetWindow() const;

			virtual bool Update(float deltaTime);
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

			//OpenGL context thread safety
			static void LockGLContext();
			static void UnlockGLContext();

			//Check for OpenGL errors
			static bool CheckGLError();

		protected:
			//Window
			WindowWin32* mWindow;

			//Main OpenGL context
			static HGLRC sOpenGLContext;

			//Window DC
			static HDC sDrawContext;

			static thread::CriticalSection sGLContextCriticalSection;
			static u32 sGLContextLockStack;
		};
	}
}