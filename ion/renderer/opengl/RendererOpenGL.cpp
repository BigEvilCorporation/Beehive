///////////////////////////////////////////////////
// File:		RendererOpenGL.cpp
// Date:		4th December 2013
// Authors:		Matt Phillips
// Description:	OpenGL renderer implementation
///////////////////////////////////////////////////

#include "core/debug/Debug.h"
#include "renderer/colour.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "renderer/Shader.h"
#include "renderer/opengl/RendererOpenGL.h"

namespace ion
{
	namespace render
	{
		//OpenGL extensions
		PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
		PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
		PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
		PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
		PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
		PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
		PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
		PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
		PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
		PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
		PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
		PFNGLDRAWBUFFERSPROC glDrawBuffers;

		HGLRC RendererOpenGL::sOpenGLContext = 0;
		HDC RendererOpenGL::sDrawContext = 0;
		thread::CriticalSection RendererOpenGL::sGLContextCriticalSection;
		u32 RendererOpenGL::sGLContextLockStack = 0;

		Renderer* Renderer::Create(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
		{
			return new RendererOpenGL(windowTitle, windowWidth, windowHeight, fullscreen);
		}

		Renderer* Renderer::Create(HWND window, int windowWidth, int windowHeight)
		{
			return new RendererOpenGL(window, windowWidth, windowHeight);
		}

		Renderer* Renderer::Create(HWND window, HGLRC context, int windowWidth, int windowHeight)
		{
			return new RendererOpenGL(window, windowWidth, windowHeight);
		}

		RendererOpenGL::RendererOpenGL(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
			: Renderer(windowTitle, windowWidth, windowHeight, fullscreen)
			, m_perspectiveMode(Perspective3D)
		{
			//Create window
			CreateWindow(windowTitle, windowWidth, windowHeight, fullscreen);

			//Create OpenGL context
			CreateContext();

			//Init context
			InitContext(windowWidth, windowHeight);
		}

		RendererOpenGL::RendererOpenGL(HWND window, int windowWidth, int windowHeight)
			: Renderer("", windowWidth, windowHeight, false)
			, m_perspectiveMode(Perspective3D)
		{
			mWindow = NULL;

			//Get window draw context
			sDrawContext = GetWindowDC(window);

			//Create OpenGL context
			CreateContext();

			//Init context
			InitContext(windowWidth, windowHeight);
		}

		RendererOpenGL::RendererOpenGL(HWND window, HGLRC context, int windowWidth, int windowHeight)
			: Renderer("", windowWidth, windowHeight, false)
			, m_perspectiveMode(Perspective3D)
		{
			mWindow = NULL;

			//Get window draw context
			sDrawContext = GetWindowDC(window);

			//Using existing OpenGL context
			sOpenGLContext = context;

			//Init context
			InitContext(windowWidth, windowHeight);
		}

		RendererOpenGL::~RendererOpenGL()
		{
			delete mShaderManager;
			delete mWindow;
		}

		void RendererOpenGL::CreateWindow(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
		{
			//Create window
			mWindow = new WindowWin32(windowTitle, windowWidth, windowHeight, fullscreen);

			//Get draw context
			sDrawContext = mWindow->GetDrawContext();

			//Create pixel format descriptor
			PIXELFORMATDESCRIPTOR pixelFormatDesc = { 0 };
			pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pixelFormatDesc.nVersion = 1;
			pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
			pixelFormatDesc.cColorBits = 32;
			pixelFormatDesc.cDepthBits = 24;
			pixelFormatDesc.cStencilBits = 8;
			pixelFormatDesc.iLayerType = PFD_MAIN_PLANE;

			//Check pixel format
			int pixelFormat = ChoosePixelFormat(sDrawContext, &pixelFormatDesc);
			if(!pixelFormat)
			{
				debug::Error("Invalid pixel format");
			}

			//Set pixel format
			if(!SetPixelFormat(sDrawContext, pixelFormat, &pixelFormatDesc))
			{
				debug::Error("Could not set pixel format");
			}
		}

		void RendererOpenGL::CreateContext()
		{
			//Create OpenGL context
			sOpenGLContext = wglCreateContext(sDrawContext);
			if(!sOpenGLContext)
			{
				debug::Error("Could not create OpenGL context");
			}
		}

		void RendererOpenGL::InitContext(int viewportWidth, int viewportHeight)
		{
			//Set as current context
			wglMakeCurrent(sDrawContext, sOpenGLContext);
			
			//Set viewport width/height
			m_viewportWidth = viewportWidth;
			m_viewportHeight = viewportHeight;

			//Get version
			const char* version = (const char*)glGetString(GL_VERSION);

			//Intialise OpenGL extensions
			glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
			glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
			glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
			glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
			glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
			glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
			glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
			glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
			glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
			glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
			glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
			glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");

			//Background colour
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			//Enable depth testing
			glEnable(GL_DEPTH_TEST);

			//Depth buffer setup
			glClearDepth(1.0f);
			glDepthFunc(GL_LEQUAL);

			//Enable back face culling
			glEnable(GL_CULL_FACE);

			//Shading and perspective
			glShadeModel(GL_SMOOTH);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

			//Set default blending mode
			glBlendFunc(GL_ONE, GL_ONE);

			//Call resize to setup viewport
			OnResize(viewportWidth, viewportHeight);

			//Check for OpenGL errors
			if(!CheckGLError())
			{
				debug::Error("Could not initialise OpenGL");
			}

			//Clear depth and colour
			ClearColour();
			ClearDepth();

			//Create shader manager
			mShaderManager = ShaderManager::Create();
		}

		Window* RendererOpenGL::GetWindow() const
		{
			return mWindow;
		}

		void RendererOpenGL::LockGLContext()
		{
			sGLContextCriticalSection.Begin();

			if(sGLContextLockStack == 0)
			{
				wglMakeCurrent(sDrawContext, sOpenGLContext);
			}

			sGLContextLockStack++;
		}

		void RendererOpenGL::UnlockGLContext()
		{
			debug::Assert(sGLContextLockStack > 0, "Bad OpenGL context lock count");
			sGLContextLockStack--;
			if(!sGLContextLockStack)
			{
				wglMakeCurrent(sDrawContext, NULL);
			}

			sGLContextCriticalSection.End();
		}

		bool RendererOpenGL::CheckGLError()
		{
			LockGLContext();
			GLenum error = glGetError();
			UnlockGLContext();

			if(error != GL_NO_ERROR)
			{
				debug::Error("OpenGL error");
			}

			return error == GL_NO_ERROR;
		}

		bool RendererOpenGL::Update(float deltaTime)
		{
			return mWindow->Update();
		}

		void RendererOpenGL::OnResize(int width, int height)
		{
			LockGLContext();

			//Make sure height is at least 1
			if(height == 0)
			{
				height = 1;
			}

			//Resize window
			if(mWindow)
			{
				mWindow->Resize(width, height);
			}

			m_viewportWidth = width;
			m_viewportHeight = height;

			//Set the viewport
			glViewport(0, 0, width, height);

			//Select projection matrix
			glMatrixMode(GL_PROJECTION);

			//Reset projection matrix
			glLoadIdentity();

			//Calculate aspect ratio
			float aspectRatio = (float)width / (float)height;

			//Set perspective mode
			switch(m_perspectiveMode)
			{
			case Perspective3D:
				//TODO: Expose FOV and near/far
				gluPerspective(45.0f, aspectRatio, 0.1f, 10000.0f);
				break;

			case Ortho2DNormalised:
				glOrtho(0.0, 1.0f, 0.0, 1.0f, -1.0, 1.0);
				break;

			case Ortho2DAbsolute:
				glOrtho(0.0, m_viewportWidth, 0.0, m_viewportHeight, -1.0, 1.0);
				break;
			}

			//Select the modelview matrix
			glMatrixMode(GL_MODELVIEW);

			//Reset the modelview matrix
			glLoadIdentity();

			UnlockGLContext();
		}

		void RendererOpenGL::SetMatrix(const Matrix4& matrix)
		{
			LockGLContext();
			glLoadMatrixf(matrix.GetAsFloatArray());
			UnlockGLContext();
		}

		Matrix4 RendererOpenGL::GetProjectionMatrix()
		{
			float matrix[16] = { 0.0f };

			LockGLContext();
			glGetFloatv(GL_PROJECTION_MATRIX, matrix);
			UnlockGLContext();

			return Matrix4(matrix);
		}

		void RendererOpenGL::BeginFrame()
		{
			LockGLContext();
		}

		void RendererOpenGL::EndFrame()
		{
			UnlockGLContext();
		}

		void RendererOpenGL::SwapBuffers()
		{
			LockGLContext();
			::SwapBuffers(sDrawContext);
			UnlockGLContext();
		}

		void RendererOpenGL::ClearColour()
		{
			LockGLContext();

			//Clear colour buffer
			glClear(GL_COLOR_BUFFER_BIT);

			UnlockGLContext();
		}

		void RendererOpenGL::ClearDepth()
		{
			LockGLContext();

			//Clear depth buffer
			glClear(GL_DEPTH_BUFFER_BIT);

			UnlockGLContext();
		}

		void RendererOpenGL::SetClearColour(const Colour& colour)
		{
			LockGLContext();

			glClearColor(colour.r, colour.g, colour.b, colour.a);

			UnlockGLContext();
		}

		void RendererOpenGL::SetPerspectiveMode(PerspectiveMode perspectiveMode)
		{
			m_perspectiveMode = perspectiveMode;
			OnResize(m_viewportWidth, m_viewportHeight);
		}

		void RendererOpenGL::SetAlphaBlending(AlphaBlendType alphaBlendType)
		{
			LockGLContext();

			switch(alphaBlendType)
			{
			case NoBlend:
				glDisable(GL_BLEND);
				break;

			case Additive:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;

			case Translucent:
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;

			default:
				break;
			}

			UnlockGLContext();
		}

		void RendererOpenGL::SetFaceCulling(CullingMode cullingMode)
		{
			LockGLContext();

			switch(cullingMode)
			{
			case NoCull:
				glDisable(GL_CULL_FACE);
				break;

			case Clockwise:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
				break;

			case CounterClockwise:
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;

			default:
				break;
			}

			UnlockGLContext();
		}

		void RendererOpenGL::DrawVertexBuffer(const VertexBuffer& vertexBuffer)
		{
			LockGLContext();

			//Enable client states
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//Set element pointers
			glVertexPointer(vertexBuffer.GetVertexSize(), GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetVertexBuffer());
			glNormalPointer(GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetNormalBuffer());
			glTexCoordPointer(vertexBuffer.GetTexCoordSize(), GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetTexCoordBuffer());

			//Determine pattern type
			int drawPattern = 0;
			int numVertices = vertexBuffer.GetNumVerts();

			switch(vertexBuffer.GetPattern())
			{
			case VertexBuffer::Triangles:
				drawPattern = GL_TRIANGLES;
				break;

			case VertexBuffer::Quads:
				drawPattern = GL_QUADS;
				break;

			default:
				debug::Error("Invalid number of vertices");
			}

			//Draw
			glDrawArrays(drawPattern, 0, vertexBuffer.GetNumVerts());

			//Disable client states
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			if(glGetError() != GL_NO_ERROR)
			{
				debug::Error("Could not draw vertex buffer");
			}

			UnlockGLContext();
		}

		void RendererOpenGL::DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
		{
			LockGLContext();

			//Enable client states
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//Set element pointers
			glVertexPointer(vertexBuffer.GetVertexSize(), GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetVertexBuffer());
			glNormalPointer(GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetNormalBuffer());
			glTexCoordPointer(vertexBuffer.GetTexCoordSize(), GL_FLOAT, vertexBuffer.GetStrideBytes(), vertexBuffer.GetTexCoordBuffer());

			//Determine pattern type
			int drawPattern = 0;
			int numVertices = vertexBuffer.GetNumVerts();

			switch(vertexBuffer.GetPattern())
			{
			case VertexBuffer::Triangles:
				drawPattern = GL_TRIANGLES;
				break;

			case VertexBuffer::Quads:
				drawPattern = GL_QUADS;
				break;

			default:
				debug::Error("Invalid number of vertices");
			}

			//Draw
			glDrawElements(drawPattern, indexBuffer.GetSize(), GL_UNSIGNED_SHORT, indexBuffer.GetAddress());

			//Disable client states
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			CheckGLError();

			UnlockGLContext();
		}
	}
}
