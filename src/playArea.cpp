//---------------------------------------------------------------------------
//--
//--	File	: playArea.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of playArea object
//--
//---------------------------------------------------------------------------

#include "playArea.h"

// Fonts
#ifdef DEST_CASIO_CALC
extern font_t font_horz;
extern font_t font_vert;
#endif // #ifdef DEST_CASIO_CALC

//---------------------------------------------------------------------------
//--
//-- playArea object
//--
//--    methods, constants (coordinates, dimensions) for the casio calculators
//--
//---------------------------------------------------------------------------

// Construction
//
playArea::playArea(){

    rotatedDisplay_ = false;

    // Fonts
#ifdef DEST_CASIO_CALC
    hFont_ = &font_horz;
    vFont_ = &font_vert;
#endif // #ifdef DEST_CASIO_CALC

    // Default keys
    keyFall_ = KEY_CODE_FALL;
    keyPause_ = KEY_CODE_PAUSE;
    keyRotateDisplay_ = KEY_CODE_ROTATE_DISPLAY;
    keyQuit_ = KEY_CODE_EXIT;

    // set parameters
    _rotatedDisplay(rotatedDisplay_, true);
}

// Destruction
//
playArea::~playArea(){
    // Return to default font
#ifdef DEST_CASIO_CALC
    dfont(dfont_default());
#endif // #ifdef DEST_CASIO_CALC
}

// rotatedDisplay() : Update members on rotation
//
//  @doRotate : indicates wether display must rotate or not
//
void playArea::_rotatedDisplay(bool doRotate, bool force){
    if (force || rotatedDisplay_ != doRotate){
        if (false == (rotatedDisplay_ = doRotate)){
            playfield_.boxWidth = CASIO_BOX_WIDTH;

            playfield_.pos.x = CASIO_PLAYFIELD_LEFT + CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            playfield_.pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            playfield_.pos.w = PLAYFIELD_WIDTH * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;
            playfield_.pos.h = PLAYFIELD_HEIGHT * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;

            // Nextpiece zone dims
            nextPiece_.boxWidth = CASIO_BOX_WIDTH_NP;
            nextPiece_.pos.x = CASIO_INFO_LEFT + CASIO_INFO_GAP;
            if (nextPiece_.pos.x <= (playfield_.pos.x + playfield_.pos.w)){
                nextPiece_.pos.x = playfield_.pos.x + playfield_.pos.w + 2 * CASIO_INFO_GAP;
            }
            nextPiece_.pos.y = CASIO_INFO_TOP;
            nextPiece_.pos.w = nextPiece_.pos.h = 4 * nextPiece_.boxWidth + 2 * CASIO_INFO_GAP;

            // Keys
            keyLeft_ = KEY_CODE_LEFT;
            keyRight_ = KEY_CODE_RIGHT;
            keyRotatePiece_ = KEY_CODE_UP;
            keyDown_ = KEY_CODE_DOWN;

            // Use "default" font
#ifdef DEST_CASIO_CALC
            dfont((font_t*)&font_horz);
#endif // #ifdef DEST_CASIO_CALC
        }
        else {
            // "rotated" mode
            //
            playfield_.boxWidth = CASIO_BOX_WIDTH_ROTATED;    // Larger box

            playfield_.pos.x = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            playfield_.pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            playfield_.pos.w = PLAYFIELD_WIDTH * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;
            playfield_.pos.h = PLAYFIELD_HEIGHT * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;

            // Nextpiece
            nextPiece_.boxWidth = CASIO_BOX_WIDTH_NP_ROTATED;  // box in preview is smaller
            nextPiece_.pos.w = nextPiece_.pos.h = 4 * nextPiece_.boxWidth + 2 * CASIO_INFO_GAP;
            nextPiece_.pos.x = playfield_.pos.x + playfield_.pos.w + CASIO_BORDER_GAP;
            nextPiece_.pos.y = CASIO_INFO_TOP;

            // Keys
            keyLeft_ = KEY_CODE_DOWN;
            keyRight_ = KEY_CODE_UP;
            keyRotatePiece_ = KEY_CODE_LEFT;
            keyDown_ = KEY_CODE_RIGHT;

            // Install my font
#ifdef DEST_CASIO_CALC
            dfont((font_t*)&font_vert);
#endif // #ifdef DEST_CASIO_CALC
        }

        // Values indicators
        textsPos_[0].x = textsPos_[1].x = textsPos_[2].x = nextPiece_.pos.x;
        for (uint8_t id(0); id <VAL_COUNT; id++){
            textsPos_[id].y = nextPiece_.pos.y + nextPiece_.pos.w + playfield_.boxWidth * ( 2 * id + 1);
        }
    }
}

