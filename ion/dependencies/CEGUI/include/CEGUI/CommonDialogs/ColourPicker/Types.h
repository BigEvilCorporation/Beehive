/***********************************************************************
    filename:   CEGUIColourPickerConversions.h
    created:    20th February 2010
    author:     Lukas E Meindl

    purpose:    Header of the ColourPicker colour type classes
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef CEGUI_COLOUR_PICKER_TYPES_H
#define CEGUI_COLOUR_PICKER_TYPES_H

#include "CEGUI/CommonDialogs/Module.h"
#include "CEGUI/Window.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


enum ColourPickerSliderMode
{
    ColourPickerSliderMode_L,
    ColourPickerSliderMode_A,
    ColourPickerSliderMode_B
};

class Position
{
public:
    Position(float xPosition, float yPosition) :
        x(xPosition), y(yPosition)
    {}

    Position()
    {}

    float x;
    float y;
};

namespace CEGUI
{

class CEGUI_COMMONDIALOGS_API Lab_Colour;
class CEGUI_COMMONDIALOGS_API RGB_Colour;
class CEGUI_COMMONDIALOGS_API HSV_Colour;

class CEGUI_COMMONDIALOGS_API RGB_Colour
{
public:
    RGB_Colour(unsigned char red, unsigned char green, unsigned char blue) :
        r(red), g(green), b(blue)
    {}

    RGB_Colour() {}
    RGB_Colour(const Lab_Colour& colour);
    RGB_Colour(const HSV_Colour& colour);
    RGB_Colour(const CEGUI::Colour& colour);

    unsigned char   r;
    unsigned char   g;
    unsigned char   b;

    RGB_Colour operator*(const float& number) const;
    RGB_Colour operator+(const RGB_Colour& colour) const;
};

class CEGUI_COMMONDIALOGS_API Lab_Colour
{
public:
    Lab_Colour(float LValue, float aValue, float bValue) :
        L(LValue), a(aValue), b(bValue)
    {}

    Lab_Colour() {}
    Lab_Colour(const RGB_Colour& colour);
    Lab_Colour(const HSV_Colour& colour);
    Lab_Colour(const CEGUI::Colour& colour);


    float L;
    float a;
    float b;
};

class CEGUI_COMMONDIALOGS_API HSV_Colour
{
public:
    HSV_Colour(float HValue, float SValue, float VValue) :
        H(HValue), S(SValue), V(VValue)
    {}

    HSV_Colour() {}
    HSV_Colour(const RGB_Colour& colour);
    HSV_Colour(const Lab_Colour& colour);
    HSV_Colour(const CEGUI::Colour& colour);

    float H;
    float S;
    float V;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

