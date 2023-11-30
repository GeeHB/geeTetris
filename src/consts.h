//---------------------------------------------------------------------------
//--
//--	File	: consts.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Shared types, objects and constants
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TETRIS_CONSTS_h__
#define __GEE_TETRIS_CONSTS_h__    1

#include "shared/casioCalcs.h"  // Choose dest. format
#include "shared/trace.h"      // "test" mode

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
#define APP_VERSION  "0.5-1"
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

#define SCORE_STR   "Score : "
#define LEVEL_STR   "Level : "
#define COMPLETED_LINES_STR "Lines : "

#define MAX_VALUE_NAME_LEN 30

//
// Colour IDs
//
enum COLOUR_ID{
    COLOUR_ID_BOARD     = 0x00,
    // 1 -> 7 : Pieces colours (nCurses ID)
    COLOUR_ID_SHADOW    = 8,
    COLOUR_ID_TEXT      = 9,
    COLOUR_ID_BORDER    = 10,
    COLOUR_ID_BKGRND    = 11,
    //#define COLOUR_ID_ANIMATE   12     // animation When line if full
    COLOUR_ID_NONE = 12,
    LAST_COLOUR_ID		= COLOUR_ID_NONE
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

// Tab names
//
#define TAB_ABOUT   "Tetris"
#define TAB_LINES   "Lines"
#define TAB_LEVEL   "Level"
#define TAB_SHADOW  "Shadow"
#define TAB_PLAY    "Play"
#define TAB_QUIT    "Exit"

// Comments for tabs
#define TAB_LEVEL_STR   "Choose starting level"
#define TAB_DIRTY_LINES_STR   "Number of 'dirty' lines when starting the game"
#define TAB_SHADOW_OFF_STR  "No shadows"
#define TAB_SHADOW_ON_STR   "Shadows will be drawn"

//
// Min / max values for parameters
//

#define MIN_LEVEL   1
#define MAX_LEVEL   8

#define MIN_DIRTY_LINES   0
#define MAX_DIRTY_LINES   8

#ifndef FALSE
#define FALSE   0
#define TRUE    !FALSE
#endif // #ifndef FALSE

//---------------------------------------------------------------------------
//--
//-- tetrisParameters object
//--
//--    All the game's parameters
//--    could be a simple interface / struct
//--
//---------------------------------------------------------------------------

class tetrisParameters {
    public:
        // Construction
        //
        tetrisParameters() {
            // Set default parameters
            startLevel_ = MIN_LEVEL;
            dirtyLines_ = MIN_DIRTY_LINES;
            shadow_ = TRUE;
            rotatedDisplay_ = FALSE;        // by default, no screen rotation
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
        int shadow_;
        int rotatedDisplay_;
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
