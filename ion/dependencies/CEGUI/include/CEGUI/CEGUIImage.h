/***********************************************************************
	filename: 	CEGUIImage.h
	created:	13/3/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for Image class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIImage_h_
#define _CEGUIImage_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIColourRect.h"
#include "CEGUIVector.h"
#include "CEGUISize.h"
#include "CEGUIRenderer.h"
#include "CEGUIXMLSerializer.h" 
#include <map>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Class that represents a single Image of an Imageset.
*/
class CEGUIEXPORT Image
{
public:
	/*!
	\brief
		Return a Size object containing the dimensions of the Image.

	\return
		Size object holding the width and height of the Image.
	*/
	Size	getSize(void) const			{return Size(d_scaledWidth, d_scaledHeight);}
	

	/*!
	\brief
		Return the pixel width of the image.

	\return
		Width of this Image in pixels.
	*/
	float	getWidth(void) const		{return d_scaledWidth;}


	/*!
	\brief
		Return the pixel height of the image.

	\return
		Height of this Image in pixels
	*/
	float	getHeight(void) const		{return d_scaledHeight;}


	/*!
	\brief
		Return a Point object that contains the offset applied when rendering this Image

	\return
		Point object containing the offsets applied when rendering this Image
	*/
	Point	getOffsets(void) const		{return d_scaledOffset;}


	/*!
	\brief
		Return the X rendering offset

	\return
		X rendering offset.  This is the number of pixels that the image is offset by when rendering at any given location.
	*/
	float	getOffsetX(void) const		{return	d_scaledOffset.d_x;}


	/*!
	\brief
		Return the Y rendering offset

	\return
		Y rendering offset.  This is the number of pixels that the image is offset by when rendering at any given location.
	*/
	float	getOffsetY(void) const		{return d_scaledOffset.d_y;}


	/*!
	\brief
		Return the name of this Image object.

	\return
		String object containing the name of this Image
	*/
	const String&	getName(void) const;


	/*!
	\brief
		Return the name of the Imageset that contains this Image

	\return
		String object containing the name of the Imageset which this Image is a part of.
	*/
	const String&	getImagesetName(void) const;

	/*!
	\brief
		Return the parent Imageset object that contains this Image

	\return
		The parent Imageset object.
	*/
	const Imageset*	getImageset(void) const {return d_owner;}

    /*!
    \brief
        Return Rect describing the source texture area used by this Image.

    \return
        Rect object that describes, in pixels, the area upon the source texture
        which is used when rendering this Image.
    */
    const Rect& getSourceTextureArea(void) const;

