/***********************************************************************
	filename: 	CEGUIScrollablePaneProperties.h
	created:	3/3/2005
	author:		Paul D Turner
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
#ifndef _CEGUIScrollablePaneProperties_h_
#define _CEGUIScrollablePaneProperties_h_

#include "../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of ScrollablePaneProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ScrollablePane class
*/
namespace ScrollablePaneProperties
{
    /*!
    \brief
        Property to access the setting which controls whether the content pane is automatically
        resized according to the size and position of attached content.

    \par Usage:
        - Name: ContentPaneAutoSized
        - Format: "[text]"

    \par Where [Text] is:
        - "True" to indicate the pane should automatically resize itself.
        - "False" to indicate the pane should not automatically resize itself.
    */
    class ContentPaneAutoSized : public Property
    {
    public:
        ContentPaneAutoSized() : Property(
            "ContentPaneAutoSized",
            "Property to get/set the setting which controls whether the content pane will auto-size itself.  Value is either \"True\" or \"False\".",
            "True")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
       Property to access the current content pane area rectangle (as window relative pixels).

    \par Usage:
        - Name: ContentArea
        - Format: "l:[float] t:[float] r:[float] b:[float]".

    \par Where:
        - l:[float]	specifies the position of the left edge of the area as a floating point number.
        - t:[float]	specifies the position of the top edge of the area as a floating point number.
        - r:[float]	specifies the position of the right edge of the area as a floating point number.
        - b:[float]	specifies the position of the bottom edge of the area as a floating point number.
    */
    class ContentArea : public Property
    {
    public:
        ContentArea() : Property(
            "ContentArea",
            "Property to get/set the current content area rectangle of the content pane.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom).",
            "l:0.000000 t:0.000000 r:0.000000 b:0.000000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the setting which controls whether the vertical scroll bar will 
        always be displayed, or only displayed when it is required.

    \par Usage:
        - Name: ForceVertScrollbar
        - Format: "[text]"

    \par Where [Text] is:
        - "True" to indicate that the vertical scroll bar will always be shown.
        - "False" to indicate that the vertical scroll bar will only be shown when it is needed.
    */
    class ForceVertScrollbar : public Property
    {
    public:
        ForceVertScrollbar() : Property(
            "ForceVertScrollbar",
            "Property to get/set the setting which controls whether the vertical scroll bar is aways shown.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };


    /*!
    \brief
        Property to access the setting which controls whether the horizontal scroll bar will 
        always be displayed, or only displayed when it is required.

    \par Usage:
        - Name: ForceHorzScrollbar
        - Format: "[text]"

    \par Where [Text] is:
        - "True" to indicate that the horizontal scroll bar will always be shown.
        - "False" to indicate that the horizontal scroll bar will only be shown when it is needed.
    */
    class ForceHorzScrollbar : public Property
    {
    public:
        ForceHorzScrollbar() : Property(
            "ForceHorzScrollbar",
            "Property to get/set the setting which controls whether the horizontal scroll bar is aways shown.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the step size for the horizontal Scrollbar.

    \par Usage:
        - Name: HorzStepSize
        - Format: "[float]".

    \par Where:
        - [float] specifies the size of the increase/decrease button step for the horizontal scrollbar (as a fraction of 1 page).
    */
    class HorzStepSize : public Property
    {
    public:
        HorzStepSize() : Property(
            "HorzStepSize",
            "Property to get/set the step size for the horizontal Scrollbar.  Value is a float.",
            "0.100000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the overlap size for the horizontal Scrollbar.

    \par Usage:
        - Name: HorzOverlapSize
        - Format: "[float]".

    \par Where:
        - [float] specifies the size of the per-page overlap (as a fraction of one page).
    */
    class HorzOverlapSize : public Property
    {
    public:
        HorzOverlapSize() : Property(
            "HorzOverlapSize",
            "Property to get/set the overlap size for the horizontal Scrollbar.  Value is a float.",
            "0.010000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the scroll position of the horizontal Scrollbar.

    \par Usage:
        - Name: HorzScrollPosition
        - Format: "[float]".

    \par Where:
        - [float] specifies the current scroll position / value of the horizontal Scrollbar (as a fraction of the whole).
    */
    class HorzScrollPosition : public Property
    {
    public:
        HorzScrollPosition() : Property(
            "HorzScrollPosition",
            "Property to get/set the scroll position of the horizontal Scrollbar as a fraction.  Value is a float.",
            "0.000000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the step size for the vertical Scrollbar.

    \par Usage:
        - Name: VertStepSize
        - Format: "[float]".

    \par Where:
        - [float] specifies the size of the increase/decrease button step for the vertical scrollbar (as a fraction of 1 page).
    */
    class VertStepSize : public Property
    {
    public:
        VertStepSize() : Property(
            "VertStepSize",
            "Property to get/set the step size for the vertical Scrollbar.  Value is a float.",
            "0.100000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the overlap size for the vertical Scrollbar.

    \par Usage:
        - Name: VertOverlapSize
        - Format: "[float]".

    \par Where:
        - [float] specifies the size of the per-page overlap (as a fraction of one page).
    */
    class VertOverlapSize : public Property
    {
    public:
        VertOverlapSize() : Property(
            "VertOverlapSize",
            "Property to get/set the overlap size for the vertical Scrollbar.  Value is a float.",
            "0.010000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the scroll position of the vertical Scrollbar.

    \par Usage:
        - Name: VertScrollPosition
        - Format: "[float]".

    \par Where:
        - [float] specifies the current scroll position / value of the vertical Scrollbar (as a fraction of the whole).
    */
    class VertScrollPosition : public Property
    {
    public:
        VertScrollPosition() : Property(
            "VertScrollPosition",
            "Property to get/set the scroll position of the vertical Scrollbar as a fraction.  Value is a float.",
            "0.000000")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

} // End of  ScrollablePaneProperties namespace section
} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIScrollablePaneProperties_h_
