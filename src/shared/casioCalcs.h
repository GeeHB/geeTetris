//---------------------------------------------------------------------------
//--
//--	File	: casioCalcs.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--            Define the binary's dest. format
//--
//---------------------------------------------------------------------------

#ifndef __GEE_CASIO_CALCS_h__
#define __GEE_CASIO_CALCS_h__    1

#ifndef LINUX_VER
#define DEST_CASIO_CALC        1   // Compile for a Casio calculator
#endif // #ifndef LINX_VER

#ifdef DEST_CASIO_CALC
// Specific includes for calculators
#include <gint/gint.h>
#include <gint/display.h>

// Screen dimensions in pixels
#define CASIO_WIDTH     DWIDTH
#define CASIO_HEIGHT    DHEIGHT
#else
// Screen dimensions in pixels for tests
#define CASIO_WIDTH     384
#define CASIO_HEIGHT    192
#endif // #ifdef DEST_CASIO_CALC

#include <cstdint>

// A few basic colours
//

#ifndef DEST_CASIO_CALC
// 24 bits RGB (for tests only)
#define C_RGB(r,g,b)      ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))
#endif // #ifndef DEST_CASIO_CALC

#ifdef FX9860G
enum DEF_COLOUR{
    COLOUR_BLACK   = C_BLACK,
    COLOUR_WHITE   = C_WHITE,
    COLOUR_RED     = C_DARK,
    COLOUR_GREEN   = C_LIGHT,
    COLOUR_YELLOW  = C_LIGHT,
    COLOUR_BLUE    = C_DARK,
    COLOUR_LT_BLUE = C_LIGHT,
    COLOUR_PURPLE  = C_DARK,
    COLOUR_CYAN    = C_LIGHT,
    COLOUR_ORANGE  = C_DARK,
    COLOUR_LT_GREY = C_LIGHT,
    COLOUR_GREY    = C_WHITE,
    COLOUR_DK_GREY = C_DARK,
    NO_COLOR      = -1
};
#else
enum DEF_COLOUR{
    COLOUR_BLACK   = C_RGB(0, 0, 0),
    COLOUR_WHITE   = C_RGB(31, 31, 31),
    COLOUR_RED     = C_RGB(31, 0, 0),
    COLOUR_GREEN   = C_RGB(0, 31, 0),
    COLOUR_YELLOW  = C_RGB(31, 31, 0),
    COLOUR_BLUE    = C_RGB(0, 0, 31),
    COLOUR_LT_BLUE = C_RGB(6, 6, 31),
    COLOUR_PURPLE  = C_RGB(31, 0, 31),
    COLOUR_CYAN    = C_RGB(0, 31, 31),
    COLOUR_ORANGE  = C_RGB(31, 16, 0),
    COLOUR_LT_GREY = C_RGB(29, 29, 29),
    COLOUR_GREY    = C_RGB(16, 16, 16),
    COLOUR_DK_GREY = C_RGB(8, 8, 8),
    NO_COLOR      = -1
};
#endif // #ifdef FX9860G

// Point coordinates
//
typedef struct __point {
    // Construction
    __point(){
        x = y = 0;
    }

    __point(int px, int py){
        x = px;
        y = py;
    }

    uint16_t    x;
    uint16_t    y;
} POINT;

// Position of a tab
//
typedef struct __rect{
    uint16_t    x,y;    // top left
    uint16_t    w, h;   // width and height
} RECT;

#endif // #ifndef __GEE_CASIO_CALCS_h__

// EOF
