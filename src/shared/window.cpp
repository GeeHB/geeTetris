//---------------------------------------------------------------------------
//--
//--	File	: window.cpp
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

#include "window.h"

#include <cstring>
#include <cstdlib>

// clear
//
void window::winInfo::clear(){
    if (title){
        free(title);
        title = nullptr;
    }
    style = WIN_STYLE_DEFAUlT;
    position = {0, 0, CASIO_WIDTH, CASIO_HEIGHT};
    bkColour = COLOUR_WHITE;
    borderColour = textColour = COLOUR_BLACK;
}

// Copy
//
void window::winInfo::copy(_winInfo& src){
    clear();

    if (src.title){
        title = strdup(title);
    }
    style = src.style;
    position = src.position;
    bkColour = src.bkColour;
    borderColour = src.borderColour;
    textColour = src.textColour;
}

// Construction
//
window::window(){
    activated_ = false;
    infos_.clear();         // useless ?
}

// Creation
//
bool window::create(winInfo& info){
    close(); // if previously opened

    infos_.copy(info);  // get struct. infos.

    // Position
    if (infos_.style & WIN_STYLE_VCENTER){
        if (!infos_.position.h || infos_.position.h > CASIO_HEIGHT){
            return false;
        }

        infos_.position.y = (CASIO_HEIGHT - infos_.position.h);
    }

    if (infos_.style & WIN_STYLE_HCENTER){
        if (!infos_.position.w || infos_.position.w > CASIO_WIDTH){
            return false;
        }

        infos_.position.x = (CASIO_WIDTH - infos_.position.w);
    }

#ifdef DEST_CASIO_CALC
    struct dwindow dest;
    _rect2Window(infos_.position, dest);
    dwindow_set(dest);
    dest.right--;   // included
    dest.bottom--;

    dclear(infos_.bkColour); // background

    // Draw border(s)
    if (infos_.style & WIN_STYLE_SBORDER || infos_.style & WIN_STYLE_DBORDER){
        _drawBorder(dest);

        if (infos_.style & WIN_STYLE_DBORDER){
            dest.left+=WIN_BORDER_WIDTH;
            dest.top+=WIN_BORDER_WIDTH;
            dest.right-=WIN_BORDER_WIDTH;
            dest.bottom-=WIN_BORDER_WIDTH;

            _drawBorder(dest);

            
        }
    }

    // Internal dims.
    client_.x = dest.left + WIN_BORDER_WIDTH;
    client_.y = dest.top + WIN_BORDER_WIDTH;
    client_.w = dest.right - dest.left - 2 * WIN_BORDER_WIDTH + 1;
    client_.h = dest.bottom - dest.top - 2 * WIN_BORDER_WIDTH + 1;

    // Draw title
    if (infos_.title && infos_.title[0]){       // At least a char
        int width(0), height(0);
        int len(strlen(infos_.title)), nLen(len);
        int maxWidth(client_.w - 2 * WIN_BORDER_WIDTH);
        dnsize(infos_.title, len, NULL, &width, &height);
        
        // Adjust title length ?
        if (width > maxWidth){
            nLen = (int)((len * maxWidth) / width);
        }
        
        // Center the title (or part of the title that fit window size)
        dtext_opt(client_.x + (client_.w - width)/2 + WIN_BORDER_WIDTH, WIN_BORDER_WIDTH, infos_.textColour, infos_.bkColour, DTEXT_LEFT, DTEXT_TOP, infos_.title, nLen);

        // New client dims with a title
        client_.y += height;
        client_.h -= height;
    }
#endif // #ifdef DEST_CASIO_CALC

    update();

    // done
    return true;
}

// Close the window
//
void window::close(){
    if (activated_){
        // return to a whole screened window
#ifdef DEST_CASIO_CALC
        struct dwindow screen = {0, 0, CASIO_WIDTH, CASIO_HEIGHT};
        dwindow_set(screen);
#endif // #ifdef DEST_CASIO_CALC

        // end
        infos_.clear();
        activated_ = false;
    }
}

// Update
//
void window::update(){
#ifdef DEST_CASIO_CALC
    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

#ifdef DEST_CASIO_CALC

// Draw a single border
//
void window::_drawBorder(struct dwindow& dest){
    dline(dest.left, dest.top, dest.right, dest.top, infos_.borderColour);
    dline(dest.left, dest.bottom, dest.right, dest.bottom, infos_.borderColour);
    dline(dest.left, dest.top, dest.left, dest.bottom, infos_.borderColour);
    dline(dest.right, dest.top, dest.right, dest.bottom, infos_.borderColour);
}
#endif // #ifdef DEST_CASIO_CALC

// EOF
