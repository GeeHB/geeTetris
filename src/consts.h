//----------------------------------------------------------------------
//--
//--    consts.h
//--
//--
//--            Shared types, objects and constants
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_CONSTS_h__
#define __GEE_TETRIS_CONSTS_h__    1

#include "shared/casioCalcs.h"  // Choose dest. format

#ifdef DEST_CASIO_CALC
#include <gint/keyboard.h>
#else
#include <malloc.h>
#include <cstdint> // <stdint.h>
#include <cstdio>
#endif // #ifdef DEST_CASIO_CALC

#include <cstring>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// App. infos
#define APP_NAME     "geeTetris"
#define APP_VERSION  "0.7.1"
#define APP_AUTHOR   "GeeHB"

// Playfield's dimensions (in box units)
//
#define PLAYFIELD_WIDTH     10
#define PLAYFIELD_HEIGHT    20

// Scores valorisation (in %)
//
#define SCORE_NO_SHADOW         10.0    // +10% if no pieces'shadowing
#define SCORE_SPEED_GAME        2.0     // 2% * {piece's height} => quick play <=> more points
#define SCORE_DIRTY_LINES       0.1     // 0.1% per starting dirty-line
#define SCORE_LEVEL_VALUATION   15.0    // 15% more for each level

// Indicators
//
enum VALUE_ID{
    SCORE_ID = 0,
    LEVEL_ID = 1,
    COMPLETED_LINES_ID = 2,
    VAL_COUNT = COMPLETED_LINES_ID + 1
};

#ifdef FX9860G
#define SCORE_STR   "Sc: "
#define LEVEL_STR   "Le: "
#define COMPLETED_LINES_STR "Li: "
#define MAX_VALUE_NAME_LEN 13
#else
#define SCORE_STR   "Score : "
#define LEVEL_STR   "Level : "
#define COMPLETED_LINES_STR "Lines : "
#define MAX_VALUE_NAME_LEN 30
#endif // #ifdef FX9860G

//
// Colour IDs
//
enum COLOUR_ID{
    COLOUR_ID_BOARD     = 0,
    // 1 -> 7 : Pieces colours (nCurses ID)
    COLOUR_ID_SHADOW    = 8,
    COLOUR_ID_TEXT      = 9,
    COLOUR_ID_BORDER    = 10,
    COLOUR_ID_BKGRND    = 11,
    //#define COLOUR_ID_ANIMATE   12     // animation When line if full
    COLOUR_ID_NONE = 12,
    LAST_COLOUR_ID      = COLOUR_ID_BKGRND
};

// Key codes
//
#ifdef DEST_CASIO_CALC
enum GAME_KEY{
    KEY_CODE_F1 = KEY_F1,     // !!!
    KEY_CODE_F6 = KEY_F6,
    KEY_CODE_UP = KEY_UP,
    KEY_CODE_DOWN = KEY_DOWN,
    KEY_CODE_LEFT = KEY_LEFT,
    KEY_CODE_RIGHT = KEY_RIGHT,
    KEY_CODE_FALL = KEY_0,
    KEY_CODE_PAUSE = KEY_OPTN,
    KEY_CODE_CAPTURE = KEY_7,
    KEY_CODE_ROTATE_DISPLAY = KEY_XOT,
    KEY_CODE_SHOW_SCORES = KEY_MUL,
    KEY_CODE_EXIT = KEY_EXIT,
    KEY_CODE_ENTER = KEY_EXE
};
#else
enum GAME_KEY{
    KEY_CODE_F1 = 'A',
    KEY_CODE_F6 = 'F',
    KEY_CODE_LEFT = '4',
    KEY_CODE_RIGHT = '5',
    KEY_CODE_UP = '3',
    KEY_CODE_DOWN = '2',
    KEY_CODE_FALL = ' ',
    KEY_CODE_PAUSE = 'p',
    KEY_CODE_ROTATE_DISPLAY = 'r',
    KEY_CODE_SHOW_SCORES = 's',
    KEY_CODE_EXIT = 'q',
    KEY_CODE_ENTER = '\13'
};
#endif // #ifdef DEST_CASIO_CALC

//
// Min / max values for parameters
//

#define MIN_LEVEL   1
#define MAX_LEVEL   8

#define MIN_DIRTY_LINES   0
#define MAX_DIRTY_LINES   8

#define DEF_SHADOW      1  // 1 == true

// Ensure value is in the range
#define IN_RANGE(val, min, max) ((val < min)?min:((val > max)?max:val))

//
// Value of parameters
//
#define VAL_RANGE_BOX_WIDTH     15

#define VAL_RANGE_COMMENT_X     10
#define VAL_COMMENT_Y_OFFSET    15

#define VAL_COL_BORDER          COLOUR_BLACK
#define VAL_COL_TXT_SEL         COLOUR_WHITE
#define VAL_COL_BK_SEL          COLOUR_LT_BLUE

#define VAL_COL_TXT_UNSEL       COLOUR_BLACK
#define VAL_COL_BK_UNSEL        COLOUR_WHITE

// Blinking
//
#define BLINK_TICK_DURATION     100 //  in ms
#define BLINK_TICKCOUNT         5   // "duration" of blinking in ticks

// "pause" image
//
#define IMG_PAUSE_W         CASIO_WIDTH
#define IMG_PAUSE_H         70
#define IMG_PAUSE_COPY_Y    46
#define IMG_PAUSE_LINES     (CASIO_HEIGHT - IMG_PAUSE_H)

//----------------------------------------------------------------------
//--
//-- tetrisParameters object
//--
//--    All the game's parameters
//--    could be a simple interface / struct
//--
//----------------------------------------------------------------------

class tetrisParameters {
    public:
        // Construction
        //
        tetrisParameters() {
            // Set default parameters
            startLevel_ = MIN_LEVEL;
            dirtyLines_ = MIN_DIRTY_LINES;
            shadow_ = (DEF_SHADOW == 1);
            rotatedDisplay_ = false; // by default, no screen rotation
        }

        // recopy
        void copy(tetrisParameters* other) {
            if (other){
                startLevel_ = other->startLevel_;
                dirtyLines_ = other->dirtyLines_;
                shadow_ = other->shadow_;
                rotatedDisplay_ = other->rotatedDisplay_;
            }
        }

    // Members
    //
    public:
        int startLevel_;
        int dirtyLines_;
        bool shadow_;
        bool rotatedDisplay_;
};

// A single unsigned value (and its previous val if exists)
//
#define MAX_VALUE_NAME  20
typedef struct __uvalue{

    // Construction
    __uvalue(){
        name[0] = '\0';
        value = 0;
        previous = -1; //  value is not (yet) used
    }

    char       name[MAX_VALUE_NAME+1];
    uint32_t   value;
    int32_t    previous;
} UVALUE;

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CONSTS_h__

// EOF
