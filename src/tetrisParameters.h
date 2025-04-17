//----------------------------------------------------------------------
//--
//--    tetrisParameters.h
//--
//--        Game's parameters
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_PARAMETERS_h__
#define __GEE_TETRIS_PARAMETERS_h__    1

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
    BOOL rotatedDisplay;
} tetrisParameters, PARAMS, * PPARAMS;

BOOL params_init(PPARAMS const params);
BOOL params_copy(PPARAMS const params, PPARAMS const other);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_PARAMETERS_h__

// EOF