// dtext() : Draw a line of text horizontally or vertically
//          according to the display orientation
//
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea::dtext(int x, int y, int fg, const char* text){
    if (isRotated()){
        _dtextV(x, y, fg, text);
    }
    else{
#ifdef DEST_CASIO_CALC
        ::dtext(x, y, fg, text);
#endif // #ifdef DEST_CASIO_CALC
    }
}

// drawRectangle() : Draw a single coloured rectangle
//
//   @x,@y : top left starting point
//   @width, @height : dimensions
//   @borderColour : Colour of the border or NO_COLOR (-1) if none
//   @fillColour : Filling colour or NO_COLOR (-1) if none
//
void playArea::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int fillColour, int borderColour){
    int16_t xFrom(x), yFrom(y);
    int16_t xTo(xFrom + width - 1), yTo(yFrom + height - 1);

    // Horizontal display ?
    if (isRotated()){
        rotate(xFrom, yFrom, xTo, yTo);
    }

    // Draw the rect
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
    // Coloured rect
    drect(xFrom, yFrom, xTo - 1, yTo - 1, fillColour);

    // Half a border (bottom and right) for "small" pieces
    if (NO_COLOR != borderColour){
        dline(xFrom, yTo, xTo, yTo, borderColour);
        dline(xTo, yFrom, xTo, yTo, borderColour);
    }
#else
    drect_border(xFrom, yFrom, xTo, yTo, fillColour, 1, borderColour);
#endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC
}

// drawBorder() : Draw a border around a rectangle
//
//   @x,@y : top left starting point
//   @width, @height : dimensions
//   @borderColour : Colour of the border
//
void playArea::drawBorder(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int borderColour){
    // Draw the rect
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
    int16_t xFrom(x), yFrom(y);
    int16_t xTo(xFrom + width - 1), yTo(yFrom + height - 1);

    // Horizontal display ?
    if (isRotated()){
        rotate(xFrom, yFrom, xTo, yTo);
    }

    drect_border(xFrom, yFrom, xTo, yTo, C_NONE, 1, borderColour);
#else
    drawRectangle(x, y, width, height, C_NONE, borderColour);
#endif // #ifdef FX9860G
#endif // #ifdef FX9860G
}

// _shitfToZone() : Change the origin and the coordinate system
//          according to a selected drawing zone
//
//  @zoneID  : ID of the desintation zone
//  @x, @y [i/o] : coordinates to change
//  @width, @height : (new) dimensions in pixels of a single block in the choosen area
//
void playArea::shitfToZone(uint8_t zoneID, uint16_t& x, uint16_t& y, uint16_t& width, uint16_t& height) {
    if (zoneID == ZONE_GAME){
        x = playfield_.pos.x + x * playfield_.boxWidth;
        y = playfield_.pos.y + (PLAYFIELD_HEIGHT - 1 - y) * playfield_.boxWidth;
        width = height = playfield_.boxWidth;
    }
    else{
        // Draw next piece
        x = nextPiece_.pos.x + CASIO_INFO_GAP;
        y = nextPiece_.pos.y + CASIO_INFO_GAP;
        width = height = nextPiece_.boxWidth;
    }
}

// _dtextV() : Draw a line of text vertically
//
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea::_dtextV(int x, int y, int fg, const char* text){
    if (strlen(text) > 0){
        int16_t xFrom(x), yFrom(y), xTo, yTo;

        // dimensions of the first char.
        char* current = (char*)text;
        int w, h;
#ifdef DEST_CASIO_CALC
        dnsize(current, 1, font(false), &w, &h);
#else
        w = h = 10; // for debug tests
#endif // #ifdef DEST_CASIO_CALC

        // Get new coordinates of the anchor
        xTo = xFrom + w;
        yTo = yFrom + h;
        rotate(xFrom, yFrom, xTo, yTo);

        // Draw the string (char. by char.)
        while (*current){
#ifdef DEST_CASIO_CALC
            dtext_opt(xFrom, yFrom,  fg, C_NONE, DTEXT_RIGHT, DTEXT_BOTTOM, current, 1);
            dnsize(current, 1, font(false), &w, &h);
#endif // #ifdef DEST_CASIO_CALC

            // Update anchor pos.
            yFrom-=h;

            // Next char
            current++;
        }
    }
}

// EOF
