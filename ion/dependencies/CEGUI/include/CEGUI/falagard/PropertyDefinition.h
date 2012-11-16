/***********************************************************************
    filename:   CEGUIPropertyDefinition.h
    created:    Sun Jun 26 2005
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
#ifndef _CEGUIFalPropertyDefinition_h_
#define _CEGUIFalPropertyDefinition_h_

#include "./PropertyDefinitionBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class representing a generic get/set property.
    */
    class CEGUIEXPORT PropertyDefinition : public PropertyDefinitionBase
    {
    public:
        /*!
        \deprecated
            This version of the constructor is deprecated in favour of the
            version taking a help string.
        */
        PropertyDefinition(const String& name, const String& initialValue, bool redrawOnWrite, bool layoutOnWrite);

        //! Constructor.
        PropertyDefinition(const String& name, const String& initialValue,
                           const String& help, bool redrawOnWrite,
                           bool layoutOnWrite);

        // abstract members from Property
        String get(const PropertyReceiver* receiver) const;
        void set(PropertyReceiver* receiver, const String& value);

    protected:
        void writeXMLElementType(XMLSerializer& xml_stream) const;

        String d_userStringName;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalPropertyDefinition_h_
