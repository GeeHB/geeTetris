//---------------------------------------------------------------------------
//--
//--	File	: trace.h
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

#ifndef __GEE_TETRIS_TRACE_h__
#define __GEE_TETRIS_TRACE_h__    1

#include "shared/casioCalcs.h"

#ifdef DEST_CASIO_CALC
#include "shared/scrCapture.h"   // capture only on TRACE mode

#define TRACE_MODE       1       // Only for tests
#else
#ifdef TRACE_MODE       // ???
#undef TRACE_MODE
#endif // TRACE_MODE
#endif // DEST_CASIO_CALCDEST_CASIO_CALC

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#ifdef TRACE_MODE
#define TRACE_POS_X     (CASIO_WIDTH - 140)
#define TRACE_POS_Y     (CASIO_HEIGHT - 60)
#define TRACE_WIDTH     100
#define TRACE_HEIGHT    15

#define TRACE(val, bk)      { if (C_NONE != bk) drect(TRACE_POS_X, TRACE_POS_Y, TRACE_POS_X + TRACE_WIDTH - 1, TRACE_POS_Y + TRACE_HEIGHT -1, bk);dtext(TRACE_POS_X, TRACE_POS_Y, C_BLACK, val);dupdate();}
#else
#define TRACE(val, bk)      {}
#endif // #ifdef TRACE_WIDTH

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_TRACE_h__

// EOF
