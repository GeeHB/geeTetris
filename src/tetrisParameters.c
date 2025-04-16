//----------------------------------------------------------------------
//--
//--    tetrisParameters.c
//--
//--        Game's parameters
//--
//----------------------------------------------------------------------

#include"tetrisParameters.h"


BOOL params_init(PPARAMS const params){
    if (params){
        // Set default parameters
        params->startLevel = MIN_LEVEL;
        params->dirtyLines = MIN_DIRTY_LINES;
        params->shadow = (DEF_SHADOW == 1);
        params->rotatedDisplay = FALSE; // by default, no screen rotation
        return TRUE;
    }

    return FALSE;
}

BOOL params_copy(PPARAMS const params, PPARAMS const other){
    if (params && other){
        params->startLevel = other->startLevel;
        params->dirtyLines = other->dirtyLines;
        params->shadow = other->shadow;
        params->rotatedDisplay = other->rotatedDisplay;

        return TRUE;
    }

    return FALSE;
}



// EOF
