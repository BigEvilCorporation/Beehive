///////////////////////////////////////////////////
// File:		RendererOpenGL.h
// Date:		4th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL renderer implementation
///////////////////////////////////////////////////

#pragma once

#include "core/thread/CriticalSection.h"
#include "renderer/Renderer.h"

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

namespace ion
{
	namespace render
	{
		class RendererOpenGL : public Renderer
		{
		public:
			//Create from existing DC
			RendererOpenGL(DeviceContext deviceContext);

			virtual ~RendererOpenGL();

			virtual bool Update(float deltaTime);
			virtual void OnResize(int width, int height);

			//Fixed function transforms
			virtual void SetMatrix(const Matrix4& matrix);
			virtual Matrix4 GetProjectionMatrix();

			//Rendering - general
			virtual void BeginFrame(const Viewport& viewport, const DeviceContext& deviceContext);
			virtual void EndFrame();
			virtual void SwapBuffers();
			virtual void ClearColour();
			virtual void ClearDepth();

			//Render states
			virtual void SetAlphaBlending(AlphaBlendType alphaBlendType);
			virtual void SetFaceCulling(CullingMode cullingMode);
			virtual void SetDepthTest(DepthTest depthTest);

			//Vertex buffer drawing
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer);
			virtual void DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer);

		protected:
			//Setup
			void BindDC(DeviceContext deviceContext);
			void CreateContext(DeviceContext deviceContext);
			void InitContext(DeviceContext deviceContext);
			void SetupViewport(const Viewport& viewport);

			//Lock/unlock context for device context
			void LockGLContext(DeviceContext deviceContext);
			void UnlockGLContext();

			//Check for OpenGL errors
			static bool CheckGLError();

			//OpenGL context
			HGLRC m_openGLContext;

			//DC for gobal (non-rendering) context
			DeviceContext m_globalDC;

			//DC for current locked context
			DeviceContext m_currentDC;

			thread::CriticalSection m_contextCriticalSection;
			u32 m_contextLockStack;
		};
	}
}