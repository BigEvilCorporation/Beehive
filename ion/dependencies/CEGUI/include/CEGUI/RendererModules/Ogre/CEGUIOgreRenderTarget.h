/***********************************************************************
    filename:   CEGUIOgreRenderTarget.h
    created:    Tue Feb 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIOgreRenderTarget_h_
#define _CEGUIOgreRenderTarget_h_

#include "../../CEGUIRenderTarget.h"
#include "CEGUIOgreRenderer.h"
#include "../../CEGUIRect.h"
#include <OgreMatrix4.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//! Intermediate RenderTarget implementing common parts for Ogre engine.
class OGRE_GUIRENDERER_API OgreRenderTarget : public virtual RenderTarget
{
public:
    //! Constructor
    OgreRenderTarget(OgreRenderer& owner, Ogre::RenderSystem& rs);

    //! Destructor
    virtual ~OgreRenderTarget();

    /*!
    \brief
        Set the underlying viewport area directly - bypassing what the
        RenderTarget considers to be it's area - thus allowing the view port
        area used for rendering to be different to the area set for the target.

    \param area
        Rect object describing the area to use in pixels.

    \deprecated
        This function is deprecated and will be removed or changed considerably
        in future releases.
    */
    void setOgreViewportDimensions(const Rect& area);

    // implement parts of CEGUI::RenderTarget interface
    void draw(const GeometryBuffer& buffer);
    void draw(const RenderQueue& queue);
    void setArea(const Rect& area);
    const Rect& getArea() const;
    void activate();
    void deactivate();
    void unprojectPoint(const GeometryBuffer& buff,
                        const Vector2& p_in, Vector2& p_out) const;

protected:
    //! helper that initialises the cached matrix
    void updateMatrix() const;
    //! helper that initialises the viewport
    void updateViewport();
    //! helper to update the actual Ogre viewport dimensions
    void updateOgreViewportDimensions(const Ogre::RenderTarget* const rt);

    //! OgreRenderer object that owns this RenderTarget
    OgreRenderer& d_owner;
    //! Ogre RendererSystem used to affect the rendering process
    Ogre::RenderSystem& d_renderSystem;
    //! holds defined area for the RenderTarget
    Rect d_area;
    //! Ogre render target that we are effectively wrapping
    Ogre::RenderTarget* d_renderTarget;
    //! Ogre viewport used for this target.
    Ogre::Viewport* d_viewport;
    //! projection / view matrix cache
    mutable Ogre::Matrix4 d_matrix;
    //! true when d_matrix is valid and up to date
    mutable bool d_matrixValid;
    //! tracks viewing distance (this is set up at the same time as d_matrix)
    mutable float d_viewDistance;
    //! true when d_viewport is up to date and valid.
    bool d_viewportValid;
    //! holds set Ogre viewport dimensions
    Rect d_ogreViewportDimensions;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOgreRenderTarget_h_
