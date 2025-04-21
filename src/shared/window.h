//----------------------------------------------------------------------
//--
//--    window.h
//--
//--            Definition of window object - Show a "window" on
//--            top of the screen
//--
//----------------------------------------------------------------------

#ifndef __GEE_WINDOW_h__
#define __GEE_WINDOW_h__    1

#include "casioCalcs.h"

#ifdef DEST_CASIO_CALC
#include <gint/display.h>
#endif // #ifdef DEST_CASIO_CALC

#define _GEEHB_WINDOW_VER_      "0.2.1"

#define WIN_BORDER_WIDTH        2

// "style" of a window
#define WIN_STYLE_SBORDER       1   // Simple border
#define WIN_STYLE_DBORDER       2   // double border
#define WIN_STYLE_VCENTER       4   // Center window vertically
#define WIN_STYLE_HCENTER       8   // Center horizontally
#define WIN_STYLE_CENTER        (WIN_STYLE_VCENTER | WIN_STYLE_HCENTER)

#define WIN_STYLE_DEFAULT       (WIN_STYLE_DBORDER | WIN_STYLE_CENTER)

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//
//  window - A basic window on top of the screen
//

typedef struct _window{
    uint8_t style;
    char*   title;
    BOOL    activated;
    RECT    pos;
    RECT    client;
    int     bkColour;
    int     borderColour;
    int     textColour;
}window, WINDOW, * PWINDOW;


// window_init() : Close the current window
//
//  @win : Pointer to a window struct.
//  @title : window's title (can be NULL)
//
//  @return : TRUE if successfully initalized
//
BOOL window_init(PWINDOW const win, const char* title);

// window_clear() : clear struct. content
//
//  @win : Pointer to a window struct.
//
void window_clear(PWINDOW const win);

// window_close() : Close the current window
//
//  @win : Pointer to tha window struct.
//
void window_close(PWINDOW const win);

// drawText() : Draw a line of text (in window coordinates)
//
//  @win : Pointer to a window struct.
//  @text : pointer to the text to draw
//  @x, @y : coordinates of text relative to top left corner of the window
//          if < 0, the text will be centered
//          (horizontally for x  <0, vertically if y < 0)
//  @tCol : text colour. If is equal to -1, the default text colour
//          will be used
//  @bCol : background colour. If is equal to -1,
//          the default ground colour will be used
//
void window_drawText(PWINDOW win, const char* text, int x, int y, int tCol, int bCol);

// window_drawBorder() : Draw a single border
//
//  @win : Pointer to a window struct.
//  @rect : Border rect
//
void window_drawBorder(PWINDOW const win, PRECT rect);

#ifdef DEST_CASIO_CALC
// window_RECT2Window() : Convert a rect. struct to a window struct
//
//  @rect : pointer to a RECT strict.
//  @rwin : pointer to a dwindow struct.
//
void window_RECT2Window(PRECT rect, struct dwindow* rwin);
#endif // #ifdef DEST_CASIO_CALC

// window_2Screen() : Convert window (x, y) into screen (x,y)
//
//  @win : Pointer to a window struct.
//  @coord : Coordinates to convert
//
void window_win2Screen(PWINDOW win, PPOINT coord);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_WINDOW_h__

// EOF
