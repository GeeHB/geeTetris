//----------------------------------------------------------------------
//--
//--    menus.h
//--
//--        Definition of constants for menu bars
//--
//----------------------------------------------------------------------

#ifndef __GEETETRIS_SUB_MENU_h__
#define __GEETETRIS_SUB_MENU_h__    1

#include "shared/menuBar.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Main menu-bar
//
#define IDM_START               1
#ifdef FX9860G
#define IDS_START               "Go"
#else
#define IDS_START               "Start"
#endif // #ifdef FX9860G

#define IDM_PARAMS              2
#define IDS_PARAMS              "Params."

#define IDM_ABOUT               3
#ifdef FX9860G
#define IDS_ABOUT               "?"
#else
#define IDS_ABOUT               "About"
#endif // #ifdef FX9860G

#define IDM_QUIT                4
#define IDS_QUIT                "Quit"

// Parameters sub-menu
//
#define IDM_PARAMS_SHADOW       21
#define IDS_PARAMS_SHADOW       "Shadow"

#define IDM_PARAMS_LINES        22
#define IDS_PARAMS_LINES        "Lines"
#define COMMENT_PARAMS_LINES    "'Dirty' lines"

#define IDM_PARAMS_LEVEL        23
#define IDS_PARAMS_LEVEL        "Level"
#define COMMENT_PARAMS_LEVEL    "Starting level"

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEETETRIS_SUB_MENU_h__

// EOF
