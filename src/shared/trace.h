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

#ifdef DEST_CASIO_CALC
#include "scrCapture.h"   // capture only on TRACE mode

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

// TRACE : add a line a text in the screen
//
//  @val : text to display
//  @tCol : text colour
//  @bkCol : Background colour
//
#define TRACE(val, tCol, bkCol)      { if (NO_COLOR != bkCol) drect(TRACE_POS_X, TRACE_POS_Y, TRACE_POS_X + TRACE_WIDTH - 1, TRACE_POS_Y + TRACE_HEIGHT -1, bkCol);dtext(TRACE_POS_X, TRACE_POS_Y, tCol, val);dupdate();}
#else
#define TRACE(val, tCol, tbk)      {}
#endif // #ifdef TRACE_WIDTH

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_TRACE_h__

// EOF
