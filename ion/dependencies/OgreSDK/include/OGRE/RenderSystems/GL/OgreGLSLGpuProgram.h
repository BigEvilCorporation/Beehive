/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef __GLSLGpuProgram_H__
#define __GLSLGpuProgram_H__

// Precompiler options
#include "OgreGLSLExtSupport.h"
#include "OgreGLGpuProgram.h"


namespace Ogre {

    /** GLSL low level compiled shader object - this class is used to get at the linked program object 
		and provide an interface for GLRenderSystem calls.  GLSL does not provide access to the
		low level code of the shader so this class is really just a dummy place holder.
		GLSL uses a program object to represent the active vertex and fragment programs used
		but Ogre materials maintain seperate instances of the active vertex and fragment programs
		which creates a small problem for GLSL integration.  The GLSLGpuProgram class provides the
		interface between the GLSLLinkProgramManager , GLRenderSystem, and the active GLSLProgram
		instances.
	*/
    class _OgreGLExport GLSLGpuProgram : public GLGpuProgram
    {
    private:
		/// GL Handle for the shader object
		GLSLProgram* mGLSLProgram;

		/// keep track of the number of vertex shaders created
		static GLuint mVertexShaderCount;
		/// keep track of the number of fragment shaders created
		static GLuint mFragmentShaderCount;
		/// keep track of the number of geometry shaders created
		static GLuint mGeometryShaderCount;

	public:
        GLSLGpuProgram(GLSLProgram* parent);
		~GLSLGpuProgram();


		/// Execute the binding functions for this program
		void bindProgram(void);
		/// Execute the unbinding functions for this program
		void unbindProgram(void);
		/// Execute the param binding functions for this program
		void bindProgramParameters(GpuProgramParametersSharedPtr params, uint16 mask);
		/// Execute the pass iteration param binding functions for this program
		void bindProgramPassIterationParameters(GpuProgramParametersSharedPtr params);

		/// Get the assigned GL program id
		const GLuint getProgramID(void) const
		{ return mProgramID; }

		/// get the GLSLProgram for the shader object
		GLSLProgram* getGLSLProgram(void) const { return mGLSLProgram; }

		/// @copydoc GLGpuProgram::getAttributeIndex
		GLuint getAttributeIndex(VertexElementSemantic semantic, uint index);
		
		/// @copydoc GLGpuProgram::isAttributeValid
		bool isAttributeValid(VertexElementSemantic semantic, uint index);
		

    protected:
        /// Overridden from GpuProgram
        void loadFromSource(void);
		/// @copydoc Resource::unloadImpl
		void unloadImpl(void);
		/// @copydoc Resource::loadImpl
		void loadImpl(void);


    };


}


#endif // __GLSLGpuProgram_H__
