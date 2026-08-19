//----------------------------------------------------------------------
//--
//--    window.c
//--
//--           Show a "window" on top of the screen
//--
//----------------------------------------------------------------------

#include "window.h"

#include <string.h>

//
//  window - A basic window on top of the screen
//

// window_init() : Initialize the window
//
//  @win : Pointer to a window struct.
//  @title : window's title (can be NULL)
//
//  @return : TRUE if successfully initalized
//
BOOL window_init(PWINDOW const win, const char* title){
    if (win){
        win->activated = FALSE;
        win->title = title?strdup(title):NULL;
        win->style = WIN_STYLE_DEFAULT;
        win->pos = (RECT){.x = 0, .y = 0, .w = CASIO_WIDTH, .h = CASIO_HEIGHT};
        win->client = (RECT){.x = 0, .y = 0, .w = 0, .h = 0};
        win->bkColour = COLOUR_WHITE;
        win->borderColour = win->textColour = COLOUR_BLACK;
        return TRUE;
    }

    return FALSE;
}

// window_clear() : clear struct. content
//
//  @win : Pointer to a window struct.
//
void window_clear(PWINDOW const win){
    if (win && win->title){
        free(win->title);
        win->title = NULL;
    }
}

// window_create() : Creation of a window
//
//  @win : Pointer to a window struct.
//
//  @return : TRUE if created else false
//
BOOL window_create(PWINDOW win){
    if (win){
        window_close(win);

        // Position
        if (win->style & WIN_STYLE_VCENTER){
            if (!win->pos.h || win->pos.h > CASIO_HEIGHT){
                return FALSE;
            }

            win->pos.y = (CASIO_HEIGHT - win->pos.h) / 2;
        }

        if (win->style & WIN_STYLE_HCENTER){
            if (!win->pos.w || win->pos.w > CASIO_WIDTH){
                return FALSE;
            }

            win->pos.x = (CASIO_WIDTH - win->pos.w) / 2;
        }

#ifdef DEST_CASIO_CALC
        struct dwindow dest;
        window_RECT2Window(&win->pos, &dest);
        dwindow_set(dest);

        dest.right--;   // included
        dest.bottom--;

        dclear((color_t)win->bkColour); // background

        // Draw border(s)
        if (win->style & WIN_STYLE_SBORDER || win->style & WIN_STYLE_DBORDER){
            window_drawBorder(win, &win->pos);

            if (win->style & WIN_STYLE_DBORDER){
                RECT dBorder;
                copyRect(&dBorder, &win->pos);
                deflateRect(&dBorder, WIN_BORDER_WIDTH, WIN_BORDER_WIDTH);
                window_drawBorder(win, &dBorder);
            }
        }

        // Internal dims.
        win->client.x = dest.left + WIN_BORDER_WIDTH;
        win->client.y = dest.top + WIN_BORDER_WIDTH;
        win->client.w = dest.right - dest.left - 2 * WIN_BORDER_WIDTH + 1;
        win->client.h = dest.bottom - dest.top - 2 * WIN_BORDER_WIDTH + 1;

        // Draw title
        if (win->title && win->title[0]){       // At least a char
            int width, height;
            int len = strlen(win->title);
            int nLen = len;
            int maxWidth = win->client.w - 2 * WIN_BORDER_WIDTH;
            dnsize(win->title, len, NULL, &width, &height);

            // Adjust title length ?
            if (width > maxWidth){
                nLen = (int)(len * maxWidth / width);
            }

            // Center the title (or part of the title that fit window size)
            dtext_opt(win->client.x + (win->client.w - width)/2,
                        win->client.y + WIN_BORDER_WIDTH, win->textColour,
                        win->bkColour, DTEXT_LEFT, DTEXT_TOP,
                        win->title, nLen);

            // New client dims with a title
            win->client.y += height;
            win->client.h -= height;
        }
#endif // #ifdef DEST_CASIO_CALC

        // done
        win->activated = TRUE;
        return TRUE;
    }

    return FALSE;
}

// window_close() : Close the current window
//
//  @win : Pointer to tha window struct.
//
void window_close(PWINDOW const win){
    if (win && win->activated){
        // return to a whole screened window
#ifdef DEST_CASIO_CALC
        struct dwindow screen = (struct dwindow){.left = 0, .top = 0, CASIO_WIDTH, CASIO_HEIGHT};
        dwindow_set(screen);
#endif // #ifdef DEST_CASIO_CALC

        // end
        window_clear(win);
        win->activated = FALSE;
    }
}

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
void window_drawText(PWINDOW win, const char* text, int x, int y, int tCol, int bCol){
    if (win && win->activated && text && text[0]){
#ifdef DEST_CASIO_CALC
        POINT dest;
        int w, h;
        if (x < 0 || y < 0){
            dsize(text, NULL, &w, &h);  // Need text dims to center
        }

        // Center text (or not ...)
        dest.x = win->client.x + ((x<0)?(((int)win->client.w - w)/2):x);
        dest.y = win->client.y + ((y<0)?(((int)win->client.h - h)/2):y);

        dtext_opt(dest.x, dest.y,
            (tCol==-1)?win->textColour:tCol,
            (bCol==-1)?win->bkColour:bCol,
            DTEXT_LEFT, DTEXT_TOP,
            text);
#endif // #ifdef DEST_CASIO_CALC
    } // if (win && activated && ...)
}

// window_drawBorder() : Draw a single border
//
//  @win : Pointer to a window struct.
//  @rect : Border rect
//
void window_drawBorder(PWINDOW const win, PRECT rect){
#ifdef DEST_CASIO_CALC
    dline(rect->x, rect->y, rect->x + rect->w - 1, rect->y, win->borderColour);
    dline(rect->x, rect->y + rect->h - 1, rect->x + rect->w - 1, rect->y + rect->h - 1, win->borderColour);
    dline(rect->x, rect->y, rect->x, rect->y + rect->h - 1, win->borderColour);
    dline(rect->x + rect->w - 1, rect->y, rect->x + rect->w - 1, rect->y + rect->h - 1, win->borderColour);
#endif // #ifdef DEST_CASIO_CALC
}

// window_2Screen() : Convert window (x, y) into screen (x,y)
//
//  @win : Pointer to a window struct.
//  @coord : Coordinates to convert
//
void window_win2Screen(PWINDOW win, PPOINT coord){
    if (win && coord){
        coord->x+=win->client.x;
        coord->y+=win->client.y;
    }
}

#ifdef DEST_CASIO_CALC
// window_RECT2Window() : Convert a rect. struct to a window struct
//
//  @rect : pointer to a RECT strict.
//  @rwin : pointer to a dwindow struct.
//
void window_RECT2Window(PRECT rect, struct dwindow* rwin){
    if (rect && rwin){
        rwin->left = rect->x;
        rwin->top = rect->y;
        rwin->right = rect->x + rect->w;
        rwin->bottom = rect->y +rect->h;
    }
}
#endif // #ifdef DEST_CASIO_CALC

// EOF
