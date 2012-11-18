/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd

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
#ifndef __HighLevelGpuProgram_H__
#define __HighLevelGpuProgram_H__

#include "OgrePrerequisites.h"
#include "OgreGpuProgram.h"

namespace Ogre {

	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Resources
	*  @{
	*/
	/** Abstract base class representing a high-level program (a vertex or
        fragment program).
    @remarks
        High-level programs are vertex and fragment programs written in a high-level
        language such as Cg or HLSL, and as such do not require you to write assembler code
        like GpuProgram does. However, the high-level program does eventually 
        get converted (compiled) into assembler and then eventually microcode which is
        what runs on the GPU. As well as the convenience, some high-level languages like Cg allow
        you to write a program which will operate under both Direct3D and OpenGL, something
        which you cannot do with just GpuProgram (which requires you to write 2 programs and
        use each in a Technique to provide cross-API compatibility). Ogre will be creating
        a GpuProgram for you based on the high-level program, which is compiled specifically 
        for the API being used at the time, but this process is transparent.
    @par
        You cannot create high-level programs direct - use HighLevelGpuProgramManager instead.
        Plugins can register new implementations of HighLevelGpuProgramFactory in order to add
        support for new languages without requiring changes to the core Ogre API. To allow 
        custom parameters to be set, this class extends StringInterface - the application
        can query on the available custom parameters and get/set them without having to 
        link specifically with it.
    */
    class _OgreExport HighLevelGpuProgram : public GpuProgram
    {
    protected:
        /// Whether the high-level program (and it's parameter defs) is loaded
        bool mHighLevelLoaded;
        /// The underlying assembler program
        GpuProgramPtr mAssemblerProgram;
		/// Have we built the name->index parameter map yet?
		mutable bool mConstantDefsBuilt;

        /// Internal load high-level portion if not loaded
        virtual void loadHighLevel(void);
        /// Internal unload high-level portion if loaded
        virtual void unloadHighLevel(void);
        /** Internal load implementation, loads just the high-level portion, enough to 
            get parameters.
        */
        virtual void loadHighLevelImpl(void);
        /** Internal method for creating an appropriate low-level program from this
        high-level program, must be implemented by subclasses. */
        virtual void createLowLevelImpl(void) = 0;
        /// Internal unload implementation, must be implemented by subclasses
        virtual void unloadHighLevelImpl(void) = 0;
        /// Populate the passed parameters with name->index map
        virtual void populateParameterNames(GpuProgramParametersSharedPtr params);
		/** Build the constant definition map, must be overridden.
		@note The implementation must fill in the (inherited) mConstantDefs field at a minimum, 
			and if the program requires that parameters are bound using logical 
			parameter indexes then the mFloatLogicalToPhysical and mIntLogicalToPhysical
			maps must also be populated.
		*/
		virtual void buildConstantDefinitions() const = 0;

        /** @copydoc Resource::loadImpl */
        void loadImpl();
        /** @copydoc Resource::unloadImpl */
        void unloadImpl();
    public:
        /** Constructor, should be used only by factory classes. */
        HighLevelGpuProgram(ResourceManager* creator, const String& name, ResourceHandle handle,
            const String& group, bool isManual = false, ManualResourceLoader* loader = 0);
        ~HighLevelGpuProgram();


        /** Creates a new parameters object compatible with this program definition. 
        @remarks
            Unlike low-level assembly programs, parameters objects are specific to the
            program and therefore must be created from it rather than by the 
            HighLevelGpuProgramManager. This method creates a new instance of a parameters
            object containing the definition of the parameters this program understands.
        */
        GpuProgramParametersSharedPtr createParameters(void);
        /** @copydoc GpuProgram::getBindingDelegate */
        GpuProgram* _getBindingDelegate(void) { return mAssemblerProgram.getPointer(); }

		/** Get the full list of GpuConstantDefinition instances.
		@note
		Only available if this parameters object has named parameters.
		*/
		const GpuNamedConstants& getConstantDefinitions() const;

		/// Override GpuProgram::getNamedConstants to ensure built
		const GpuNamedConstants& getNamedConstants() const { return getConstantDefinitions(); }





    };

    /** Specialisation of SharedPtr to allow SharedPtr to be assigned to HighLevelGpuProgramPtr 
    @note Has to be a subclass since we need operator=.
    We could templatise this instead of repeating per Resource subclass, 
    except to do so requires a form VC6 does not support i.e.
    ResourceSubclassPtr<T> : public SharedPtr<T>
    */
    class _OgreExport HighLevelGpuProgramPtr : public SharedPtr<HighLevelGpuProgram> 
    {
    public:
        HighLevelGpuProgramPtr() : SharedPtr<HighLevelGpuProgram>() {}
        explicit HighLevelGpuProgramPtr(HighLevelGpuProgram* rep) : SharedPtr<HighLevelGpuProgram>(rep) {}
        HighLevelGpuProgramPtr(const HighLevelGpuProgramPtr& r) : SharedPtr<HighLevelGpuProgram>(r) {} 
        HighLevelGpuProgramPtr(const ResourcePtr& r) : SharedPtr<HighLevelGpuProgram>()
        {
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
			    OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<HighLevelGpuProgram*>(r.getPointer());
                pUseCount = r.useCountPointer();
                if (pUseCount)
                {
                    ++(*pUseCount);
                }
            }
        }

        /// Operator used to convert a ResourcePtr to a HighLevelGpuProgramPtr
        HighLevelGpuProgramPtr& operator=(const ResourcePtr& r)
        {
            if (pRep == static_cast<HighLevelGpuProgram*>(r.getPointer()))
                return *this;
            release();
			// lock & copy other mutex pointer
            OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
            {
                OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
			    OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
                pRep = static_cast<HighLevelGpuProgram*>(r.getPointer());
                pUseCount = r.useCountPointer();
                if (pUseCount)
                {
                    ++(*pUseCount);
                }
            }
			else
			{
				// RHS must be a null pointer
				assert(r.isNull() && "RHS must be null if it has no mutex!");
				setNull();
			}
            return *this;
        }
		/// Operator used to convert a GpuProgramPtr to a HighLevelGpuProgramPtr
		HighLevelGpuProgramPtr& operator=(const GpuProgramPtr& r);
    };
	/** @} */
	/** @} */

}
#endif
