/***********************************************************************
    filename:   FalMultiLineEditbox.h
    created:    Thu Jul 7 2005
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
#ifndef _FalMultiLineEditbox_h_
#define _FalMultiLineEditbox_h_

#include "FalModule.h"
#include "../../elements/CEGUIMultiLineEditbox.h"
#include "FalMultiLineEditboxProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    MultiLineEditbox class for the FalagardBase module.

    This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

    States:
        - Enabled    - Rendering for when the editbox is in enabled and is in read-write mode.
        - ReadOnly  - Rendering for when the editbox is in enabled and is in read-only mode.
        - Disabled  - Rendering for when the editbox is disabled.

    NamedAreas:
        TextArea         - area where text, selection, and carat imagery will appear.
        TextAreaHScroll  - TextArea when only horizontal scrollbar is visible.
        TextAreaVScroll  - TextArea when only vertical scrollbar is visible.
        TextAreaHVScroll - TextArea when both horizontal and vertical scrollbar is visible.

    PropertyDefinitions (optional, defaults will be black):
        - NormalTextColour        - property that accesses a colour value to be used to render normal unselected text.
        - SelectedTextColour      - property that accesses a colour value to be used to render selected text.
        - ActiveSelectionColour   - property that accesses a colour value to be used to render active selection highlight.
        - InactiveSelectionColour - property that accesses a colour value to be used to render inactive selection highlight.

    Imagery Sections:
        - Carat

    Child Widgets:
        Scrollbar based widget with name suffix "__auto_vscrollbar__"
        Scrollbar based widget with name suffix "__auto_hscrollbar__"

*/
class FALAGARDBASE_API FalagardMultiLineEditbox : public MultiLineEditboxWindowRenderer
{
public:
    static const utf8   TypeName[];     //! type name for this widget.
    //! Name of property to use to obtain unselected text rendering colour.
    static const String UnselectedTextColourPropertyName;
    //! Name of property to use to obtain selected text rendering colour.
    static const String SelectedTextColourPropertyName;
    //! Name of property to use to obtain active selection rendering colour.
    static const String ActiveSelectionColourPropertyName;
    //! Name of property to use to obtain inactive selection rendering colour.
    static const String InactiveSelectionColourPropertyName;
    //! The default timeout (in seconds) used when blinking the caret.
    static const float DefaultCaretBlinkTimeout;

    /*!
    \brief
        Constructor
    */
    FalagardMultiLineEditbox(const String& type);

    // overridden from base classes.
    Rect getTextRenderArea(void) const;
    void render();
    void update(float elapsed);

    //! return whether the blinking caret is enabled.
    bool isCaretBlinkEnabled() const;
    //! return the caret blink timeout period (only used if blink is enabled).
    float getCaretBlinkTimeout() const;
    //! set whether the blinking caret is enabled.
    void setCaretBlinkEnabled(bool enable);
    //! set the caret blink timeout period (only used if blink is enabled).
    void setCaretBlinkTimeout(float seconds);

protected:
    /*!
    \brief
        Perform rendering of the widget control frame and other 'static' areas.  This
        method should not render the actual text.  Note that the text will be rendered
        to layer 4 and the selection brush to layer 3, other layers can be used for
        rendering imagery behind and infront of the text & selection..

    \return
        Nothing.
    */
    void cacheEditboxBaseImagery();

    /*!
    \brief
        Render the carat.

    \return
        Nothing
    */
    void cacheCaratImagery(const Rect& textArea);

    /*!
    \brief
        Render text lines.
    */
    void cacheTextLines(const Rect& dest_area);

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
        return the colour to be used for rendering the selection highlight
        when the editbox is active.

    \return
        colour value describing the colour to be used.
    */
    colour getActiveSelectionColour() const;

    /*!
    \brief
        return the colour to be used for rendering the selection highlight
        when the editbox is inactive.

    \return
        colour value describing the colour to be used.
    */
    colour getInactiveSelectionColour() const;

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

    // properties
    static FalagardMultiLineEditboxProperties::BlinkCaret d_blinkCaretProperty;
    static FalagardMultiLineEditboxProperties::BlinkCaretTimeout d_blinkCaretTimeoutProperty;

    //! true if the caret imagery should blink.
    bool d_blinkCaret;
    //! time-out in seconds used for blinking the caret.
    float d_caretBlinkTimeout;
    //! current time elapsed since last caret blink state change.
    float d_caretBlinkElapsed;
    //! true if caret should be shown.
    bool d_showCaret;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalMultiLineEditbox_h_
