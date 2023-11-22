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

#define CASIO_VERT_TEXT_OFFSET  15

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

        //
        // Displays orientations
        //

        // Update members on rotation
        void rotatedDisplay(bool doRotate);

        // (trigonometric) rotations
        //  ... of a single point
        void rotate(int16_t& x, int16_t& y){
            int16_t ny(CASIO_HEIGHT - x);
            x = y;
            y = ny;
        }

        // ... of a rect
        void rotate(int16_t& xFrom, int16_t& yFrom, int16_t& xTo, int16_t& yTo){
            rotate(xFrom, yFrom);
            rotate(xTo, yTo);

            // The rect (xFrom, yFrom) -> (xTo, yTo)
            // turns and becomes (xTo, yFrom) -> (xFrom, yTo)
            int16_t oFrom(xFrom);
            xFrom = xTo;
            xTo = oFrom;
        }

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
        //   @borderColour : Colour of the border in RGB format or -1 (if no border)
        //   @fillColour : Filling colour in RGB format or -1 (if empty)
        //
        void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int32_t fillColour = NO_COLOR, int32_t borderColour = NO_COLOR);;

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
