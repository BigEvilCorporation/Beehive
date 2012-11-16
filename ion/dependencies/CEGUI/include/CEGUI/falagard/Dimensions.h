/***********************************************************************
    filename:   CEGUIDimensions.h
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIFalDimensions_h_
#define _CEGUIFalDimensions_h_

#include "./Enums.h"
#include "../String.h"
#include "../UDim.h"
#include "../Rect.h"
#include "../XMLSerializer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
     /*!
    \brief
        Abstract interface for a generic 'dimension' class.
    */
    class CEGUIEXPORT BaseDim :
        public AllocatedObject<BaseDim>
    {
    public:
        BaseDim();

        virtual ~BaseDim();

        /*!
        \brief
            Return a value that represents this dimension as absolute pixels.

        \param wnd
            Window object that may be used by the specialised class to aid in
            calculating the final value.

        \return
            float value which represents, in pixels, the same value as this BaseDim.
        */
        float getValue(const Window& wnd) const;

        /*!
        \brief
            Return a value that represents this dimension as absolute pixels.

        \param wnd
            Window object that may be used by the specialised class to aid in
            calculating the final value (typically would be used to obtain
            window/widget dimensions).

        \param container
            Rect object which describes an area to be considered as the base area
            when calculating the final value.  Basically this means that relative values
            are calculated from the dimensions of this Rect.

        \return
            float value which represents, in pixels, the same value as this BaseDim.
        */
        float getValue(const Window& wnd, const Rectf& container) const;

        /*!
        \brief
            Create an exact copy of the specialised class and return it as a pointer to
            a BaseDim object.

            Since the system needs to be able to copy objects derived from BaseDim, but only
            has knowledge of the BaseDim interface, this clone method is provided to prevent
            slicing issues.

        \return
            BaseDim object pointer
        */
        BaseDim* clone() const;

        /*!
        \brief
            Return the DimensionOperator set for this BaseDim based object.

        \return
            One of the DimensionOperator enumerated values representing a mathematical operation to be
            performed upon this BaseDim using the set operand.
        */
        DimensionOperator getDimensionOperator() const;

        /*!
        \brief
            Set the DimensionOperator set for this BaseDim based object.

        \param op
            One of the DimensionOperator enumerated values representing a mathematical operation to be
            performed upon this BaseDim using the set operand.

        \return
            Nothing.
        */
        void setDimensionOperator(DimensionOperator op);

        /*!
        \brief
            Return a pointer to the BaseDim set to be used as the other operand.

        \return
            Pointer to the BaseDim object.
        */
        const BaseDim* getOperand() const;

        /*!
        \brief
            Set the BaseDim set to be used as the other operand in calculations for this BaseDim.

        \param operand
            sub-class of BaseDim representing the 'other' operand.  The given object will be cloned; no
            transfer of ownership occurrs for the passed object.

        \return
            Nothing.
        */
        void setOperand(const BaseDim& operand);

        /*!
        \brief
            Writes an xml representation of this BaseDim to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.

        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

    protected:
        /*!
        \brief
            Implementataion method to return the base value for this BaseDim.  This method should
            not attempt to apply the mathematical operator; this is handled automatically.
        */
        virtual float getValue_impl(const Window& wnd) const = 0;

        /*!
        \brief
            Implementataion method to return the base value for this BaseDim.  This method should
            not attempt to apply the mathematical operator; this is handled automatically by BaseDim.
        */
        virtual float getValue_impl(const Window& wnd, const Rectf& container) const = 0;

        /*!
        \brief
            Implementataion method to return a clone of this sub-class of BaseDim.
            This method should not attempt to clone the mathematical operator or operand; theis is
            handled automatically by BaseDim.
        */
        virtual BaseDim* clone_impl() const = 0;

        /*!
        \brief
            Implementataion method to output real xml element name.
        */
        virtual void writeXMLElementName_impl(XMLSerializer& xml_stream) const = 0;

        /*!
        \brief
            Implementataion method to create the element attributes
        */
        virtual void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const = 0;

    private:
        DimensionOperator   d_operator;
        BaseDim*            d_operand;
    };


    /*!
    \brief
        Dimension type that represents an absolute pixel value.  Implements BaseDim interface.
    */
    class CEGUIEXPORT AbsoluteDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param val
            float value to be assigned to the AbsoluteDim.
        */
        AbsoluteDim(float val);

        /*!
        \brief
            Set the current value of the AbsoluteDim.
        */
        void setValue(float val);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;

        BaseDim* clone_impl() const;

    private:
        float d_val;    //!< holds pixel value for the AbsoluteDim.
    };


    /*!
    \brief
        Dimension type that represents some dimension of a named Image.  Implements BaseDim interface.
    */
    class CEGUIEXPORT ImageDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param name
            String object holding the name of the image.

        \param dim
            DimensionType value indicating which dimension of the described image that this ImageDim
            is to represent.
        */
        ImageDim(const String& name, DimensionType dim);

        /*!
        \brief
            Sets the source image information for this ImageDim.

        \param name
            String object holding the name of the image.

        \return
            Nothing.
        */
        void setSourceImage(const String& name);

        /*!
        \brief
            Sets the source dimension type for this ImageDim.

        \param dim
            DimensionType value indicating which dimension of the described image that this ImageDim
            is to represent.

        \return
            Nothing.
        */
        void setSourceDimension(DimensionType dim);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
        BaseDim* clone_impl() const;

    private:
        String d_image;         //!< name of the Image.
        DimensionType d_what;   //!< the dimension of the image that we are to represent.
    };


    /*!
    \brief
        Dimension type that represents some dimension of a Window/widget.  Implements BaseDim interface.

        When calculating the final pixel value for the dimension, a target widget name is built by
        appending the name suffix specified for the WidgetDim to the name of the window passed to
        getValue, we then find the window/widget with that name - the final value of the dimension
        is taken from this window/widget.
    */
    class CEGUIEXPORT WidgetDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param name
            String object holding the name suffix for a window/widget.

        \param dim
            DimensionType value indicating which dimension of the described image that this ImageDim
            is to represent.
        */
        WidgetDim(const String& name, DimensionType dim);

        /*!
        \brief
            Set the name suffix to use for this WidgetDim.

        \param name
            String object holding the name suffix for a window/widget.

        \return
            Nothing.
        */
        void setWidgetName(const String& name);

        /*!
        \brief
            Sets the source dimension type for this WidgetDim.

        \param dim
            DimensionType value indicating which dimension of the described image that this WidgetDim
            is to represent.

        \return
            Nothing.
        */
        void setSourceDimension(DimensionType dim);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
        BaseDim* clone_impl() const;

    private:
        String d_widgetName;    //!< Holds target window name suffix.
        DimensionType d_what;   //!< the dimension of the target window that we are to represent.
    };


    /*!
    \brief
        Dimension type that represents an Unified dimension.  Implements BaseDim interface.
    */
    class CEGUIEXPORT UnifiedDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param value
            UDim holding the value to assign to this UnifiedDim.

        \param dim
            DimensionType value indicating what this UnifiedDim is to represent.  This is required
            because we need to know what part of the base Window that the UDim scale component is
            to operate against.
        */
        UnifiedDim(const UDim& value, DimensionType dim);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
        BaseDim* clone_impl() const;

    private:
        UDim d_value;           //!< The UDim value.
        DimensionType d_what;   //!< what we represent.
    };

    /*!
    \brief
        Dimension type that represents some metric of a Font.  Implements BaseDim interface.
    */
    class CEGUIEXPORT FontDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param name
            String holding the name suffix of the window to be accessed to obtain the font
            and / or text strings to be used when these items are not explicitly given.

        \param font
            String holding the name of the font to use for this dimension.  If the string is
            empty, the font assigned to the window passed to getValue will be used.

        \param text
            String holding the text to be measured for horizontal extent.  If this is empty,
            the text from the window passed to getValue will be used.

        \param metric
            One of the FontMetricType values indicating what we should represent.

        \param padding
            constant pixel padding value to be added.
        */
        FontDim(const String& name, const String& font, const String& text, FontMetricType metric, float padding = 0);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
        BaseDim* clone_impl() const;

    private:
        String  d_font;          //!< Name of Font.  If empty font will be taken from Window.
        String  d_text;          //!< String to measure for extents, if empty will use window text.
        String  d_childName;     //!< String to hold the name of the window to use for fetching missing font and/or text.
        FontMetricType d_metric; //!< what metric we represent.
        float   d_padding;       //!< padding value to be added.
    };

    /*!
    \brief
        Dimension type that represents the value of a Window property.
        Implements BaseDim interface.
    */
    class CEGUIEXPORT PropertyDim : public BaseDim
    {
    public:
        /*!
        \brief
            Constructor.

        \param name
            String holding the name suffix of the window on which the property
            is to be accessed.

        \param property
            String object holding the name of the property this PropertyDim
            represents the value of.  The property named should represent either
            a UDim value or a simple float value - dependning upon what \a type
            is specified as.

        \param type
            DimensionType value indicating what dimension named property
            represents.  The possible DimensionType values are as follows:
            - DT_INVALID the property should represent a simple float value.
            - DT_WIDTH the property should represent a UDim value where the
            scale is relative to the targetted Window's width.
            - DT_HEIGHT the property should represent a UDim value where the
            scale is relative to the targetted Window's height.
            - All other values will cause an InvalidRequestException exception
            to be thrown.
        */
        PropertyDim(const String& name, const String& property, DimensionType type);

    protected:
        // Implementation of the base class interface
        float getValue_impl(const Window& wnd) const;
        float getValue_impl(const Window& wnd, const Rectf& container) const;
        void writeXMLElementName_impl(XMLSerializer& xml_stream) const;
        void writeXMLElementAttributes_impl(XMLSerializer& xml_stream) const;
        BaseDim* clone_impl() const;

    private:
        String d_property;      //!< Propery that this object represents.
        String d_childName;     //!< String to hold the name of the child to access the property form.
        DimensionType d_type;   //!< String to hold the type of dimension
    };

    /*!
    \brief
        Class representing some kind of dimension.

        The key thing to understand about Dimension is that it contains not just a dimensional value,
        but also a record of what the dimension value is supposed to represent. (e.g. a co-ordinate on
        the x axis, or the height of something).
    */
    class CEGUIEXPORT Dimension
    {
    public:
        /*!
        \brief
            Constructor
        */
        Dimension();

        /*!
        \brief
            Destructor
        */
        ~Dimension();

        /*!
        \brief
            Constructor

        \param dim
            object based on subclass of BaseDim which holds the dimensional value.

        \param type
            DimensionType value indicating what dimension this object is to represent.
        */
        Dimension(const BaseDim& dim, DimensionType type);

        /*!
        \brief
            Copy constructor
        */
        Dimension(const Dimension& other);

        /*!
        \brief
            Assignment operator
        */
        Dimension& operator=(const Dimension& other);

        /*!
        \brief
            return the BaseDim object currently used as the value for this Dimension.

        \return
            const reference to the BaseDim sub-class object which contains the value for this Dimension.
        */
        const BaseDim& getBaseDimension() const;

        /*!
        \brief
            set the current value for this Dimension.

        \param dim
            object based on a subclass of BaseDim which holds the dimensional value.

        \return
            Nothing.
        */
        void setBaseDimension(const BaseDim& dim);

        /*!
        \brief
            Return a DimensionType value indicating what this Dimension represents.

        \return
            one of the DimensionType enumerated values.
        */
        DimensionType getDimensionType() const;

        /*!
        \brief
            Sets what this Dimension represents.

        \param type
            one of the DimensionType enumerated values.

        \return
            Nothing.
        */
        void setDimensionType(DimensionType type);

        /*!
        \brief
            Writes an xml representation of this Dimension to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.

        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

    private:
        BaseDim*        d_value;    //!< Pointer to the value for this Dimension.
        DimensionType   d_type;     //!< What we represent.
    };


    /*!
    \brief
        Class that represents a target area for a widget or imagery component.

        This is essentially a Rect built out of Dimension objects.  Of note is that
        what would normally be the 'right' and 'bottom' edges may alternatively
        represent width and height depending upon what the assigned Dimension(s)
        represent.
    */
    class CEGUIEXPORT ComponentArea
    {
    public:
        ComponentArea();

        /*!
        \brief
            Return a Rect describing the absolute pixel area represented by this ComponentArea.

        \param wnd
            Window object to be used when calculating final pixel area.

        \return
            Rect object describing the pixels area represented by this ComponentArea when using \a wnd
            as a reference for calculating the final pixel dimensions.
        */
        Rectf getPixelRect(const Window& wnd) const;

        /*!
        \brief
            Return a Rect describing the absolute pixel area represented by this ComponentArea.

        \param wnd
            Window object to be used when calculating final pixel area.

        \param container
            Rect object to be used as a base or container when converting relative dimensions.

        \return
            Rect object describing the pixels area represented by this ComponentArea when using \a wnd
            and \a container as a reference for calculating the final pixel dimensions.
        */
        Rectf getPixelRect(const Window& wnd, const Rectf& container) const;

        /*!
        \brief
            Writes an xml representation of this ComponentArea to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

        /*!
        \brief
            Return whether this ComponentArea fetches it's area via a property on the target window.

        \return
            - true if the area comes via a Propery.
            - false if the area is not sourced from a Property.
        */
        bool isAreaFetchedFromProperty() const;

        /*!
        \brief
            Return the name of the property that will be used to determine the pixel area for this ComponentArea.

        \return
            String object holding the name of a Propery.
        */
        const String& getAreaPropertySource() const;

        /*!
        \brief
            Set the name of the property that will be used to determine the pixel area for this ComponentArea.

        \note
            Calling this will replace any existing souce, such as a named area.

        \param property
            String object holding the name of a Propery.  The property should access a URect type property.

        \return
            Nothing.
        */
        void setAreaPropertySource(const String& property);

        //! Set the named area source of the ComponentArea.
        void setNamedAreaSouce(const String& widget_look, const String& area_name);

        /*!
        \brief
            Return whether this ComponentArea fetches it's area via a named area
            defined.

        \return
            - true if the area comes via a named area defined in a WidgetLook.
            - false if the area is not sourced from a named area.
        */
        bool isAreaFetchedFromNamedArea() const;

        Dimension d_left;   //!< Left edge of the area.
        Dimension d_top;    //!< Top edge of the area.
        Dimension d_right_or_width;     //!< Either the right edge or the width of the area.
        Dimension d_bottom_or_height;   //!< Either the bototm edge or the height of the area.

    private:
        //! name of property or named area.  Property must access a URect style value.
        String d_namedSource;
        //! name of widget look holding the named area to fetch
        String d_namedAreaSourceLook;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalDimensions_h_
