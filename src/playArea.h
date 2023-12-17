//---------------------------------------------------------------------------
//--
//--	File	: playArea.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of playArea object and consts for casio fx-CG50
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_FXCG50_h__
#define __GEE_TETRIS_CASIO_FXCG50_h__    1

#include "consts.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//---------------------------------------------------------------------------
//--
//-- Constants
//--
//---------------------------------------------------------------------------


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
#define CASIO_INFO_TOP      10
#define CASIO_INFO_GAP      4       // between border and text
#endif // #ifdef FX9860G

// During games draings appends in 2 zones :
//      game zone and next-piece (ie. preview) zone
//
typedef struct __zone{
    uint8_t    boxWidth;    // Single box width in pixels
    RECT       pos;         // Position and dimensions
} ZONE;

// Prefined zones
enum ZONE_ID{
    ZONE_GAME = 0,
    ZONE_NEXTPIECE = 1,
    ZONE_PREVIEW = 1
};

//---------------------------------------------------------------------------
//--
//-- playArea object
//--
//--    methods, constants (coordinates, dimensions) for the casio calculators
//--
//---------------------------------------------------------------------------

class playArea{

    // Public methods
    //
    public:
        // Construction
        playArea();

        // Destruction
        ~playArea();

        // update() : Update display
        //
        void update(){
#ifdef DEST_CASIO_CALC
            dupdate();
#endif // #ifdef DEST_CASIO_CALC
        }

        // clear() : Clear the screen with the given colour
        //
        void clear(int color){
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
            dclear((color_t)color);
#else
            dclear(color);
#endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC
        }

        //
        // Displays orientations
        //

        // Update members on rotation
        void rotatedDisplay(bool doRotate){
            _rotatedDisplay(doRotate, false);
        }

        // (trigonometric) rotations
        //  ... of a single point
        void rotate(int16_t& x, int16_t& y);

        // ... of a rect
        void rotate(int16_t& xFrom, int16_t& yFrom, int16_t& xTo, int16_t& yTo);

        bool isRotated(){
            return rotatedDisplay_;
        }

        //
        // Drawings methods
        //

        // Draw a line of text
        void dtext(int x, int y, int fg, const char* text);

        // drawRectangle() : Draw a single coloured rectangle
        //
        //   @x,@y : top left starting point
        //   @width, @height : dimensions
        //   @borderColour : Colour of the border NO_COLOR (-1) if none
        //   @fillColour : Filling colour NO_COLOR (-1) if none
        //
        void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int fillColour = NO_COLOR, int borderColour = NO_COLOR);

        // drawBorder() : Draw a border around a rectangle
        //
        //   @x,@y : top left starting point
        //   @width, @height : dimensions
        //   @borderColour : Colour of the border
        //
        void drawBorder(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int borderColour);

        // _shitfToZone() : Change the origin and the coordinate system
        //          according to a selected drawing zone
        //
        //  @zoneID  : ID of the desintation zone
        //  @x, @y [i/o] : coordinates to change
        //  @width, @height : (new) dimensions in pixels of a single block in the choosen area
        //
        void shitfToZone(uint8_t zoneID, uint16_t& x, uint16_t& y, uint16_t& width, uint16_t& height);

        // Zones
        //
        ZONE* playfield(){
            return &playfield_;
        }
        ZONE* nextPiece(){
            return &nextPiece_;
        }

        // Strings manipulations
        static char* __valtoa(int num, const char* name, char* str, size_t rLength = 0);
        static void __strrev(char *str);
        static char* __strdrag(char *str, int rightChars);

        // Fonts
        //
#ifdef DEST_CASIO_CALC
        font_t* font(bool horz = true){
            return (horz?hFont_:vFont_);
        }
#endif // #ifdef DEST_CASIO_CALC

        //
        // Members
        //

        // Texts
        POINT       textsPos_[VAL_COUNT];      // Positions of texts

        // Keyboard
        char        keyLeft_, keyRight_, keyRotatePiece_, keyDown_, keyFall_;
        char        keyPause_, keyRotateDisplay_;
        char        keyQuit_;

        // Private methods
    private:

        // Draw a line of text vertically
        void _dtextV(int x, int y, int fg, const char* text);

        // Update members on rotation
        void _rotatedDisplay(bool doRotate, bool force);

    protected:
        // Screen & display parameters
        bool        rotatedDisplay_;    // Rotate all displays (default = False) ?

        ZONE        playfield_;
        ZONE        nextPiece_;

#ifdef DEST_CASIO_CALC
        font_t     *hFont_, *vFont_;    // Fonts
#endif //#ifdef DEST_CASIO_CALC
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_FXCG50_h__

// EOF
