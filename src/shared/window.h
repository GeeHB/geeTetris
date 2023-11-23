//---------------------------------------------------------------------------
//--
//--	File	: window.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of window object - Show a "window" on
//--            top of the screen
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TOOLS_WINDOW_h__
#define __GEE_TOOLS_WINDOW_h__    1

#include "casioCalcs.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#ifdef DEST_CASIO_CALC
#include <gint/display.h>
#endif // #ifdef DEST_CASIO_CALC

#define WIN_BORDER_WIDTH        2

// "style" of a window
#define WIN_STYLE_SBORDER       1   // Simple border
#define WIN_STYLE_DBORDER       2   // double border
#define WIN_STYLE_VCENTER       4   // Center window vertically
#define WIN_STYLE_HCENTER       8   // Center horizontally

#define WIN_STYLE_DEFAUlT       (WIN_STYLE_DBORDER | WIN_STYLE_VCENTER | WIN_STYLE_HCENTER)

//  window object - A basic window on top of the screen
//

class window{
public:

    typedef struct _winInfo{

        // Construction
        _winInfo(){
            clear();
        }

        // clear
        void clear();

        // Copy
        void copy(_winInfo& src);

        char*   title;
        int     style;
        RECT    position;
        int     bkColour;
        int     borderColour;
        int     textColour;
    }winInfo;

    // Construction
    window();

    // Destruction
    virtual ~window(){
        close();
    }

    // Creation
    bool create(winInfo& info);

    // Close the window
    void close();

    // Update
    void update();

    //
    // Helpers
    //

    // Draw a line of text (in window coordinates)
    void drawText(const char* text, int x, int y, int tCol = -1, int bCol = -1){
        if (text && text[0]){
            dtext_opt(x + client_.x, y + client_.y, (tCol==-1)?infos_.textColour:tCol, (bCol==-1)?infos_.bkColour:bCol, DTEXT_LEFT, DTEXT_TOP, text);
        }
    }

    // Convert window (x, y) into screen (x,y)
    void win2Screen(POINT& coord){
        coord.x+=client_.x;
        coord.y+=client_.y;
    }

private:
#ifdef DEST_CASIO_CALC
    // Convert a rect. struct to a window struct
    void _rect2Window(RECT& rect, struct dwindow& win){
        win = {rect.x, rect.y, rect.x + rect.w, rect.y+rect.h};
    }

    void _drawBorder(struct dwindow& dest);
#endif // #ifdef DEST_CASIO_CALC

protected:
    // Members
    //
    bool        activated_; // Is the window in place ?
    winInfo     infos_;     // Informations concerning this window
    RECT        client_;    // Client area in screen coordinates
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TOOLS_WINDOW_h__

// EOF
