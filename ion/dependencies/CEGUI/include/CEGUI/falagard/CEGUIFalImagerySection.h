/***********************************************************************
    filename:   CEGUIFalImagerySection.h
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
#ifndef _CEGUIFalImagerySection_h_
#define _CEGUIFalImagerySection_h_

#include "CEGUIFalImageryComponent.h"
#include "CEGUIFalTextComponent.h"
#include "CEGUIFalFrameComponent.h"
#include "../CEGUIXMLSerializer.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates a re-usable collection of imagery specifications.
    */
    class CEGUIEXPORT ImagerySection
    {
    public:
        /*!
        \brief
            Constructor.
        */
        ImagerySection();

        /*!
        \brief
            ImagerySection constructor.  Name must be supplied, masterColours are set to 0xFFFFFFFF by default.

        \param name
            Name of the new ImagerySection.
        */
        ImagerySection(const String& name);

        /*!
        \brief
            Render the ImagerySection.

        \param srcWindow
            Window object to be used when calculating pixel values from BaseDim values.

        \param modColours
            ColourRect specifying colours to be modulated with the ImagerySection's master colours.  May be 0.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const CEGUI::ColourRect* modColours = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Render the ImagerySection.

        \param srcWindow
            Window object to be used when calculating pixel values from BaseDim values.

        \param baseRect
            Rect object to be used when calculating pixel values from BaseDim values.

        \param modColours
            ColourRect specifying colours to be modulated with the ImagerySection's master colours.  May be 0.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect& baseRect, const CEGUI::ColourRect* modColours = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Add an ImageryComponent to this ImagerySection.

        \param img
            ImageryComponent to be added to the section (a copy is made)

        \return
            Nothing
        */
        void addImageryComponent(const ImageryComponent& img);

        /*!
        \brief
            Clear all ImageryComponents from this ImagerySection.

        \return
            Nothing
        */
        void clearImageryComponents();

        /*!
        \brief
            Add a TextComponent to this ImagerySection.

        \param text
            TextComponent to be added to the section (a copy is made)

        \return
            Nothing
        */
        void addTextComponent(const TextComponent& text);

        /*!
        \brief
            Clear all TextComponents from this ImagerySection.

        \return
            Nothing
        */
        void clearTextComponents();

        /*!
        \brief
            Clear all FrameComponents from this ImagerySection.

        \return
            Nothing
        */
        void clearFrameComponents();

        /*!
        \brief
            Add a FrameComponent to this ImagerySection.

        \param frame
            FrameComponent to be added to the section (a copy is made)

        \return
            Nothing
        */
        void addFrameComponent(const FrameComponent& frame);

        /*!
        \brief
            Return the current master colours set for this ImagerySection.

        \return
            ColourRect describing the master colour values in use for this ImagerySection.
        */
        const ColourRect& getMasterColours() const;

        /*!
        \brief
            Set the master colours to be used for this ImagerySection.

        \param cols
            ColourRect describing the colours to be set as the master colours for this ImagerySection.

        \return
            Nothing.
        */
        void setMasterColours(const ColourRect& cols);

        /*!
        \brief
            Return the name of this ImagerySection.

        \return
            String object holding the name of the ImagerySection.
        */
        const String& getName() const;

        /*!
        \brief
            Set the name of the property where master colour values can be obtained.

        \param property
            String containing the name of the property.

        \return
            Nothing.
        */
        void setMasterColoursPropertySource(const String& property);

        /*!
        \brief
            Set whether the master colours property source represents a full ColourRect.

        \param setting
            - true if the master colours property will access a ColourRect object.
            - false if the master colours property will access a colour object.

        \return
            Nothing.
        */
        void setMasterColoursPropertyIsColourRect(bool setting = true);

        /*!
        \brief
            Return smallest Rect that could contain all imagery within this section.
        */
        Rect getBoundingRect(const Window& wnd) const;

        /*!
        \brief
            Return smallest Rect that could contain all imagery within this section.
        */
        Rect getBoundingRect(const Window& wnd, const Rect& rect) const;

        /*!
        \brief
            Writes an xml representation of this ImagerySection to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;


        //! return number of TextComponents in the ImagerySection.
        size_t getTextComponentCount() const;

        //! return a reference to a TextComponent (via index).
        const TextComponent& getTextComponent(const size_t idx) const;

    protected:
        /*!
        \brief
            Helper method to initialise a ColourRect with appropriate values according to the way the
            ImagerySection is set up.

            This will try and get values from multiple places:
                - a property attached to \a wnd
                - or the integral d_masterColours value.
        */
        void initMasterColourRect(const Window& wnd, ColourRect& cr) const;

    private:
        typedef std::vector<ImageryComponent> ImageryList;
        typedef std::vector<TextComponent> TextList;
        typedef std::vector<FrameComponent> FrameList;

        CEGUI::String       d_name;             //!< Holds the name of the ImagerySection.
        CEGUI::ColourRect   d_masterColours;    //!< Naster colours for the the ImagerySection (combined with colours of each ImageryComponent).
        FrameList           d_frames;           //!< Collection of FrameComponent objects to be drawn for this ImagerySection.
        ImageryList         d_images;           //!< Collection of ImageryComponent objects to be drawn for this ImagerySection.
        TextList            d_texts;            //!< Collection of TextComponent objects to be drawn for this ImagerySection.
        String              d_colourPropertyName;   //!< name of property to fetch colours from.
        bool                d_colourProperyIsRect;  //!< true if the colour property will fetch a full ColourRect.
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalImagerySection_h_
