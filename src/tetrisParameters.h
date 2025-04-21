//----------------------------------------------------------------------
//--
//--    tetrisParameters.h
//--
//--        Game's parameters
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_PARAMETERS_h__
#define __GEE_TETRIS_PARAMETERS_h__    1

#ifdef DEST_CASIO_CALC
#include <gint/keyboard.h>
#endif // #ifdef DEST_CASIO_CALC

#include "consts.h"
#include "shared/casioCalcs.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//
// Min / max values for parameters
//

#define MIN_LEVEL   1
#define MAX_LEVEL   8

#define MIN_DIRTY_LINES   0
#define MAX_DIRTY_LINES   8

#define DEF_SHADOW      1  // 1 == TRUE

// Tetris parameters
//
typedef struct __tetrisParameters{
    int startLevel;
    int dirtyLines;
    BOOL shadow;
    CALC_ORIENTATION orientation;
} tetrisParameters, PARAMS, * PPARAMS;

BOOL params_init(PPARAMS const params);
BOOL params_copy(PPARAMS const params, PPARAMS const other);
int8_t params_changeNumValue(uint8_t value,
                    uint8_t min, uint8_t max, const char* comment);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_PARAMETERS_h__

// EOF
