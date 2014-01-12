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

		const int RendererOpenGL::sMaxGLThreadContexts = 8;
		std::vector<SDL_GLContext> RendererOpenGL::sGLThreadContexts;
		thread::LocalStorage RendererOpenGL::sGLThreadContextStorage;
		thread::CriticalSection RendererOpenGL::sGLThreadContextCriticalSection;
		SDL_Window* RendererOpenGL::sSDLWindow = NULL;

		Renderer* Renderer::Create(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
		{
			return new RendererOpenGL(windowTitle, windowWidth, windowHeight, fullscreen);
		}

		RendererOpenGL::RendererOpenGL(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen)
			: Renderer(windowTitle, windowWidth, windowHeight, fullscreen)
		{
			//Set OpenGL version
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

			//Set OpenGL flags
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			//Set shared contexts for threading
			SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

			//Set window creation flags
			int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			if(fullscreen)
			{
				windowFlags |= SDL_WINDOW_FULLSCREEN;
			}

			//Create window
			sSDLWindow = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, windowFlags);
			if(!sSDLWindow)
			{
				debug::Error("Unable to create SDL window");
			}

			//Create OpenGL contexts for all threads
			for(int i = 0; i < sMaxGLThreadContexts; i++)
			{
				sGLThreadContexts.push_back(SDL_GL_CreateContext(sSDLWindow));
			}

			//Set OpenGL context for this thread
			SetGLThreadContext();

			//Get version
			const char* version = (const char*)glGetString(GL_VERSION);

			//Intialise OpenGL extensions
			glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) SDL_GL_GetProcAddress("glActiveTextureARB");
			glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) SDL_GL_GetProcAddress("glGenFramebuffersEXT");
			glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) SDL_GL_GetProcAddress("glBindFramebufferEXT");
			glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) SDL_GL_GetProcAddress("glGenRenderbuffersEXT");
			glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) SDL_GL_GetProcAddress("glBindRenderbufferEXT");
			glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) SDL_GL_GetProcAddress("glRenderbufferStorageEXT");
			glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) SDL_GL_GetProcAddress("glFramebufferRenderbufferEXT");
			glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
			glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
			glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC) SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
			glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) SDL_GL_GetProcAddress("glDeleteRenderbuffersEXT");
			glDrawBuffers = (PFNGLDRAWBUFFERSPROC) SDL_GL_GetProcAddress("glDrawBuffers");

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

			//Flip all texture drawing (SDL textures are upside down to OpenGL)
			glMatrixMode(GL_TEXTURE);
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
			glScalef(-1.0f, 1.0f, 1.0f);
			glMatrixMode(GL_MODELVIEW);

			//Call resize to setup viewport
			OnResize(windowWidth, windowHeight);

			//Check for OpenGL errors
			if(glGetError() != GL_NO_ERROR)
			{
				debug::Error("Could not initialise OpenGL");
			}

			//Clear depth and colour
			ClearColour();
			ClearDepth();

			//Hide the cursor by default
			SDL_ShowCursor(SDL_DISABLE);

			//Set window caption
			SetWindowTitle(windowTitle);

			//Initialise shader manager
			mShaderManager = ShaderManager::Create();
		}

		RendererOpenGL::~RendererOpenGL()
		{
			delete mShaderManager;
			
			SDL_DestroyWindow(sSDLWindow);
			SDL_Quit();
		}

		void RendererOpenGL::SetGLThreadContext()
		{
			GLThreadContext* threadContext = (GLThreadContext*)sGLThreadContextStorage.GetData();
			if(!threadContext)
			{
				//Find a context
				sGLThreadContextCriticalSection.Begin();

				for(int i = 0; i < sMaxGLThreadContexts && !threadContext; i++)
				{
					if(sGLThreadContexts[i])
					{
						threadContext = new GLThreadContext(sGLThreadContexts[i], *sSDLWindow);
						sGLThreadContexts[i] = NULL;
					}
				}

				sGLThreadContextCriticalSection.End();

				debug::Assert(threadContext != NULL, "Could not get OpenGL thread context");

				//Store for current thread
				sGLThreadContextStorage.SetData(*threadContext);
			}

			//Set context
			threadContext->Set();
		}

		void RendererOpenGL::SetWindowTitle(const std::string& title)
		{
			SDL_SetWindowTitle(sSDLWindow, title.c_str());
		}

		bool RendererOpenGL::Update(float deltaTime)
		{
			SDL_PumpEvents();

			SDL_Event sdlEvent;
			while(SDL_PollEvent(&sdlEvent) != 0)
			{
				switch(sdlEvent.type)
				{
				case SDL_QUIT:
					return false;
					break;

				default:
					break;
				}
			}

			return true;
		}

		void RendererOpenGL::OnResize(int width, int height)
		{
			//Make sure height is at least 1
			if(height == 0)
			{
				height = 1;
			}

			//Set window dimensions
			mWindowWidth = width;
			mWindowHeight = height;

			//Set the viewport
			glViewport(0, 0, width, height);

			//Select projection matrix
			glMatrixMode(GL_PROJECTION);

			//Reset projection matrix
			glLoadIdentity();

			//Calculate aspect ratio
			gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);

			//Select the modelview matrix
			glMatrixMode(GL_MODELVIEW);

			//Reset the modelview matrix
			glLoadIdentity();
		}

		void RendererOpenGL::SetMatrix(const Matrix4& matrix)
		{
			glLoadMatrixf(matrix.GetAsFloatArray());
		}

		Matrix4 RendererOpenGL::GetProjectionMatrix()
		{
			float matrix[16] = { 0.0f };
			glGetFloatv(GL_PROJECTION_MATRIX, matrix);
			return Matrix4(matrix);
		}

		void RendererOpenGL::SwapBuffers()
		{
			SDL_GL_SwapWindow(sSDLWindow);
		}

		void RendererOpenGL::ClearColour()
		{
			//Clear colour buffer
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void RendererOpenGL::ClearDepth()
		{
			//Clear depth buffer
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		void RendererOpenGL::SetClearColour(const Colour& colour)
		{
			glClearColor(colour.r, colour.g, colour.b, colour.a);
		}

		void RendererOpenGL::SetAlphaBlending(AlphaBlendType alphaBlendType)
		{
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
		}

		void RendererOpenGL::SetFaceCulling(CullingMode cullingMode)
		{
			switch(cullingMode)
			{
			case NoCull:
				glDisable(GL_CULL_FACE);
				break;

			case Clockwise:
				glEnable(GL_CULL_FACE);
				//glCullMode(GL_CULL_CW);
				break;

			case CounterClockwise:
				glEnable(GL_CULL_FACE);
				//glCullMode(GL_CULL_CCW);
				break;

			default:
				break;
			}
		}

		void RendererOpenGL::DrawVertexBuffer(const VertexBuffer& vertexBuffer)
		{
			//Enable client states
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//Set element pointers
			glVertexPointer(vertexBuffer.GetVertexSize(), GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetVertexBuffer());
			glNormalPointer(GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetNormalBuffer());
			glTexCoordPointer(vertexBuffer.GetTexCoordSize(), GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetTexCoordBuffer());

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
		}

		void RendererOpenGL::DrawVertexBuffer(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer)
		{
			//Enable client states
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			//Set element pointers
			glVertexPointer(vertexBuffer.GetVertexSize(), GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetVertexBuffer());
			glNormalPointer(GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetNormalBuffer());
			glTexCoordPointer(vertexBuffer.GetTexCoordSize(), GL_FLOAT, vertexBuffer.GetStride(), vertexBuffer.GetTexCoordBuffer());

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

			if(glGetError() != GL_NO_ERROR)
			{
				debug::Error("Could not draw vertex buffer");
			}
		}

		RendererOpenGL::GLThreadContext::GLThreadContext(SDL_GLContext& context, SDL_Window& window)
		{
			mSDLWindow = &window;
			mGLContext = context;
		}

		RendererOpenGL::GLThreadContext::~GLThreadContext()
		{
			SDL_GL_DeleteContext(mGLContext);
		}

		void RendererOpenGL::GLThreadContext::Set()
		{
			SDL_GL_MakeCurrent(mSDLWindow, mGLContext);
		}
	}
}
