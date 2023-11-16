//---------------------------------------------------------------------------
//--
//--	File	: casioCalcs.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--            Define the binary's dest. formet
//--
//---------------------------------------------------------------------------

#ifndef __GEE_CASIO_CALCS_h__
#define __GEE_CASIO_CALCS_h__    1

#ifndef LINUX_VER
#define DEST_CASIO_CALC        1   // Compile for a Casio calculator
#endif // #ifndef LINX_VER

#ifdef DEST_CASIO_CALC
// Specific includes for calculators
#include <gint/gint.h>
#include <gint/display.h>

// Screen dimensions in pixels
#define CASIO_WIDTH     DWIDTH
#define CASIO_HEIGHT    DHEIGHT
#else
// Screen dimensions in pixels
#define CASIO_WIDTH     384
#define CASIO_HEIGHT    192
#endif // #ifdef DEST_CASIO_CALC

#include <cstdint>

#endif // #ifndef __GEE_CASIO_CALCS_h__

// EOF