    /*!
    \brief
        Queue the image to be drawn.

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param position
        Vector2 object containing the location where the Image is to be drawn

    \param size
        Size object describing the size that the Image is to be drawn at.

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param top_left_colour
        Colour to be applied to the top-left corner of the Image.

    \param top_right_colour
        Colour to be applied to the top-right corner of the Image.

    \param bottom_left_colour
        Colour to be applied to the bottom-left corner of the Image.

    \param bottom_right_colour
        Colour to be applied to the bottom-right corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Vector2& position, const Size& size,
              const Rect* clip_rect,
              const colour& top_left_colour = 0xFFFFFFFF,
              const colour& top_right_colour = 0xFFFFFFFF,
              const colour& bottom_left_colour = 0xFFFFFFFF,
              const colour& bottom_right_colour = 0xFFFFFFFF,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, Rect(position.d_x, position.d_y,
                          position.d_x + size.d_width,
                          position.d_y + size.d_height),
             clip_rect,
             ColourRect(top_left_colour, top_right_colour, bottom_left_colour,
                        bottom_right_colour),
             quad_split_mode);
    }

    /*!
    \brief
        Queue the image to be drawn.

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param dest_rect
        Rect object defining the area on-screen where the Image is to be drawn.
        The Image will be scaled to fill the area as required.

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param top_left_colour
        Colour to be applied to the top-left corner of the Image.

    \param top_right_colour
        Colour to be applied to the top-right corner of the Image.

    \param bottom_left_colour
        Colour to be applied to the bottom-left corner of the Image.

    \param bottom_right_colour
        Colour to be applied to the bottom-right corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Rect& dest_rect,
              const Rect* clip_rect,
              const colour& top_left_colour = 0xFFFFFFFF,
              const colour& top_right_colour = 0xFFFFFFFF,
              const colour& bottom_left_colour = 0xFFFFFFFF,
              const colour& bottom_right_colour = 0xFFFFFFFF,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, dest_rect, clip_rect,
             ColourRect(top_left_colour, top_right_colour,
                        bottom_left_colour, bottom_right_colour),
             quad_split_mode);
    }

    /*!
    \brief
        Queue the image to be drawn. 

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param position
        Vector2 object containing the location where the Image is to be drawn.

    \param size
        Size object describing the size that the Image is to be drawn at.

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param colours
        ColourRect object that describes the colour values to use for each
        corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Vector2& position, const Size& size,
              const Rect* clip_rect, const ColourRect& colours,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, Rect(position.d_x, position.d_y,
                          position.d_x + size.d_width,
                          position.d_y + size.d_height),
             clip_rect, colours, quad_split_mode);
    }

    /*!
    \brief
        Queue the image to be drawn.

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param position
        Vector2 object containing the location where the Image is to be drawn

    \note
        The image will be drawn at it's internally defined size.

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param colours
        ColourRect object that describes the colour values to use for each
        corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const Rect* clip_rect, const ColourRect& colours,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, Rect(position.d_x, position.d_y,
                          position.d_x + getWidth(),
                          position.d_y + getHeight()),
             clip_rect, colours, quad_split_mode);
    }

    /*!
    \brief
        Queue the image to be drawn.

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param position
        Vector2 object containing the location where the Image is to be drawn

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param top_left_colour
        Colour to be applied to the top-left corner of the Image.

    \param top_right_colour
        Colour to be applied to the top-right corner of the Image.

    \param bottom_left_colour
        Colour to be applied to the bottom-left corner of the Image.

    \param bottom_right_colour
        Colour to be applied to the bottom-right corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const Rect* clip_rect,
              const colour& top_left_colour = 0xFFFFFFFF,
              const colour& top_right_colour = 0xFFFFFFFF,
              const colour& bottom_left_colour = 0xFFFFFFFF,
              const colour& bottom_right_colour = 0xFFFFFFFF,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const
    {
        draw(buffer, Rect(position.d_x, position.d_y,
                          position.d_x + getWidth(),
                          position.d_y + getHeight()),
             clip_rect,
             ColourRect(top_left_colour, top_right_colour,
                        bottom_left_colour, bottom_right_colour),
             quad_split_mode);
    }

    /*!
    \brief
        Queue the image to be drawn.

    \note
        The final position of the Image will be adjusted by the offset values
        defined for this Image object.  If absolute positioning is essential
        then these values should be taken into account prior to calling the
        draw() methods.  However, by doing this you take away the ability of the
        Imageset designer to adjust the alignment and positioning of Images,
        therefore your component is far less useful since it requires code
        changes to modify image positioning that could have been handled from a
        data file.

    \param buffer
        GeometryBuffer object where the geometry for the image will be queued.

    \param dest_rect
        Rect object defining the area on-screen where the Image is to be drawn.
        The Image will be scaled to fill the area as required.

    \param clip_rect
        Rect object that defines an on-screen area that the Image will be
        clipped to when drawing.

    \param colours
        ColourRect object that describes the colour values to use for each
        corner of the Image.

    \param quad_split_mode
        One of the QuadSplitMode values specifying the way the quad geometry for
        the image is to be split into triangles.

    \return
        Nothing
    */
    void draw(GeometryBuffer& buffer, const Rect& dest_rect,
              const Rect* clip_rect, const ColourRect& colours,
              QuadSplitMode quad_split_mode = TopLeftToBottomRight) const;

    /*!
    \brief
        Writes an xml representation of this Image object to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.


    \return
        Nothing.
    */
    void writeXMLToStream(XMLSerializer& xml_stream) const;


	friend class std::map<String, Image, String::FastLessCompare>;
	friend struct std::pair<const String, Image>;


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Default constructor (only used by std::map)
	*/
	Image(void) {}


	/*!
	\brief
		Constructor for Image objects.  This is not normally used directly by client code, use the Imageset interface instead.

	\param owner
		Pointer to a Imageset object that owns this Image.  This must not be NULL.
		
	\param name
		String object describing the name of the image being created.
		
	\param area
		Rect object describing an area that will be associated with this image.
		
	\param render_offset
		Point object that describes the offset to be applied when rendering this image.
		
	\param horzScaling
		float value indicating the initial horizontal scaling to be applied to this image.

	\param vertScaling
		float value indicating the initial vertical scaling to be applied to this image.

	\exception NullObjectException	Thrown if \a owner was NULL.
	*/
	Image(const Imageset* owner, const String& name, const Rect& area, const Point& render_offset, float horzScaling = 1.0f, float vertScaling = 1.0f);



	/*!
	\brief
		Copy constructor
	*/
	Image(const Image& image);


	/*!
	\brief
		Destructor for Image objects.
	*/
	~Image(void);


private:
	/*************************************************************************
		Friends
	*************************************************************************/
	friend class Imageset;

		
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		set the horizontal scaling factor to be applied to this Image

	\param factor
		float value describing the scaling factor required.

	\return
		Nothing.
	*/
	void	setHorzScaling(float factor);


	/*!
	\brief
		set the vertical scaling factor to be applied to this Image

	\param factor
		float value describing the scaling factor required.

	\return
		Nothing.
	*/
	void	setVertScaling(float factor);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Imageset*	d_owner;		//!< Link back to Imageset that owns this image
	Rect			d_area;			//!< Rect defining the area on the texture that makes up this image.
	Point			d_offset;		//!< Offset to use when rendering

	// image auto-scaling fields.
	float	d_scaledWidth;		//!< scaled image width.
	float	d_scaledHeight;		//!< scaled image height.
	Point	d_scaledOffset;		//!< scaled rendering offset.
	String	d_name;				//!< name of this image.
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIImage_h_
