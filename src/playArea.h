//----------------------------------------------------------------------
//--
//--    playArea.h
//--
//--        Definition of playArea object and consts for casio fx-CG50
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_FXCG50_h__
#define __GEE_TETRIS_CASIO_FXCG50_h__    1

#include "shared/keys.h"
#include "consts.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//
// Constants
//

// Box dimensions in pixels
//

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
  KEY_ID_RIGHT, KEY_ID_LEFT,
  KEY_ID_ROTATE, KEY_ID_DOWN,
  KEY_ID_FALL, KEY_ID_PAUSE,
  KEY_ID_ROTATE_DISPLAY, KEY_ID_QUIT,
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

// playArea_free() : Free | delete the playArea
//
//  @area : pointer to an area
//
void playArea_free(PPLAYAREA area);

// defaultFont() : return to default font
//
void playArea_defaultFont();

// playArea_shitfToZone() : Change the origin and the coordinate system
//          according to a selected drawing zone
//
//  @area : Pointer to the area
//  @zoneID  : ID of the desintation zone
//  @x, @y [i/o] : coordinates to change
//  @width, @height : (new) dimensions in pixels of a single block in the choosen area
//
void playArea_shitfToZone(PPLAYAREA const area, uint8_t zoneID, uint16_t* x, uint16_t* y,
    uint16_t* width, uint16_t* height);

// playArea_rotateDisplay() : Update members on rotation
//
//  @area : Pointer to the area
//  @orientation : New area orientation
//
void playArea_rotateDisplay(PPLAYAREA const area, CALC_ORIENTATION orientation);

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

// playArea_valtoa() : Transform a numeric value into a string
//
//  This specific method creates a string composed of the name of the value
//  and the value it self. It is equivalent to a sprintf(out, "%s : %d", name, value)
//
//  The base can't be changed it is always equal to 10
//
//  This method assumes the output buffer - ie. str - is large enough to contain
//  the name and the formated value.
//
//  @num : Numeric value to transform
//  @name : Name of the value (can be NULL)
//  @str : Pointer to output string
//  @rLength : Align to right ?
//
//  @return : pointer to formated string
//
char* playArea_valtoa(int num, const char* name, char* str, size_t rLength);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_FXCG50_h__

// EOF
