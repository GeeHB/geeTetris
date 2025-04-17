//----------------------------------------------------------------------
//--
//--    playArea.h
//--
//--        Definition of playArea object and consts for casio fx-CG50
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_FXCG50_h__
#define __GEE_TETRIS_CASIO_FXCG50_h__    1

#include "consts.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//
// Constants
//

// Box dimensions in pixels
//

#ifdef FX9860G
#define CASIO_BOX_WIDTH           3
#define CASIO_BOX_WIDTH_ROTATED   5

#define CASIO_BOX_WIDTH_NP          CASIO_BOX_WIDTH    // next piece preview
#define CASIO_BOX_WIDTH_NP_ROTATED  3

// Playfield pos & dims
//
#define CASIO_PLAYFIELD_LEFT    0
#define CASIO_PLAYFIELD_BORDER  1
#define CASIO_BORDER_GAP        1

// Texts pos & dims
//
#define CASIO_INFO_LEFT         65
#define CASIO_INFO_TOP          1
#define CASIO_INFO_GAP          1       // between border and text
#define CASIO_INFO_DY           12      // Between each line of text
#else
#define CASIO_BOX_WIDTH           9
#define CASIO_BOX_WIDTH_ROTATED   14

#define CASIO_BOX_WIDTH_NP          CASIO_BOX_WIDTH    // next piece preview
#define CASIO_BOX_WIDTH_NP_ROTATED  7

// Playfield pos & dims
//
#define CASIO_PLAYFIELD_LEFT    100
#define CASIO_PLAYFIELD_BORDER  3
#define CASIO_BORDER_GAP        2

// Texts pos & dims
//
#define CASIO_INFO_LEFT     250
#define CASIO_INFO_TOP      5
#define CASIO_INFO_GAP      4       // between border and text
#define CASIO_INFO_DY       23
#endif // #ifdef FX9860G

// 2 zones to draw in :
//      game zone and next-piece (ie. preview) zone
//
typedef struct __zone{
    uint8_t    boxWidth;    // Single box width in pixels
    RECT       pos;         // Position and dimensions
} ZONE, * PZONE;

// Prefined zones
enum ZONE_ID{
    ZONE_GAME = 0,
    ZONE_PLAYFIELD = 0,
    ZONE_NEXTPIECE = 1,
    ZONE_PREVIEW = 1,
    ZONE_COUNT
};

// Keys index
enum KEY_ID{
  KEY_RIGHT, KEY_LEFT,
  KEY_ROTATE, KEY_DOWN,
  KEY_FALL, KEY_PAUSE,
  KEY_ROTATE_DISPLAY, KEY_QUIT,
  KEY_COUNT
};

//
// playArea
//
//  Text and rect. drawing in both modes (vertical et horizontal)
//
typedef struct __playArea{
    POINT texts[VAL_COUNT];      // Positions of texts
    char keys[KEY_COUNT];
    CALC_ORIENTATION orientation;
    ZONE zones[ZONE_COUNT];
#ifdef DEST_CASIO_CALC
    font_t* hFont, *vFont;
#endif //#ifdef DEST_CASIO_CALC
} playArea, PLAYAREA, * PPLAYAREA;

// playArea_init() : Intialize the playArea
//
//  @area : pointer to an area
//
//  @return : TRUE if successfull
//
BOOL playArea_init(PPLAYAREA area);

// playArea_clear() : Clear | delete the playArea
//
//  @area : pointer to an area
//
void playArea_clear(PPLAYAREA area);

// defaultFont() : return to default font
//
void playArea_defaultFont();

// playArea_clearScreen() : Clear the screen with the given colour
//
//  @color : color to use to fill the screen
//
void playArea_clearScreen(int color);

// playArea_rotatedDisplay() : Update members on rotation
//
//  @area : Pointer to the area
//  @orientation : New area orientation
//
void playArea_rotatedDisplay(PPLAYAREA const area, CALC_ORIENTATION orientation);

// playArea_dtext() : Draw a line of text horizontally or vertically
//          according to the display orientation
//
//  @area : Pointer to the area
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea_dtext(PPLAYAREA const area, int x, int y, int fg, const char* text);

// playArea_dtextV() : Draw a line of text vertically
//
// used when calc orientation is HORIZONTAL ...
//
//  @area : Pointer to the area
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea_dtextV(PPLAYAREA const area, int x, int y, int fg, const char* text);

// playArea_drawRectangle() : Draw a single coloured rectangle
//
//  @area : Pointer to the area
//  @x,@y : top left starting point
//  @width, @height : dimensions
//  @fillColour : Filling colour or NO_COLOR (-1) if none
//  @borderColour : Colour of the border or NO_COLOR (-1) if none
//
void playArea_drawRectangle(PPLAYAREA const area, uint16_t x, uint16_t y, uint16_t width,
    uint16_t height, int fillColour, [[maybe_unused]]  int borderColour);

// playArea_drawBorder() : Draw a border around a rectangle
//
//  @area : Pointer to the area
//  @x,@y : top left starting point
//  @width, @height : dimensions
//  @borderColour : Colour of the border
//
void playArea_drawBorder(PPLAYAREA const area, uint16_t x, uint16_t y,
        uint16_t width, uint16_t height, int borderColour);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_FXCG50_h__

// EOF
