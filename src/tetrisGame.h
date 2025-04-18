//----------------------------------------------------------------------
//--
//--    tetrisGame.h
//--
//--        Definition of tetrisGame
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_GAME_h__
#define __GEE_TETRIS_GAME_h__   1

#include "tetrisParameters.h"
#include "piece.h"
#include "templates.h"
#include "playArea.h"
#include "sList.h"
#include "shared/keys.h"

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#include <gint/clock.h>
#endif // #ifdef DEST_CASIO_CALC

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cpluscplus

// Timer & game speed
//
#define GAME_TICK_DURATION      25      //  in ms
#define GAME_START_TICKS        48      // Initial piece speed in ticks ( 1200 ms / GAME_TICK_DURATION)

#define GAME_MAX_ACC_LEVEL      15      // No more acceleration when this level is reached
#define GAME_ACC_RATE           175     // Game acceleration in /000

#define GAME_NEXT_LEVEL_MOVES   250     // Level++ after # movements


// Game status
//
enum GAME_STATUS{
    STATUS_READY     = 1,
    STATUS_RUNNING   = 2,
    STATUS_STOPPED   = 8,
    STATUS_CANCELED  = 16
};

//
// tetrisGame
//
//  Handle the gameplay and the game(without display !)
//
typedef struct __tetrisGame{
    uint8_t status;
    uint8_t playField[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];
    PIECE tetraminos[TETRAMINOS_COUNT];                      // The tetraminos' list
    int32_t colours[LAST_COLOUR_ID+1];     // Colours in rgb
    PARAMS parameters;
    PLAYAREA casioDisplay;
    int8_t nextIndex;  // -1 = None
    PIECESTATUS nextPos, currentPos;
    UVALUE  values[VAL_COUNT];
} TETRISGAME, * PTETRISGAME;

// tetrisGame_init() : Initialize a new game
//
//  @tetris : pointer to a tetrisGame struct.
//  params : pointer to the game's parameters
//
//  @return : TRUE if init done
//
BOOL tetrisGame_init(PTETRISGAME const tetris, PPARAMS const params);

// tetrisGame_setParameters() : Set game's parameters
//
//  @tetris : pointer to a tetrisGame struct.
//  @params : Struct. containining parameters for the game
//    These parameters are choosen by the user
//
void tetrisGame_setParameters(PTETRISGAME const tetris, PPARAMS const params);

// start() : Start the tetris game
//
//  The entire game is handled by this method.
//  It retuns on error or when the game is over
//
//  @tetris : pointer to the tetris struct.
//
//  @return :  FALSE on error(s) or if canceled by user
//
BOOL tetrisGame_start(PTETRISGAME const tetris);

// pause() : Pause or resume the game
//
//  @tetris : Pointer to atretrisGame struct.
//
#ifdef DEST_CASIO_CALC
void tetrisGame_pause(PTETRISGAME const tetris);
#endif // #ifdef DEST_CASIO_CALC

// end() - Force the end of the game
//
#define tetrisGame_end(tetris) tetris->status |= STATUS_STOPPED;

#ifdef __cplusplus
}
#endif // #ifdef __cpluscplus

#endif // __GEE_TETRIS_GAME_h__

// EOF
