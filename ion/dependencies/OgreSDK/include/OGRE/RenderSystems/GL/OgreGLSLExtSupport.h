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



#ifndef __GLSLExtSupport_H__
#define __GLSLExtSupport_H__

#include "OgreGLPrerequisites.h"

//
// OpenGL Shading Language entry points
//
namespace Ogre
{

	// forward declarations
	class GLSLProgram;
	class GLSLGpuProgram;
	class GLSLLinkProgram;
	class GLSLLinkProgramManager;
	class GLSLProgramFactory;


	/** Check for GL errors and report them in the Ogre Log.

	@param forceInfoLog if true then message from GL info log is obtained
	@param forceException if true then exception is generated if a GL error found
	*/
    void checkForGLSLError(const String& ogreMethod, const String& errorTextPrefix, const GLhandleARB obj, const bool forceInfoLog = false, const bool forceException = false);

	/** if there is a message in GL info log then post it in the Ogre Log
	@param msg the info log message string is appended to this string
	@param obj the GL object that is used to retrieve the info log
	*/
	String logObjectInfo(const String& msg, const GLhandleARB obj);


} // namespace Ogre

#endif // __GLSLExtSupport_H__
