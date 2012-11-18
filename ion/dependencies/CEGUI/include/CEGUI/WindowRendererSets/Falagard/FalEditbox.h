/***********************************************************************
    filename:   FalEditbox.h
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _FalEditbox_h_
#define _FalEditbox_h_

#include "FalModule.h"
#include "../../elements/CEGUIEditbox.h"
#include "FalEditboxProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Editbox class for the FalagardBase module.

    This class requires LookNFeel to be assigned.  The LookNFeel should provide
    the following:

    States:
        - Enabled: Rendering for when the editbox is in enabled and is in
                   read-write mode.
        - ReadOnly: Rendering for when the editbox is in enabled and is in
                    read-only mode.
        - Disabled: Rendering for when the editbox is disabled.
        - ActiveSelection: additional state rendered for text selection
                           (the imagery in this section is rendered within the
                           selection area.)
        - InactiveSelection: additional state rendered for text selection
                             (the imagery in this section is rendered within the
                             selection area.)

    NamedAreas:
        - TextArea: area where text, selection, and carat imagery will appear.

    PropertyDefinitions (optional)
        - NormalTextColour: property that accesses a colour value to be used to
                            render normal unselected text.  If this property is
                            not defined, the colour defaults to black.
        - SelectedTextColour: property that accesses a colour value to be used
                              to render selected text.  If this property is
                              not defined, the colour defaults to black.

    Imagery Sections:
        - Carat
*/
class FALAGARDBASE_API FalagardEditbox : public EditboxWindowRenderer
{
public:
    //! type name for this widget.
    static const utf8 TypeName[];
    //! Name of property to access for unselected text colour.
    static const String UnselectedTextColourPropertyName;
    //! Name of property to access for selected text colour.
    static const String SelectedTextColourPropertyName;
    //! The default timeout (in seconds) used when blinking the caret.
    static const float DefaultCaretBlinkTimeout;

    /*!
    \brief
        Constructor
    */
    FalagardEditbox(const String& type);

    /*!
    \brief
        return the colour to be used for rendering Editbox text oustside of the
        selected region.

    \return
        colour value describing the colour to be used.
    */
    colour getUnselectedTextColour() const;

    /*!
    \brief
        return the colour to be used for rendering Editbox text falling within
        the selected region.

    \return
        colour value describing the colour to be used.
    */
    colour getSelectedTextColour() const;

    /*!
    \brief
        Return a colour object fetched from the named property if it exists,
        else a default colour (black).

    \param propertyName
        String object holding the name of the property to be accessed if it
        exists.
    */
    colour getOptionalPropertyColour(const String& propertyName) const;

    //! return whether the blinking caret is enabled.
    bool isCaretBlinkEnabled() const;
    //! return the caret blink timeout period (only used if blink is enabled).
    float getCaretBlinkTimeout() const;
    //! set whether the blinking caret is enabled.
    void setCaretBlinkEnabled(bool enable);
    //! set the caret blink timeout period (only used if blink is enabled).
    void setCaretBlinkTimeout(float seconds);

    /*!
    \brief
        Sets the horizontal text formatting to be used from now onwards.

    \param format
        Specifies the formatting to use.  Currently can only be one of the
        following HorizontalTextFormatting values:
            - HTF_LEFT_ALIGNED (default)
            - HTF_RIGHT_ALIGNED
            - HTF_CENTRE_ALIGNED
    */
    void setTextFormatting(const HorizontalTextFormatting format);
    HorizontalTextFormatting getTextFormatting() const;

    void render();

    // overridden from EditboxWindowRenderer base class.
    size_t getTextIndexFromPosition(const Point& pt) const;
    // overridden from WindowRenderer class
    void update(float elapsed);

protected:
    // properties
    static FalagardEditboxProperties::BlinkCaret d_blinkCaretProperty;
    static FalagardEditboxProperties::BlinkCaretTimeout d_blinkCaretTimeoutProperty;
    static FalagardEditboxProperties::TextFormatting d_textFormattingProperty;

    //! helper to draw the base imagery (container and what have you)
    void renderBaseImagery(const WidgetLookFeel& wlf) const;
    //! helper to set 'visual' to the string we will render (part of)
    void setupVisualString(String& visual) const;
    size_t getCaretIndex(const String& visual_string) const;
    float calculateTextOffset(const Rect& text_area,
                              const float text_extent,
                              const float caret_width,
                              const float extent_to_caret);
    void renderTextNoBidi(const WidgetLookFeel& wlf,
                          const String& text,
                          const Rect& text_area,
                          float text_offset);
    void renderTextBidi(const WidgetLookFeel& wlf,
                        const String& text,
                        const Rect& text_area,
                        float text_offset);
    bool editboxIsFocussed() const;
    void renderCaret(const ImagerySection& imagery,
                     const Rect& text_area,
                     const float text_offset,
                     const float extent_to_caret) const;

    bool isUnsupportedFormat(const HorizontalTextFormatting format);

    //! x rendering offset used last time we drew the widget.
    float d_lastTextOffset;
    //! true if the caret imagery should blink.
    bool d_blinkCaret;
    //! time-out in seconds used for blinking the caret.
    float d_caretBlinkTimeout;
    //! current time elapsed since last caret blink state change.
    float d_caretBlinkElapsed;
    //! true if caret should be shown.
    bool d_showCaret;
    //! horizontal formatting.  Only supports left, right, and centred.
    HorizontalTextFormatting d_textFormatting;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalEditbox_h_
