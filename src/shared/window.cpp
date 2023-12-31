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

#include "trace.h"

#include <cstring>
#include <cstdlib>

//---------------------------------------------------------------------------
//--
//-- window::_winInfo structure
//--
//--    Informations concerning a window object
//--
//---------------------------------------------------------------------------

// Construction
//
window::_winInfo::_winInfo(){
    title = nullptr;
    style = WIN_STYLE_DEFAUlT;
    position = {0, 0, CASIO_WIDTH, CASIO_HEIGHT};
    bkColour = COLOUR_WHITE;
    borderColour = textColour = COLOUR_BLACK;
}

// clear() : clear struct. content
//
void window::winInfo::clear(){
    if (title){
        free(title);
        title = nullptr;
    }
}

// copy() : recopy a strcut
//
//  All datas will be copied into the current struct.
//  title member will be reallocated
//
//  @src : _winInfo struct source to recopy
//
void window::winInfo::copy(_winInfo& src){
    clear();

    if (src.title){
        title = strdup(src.title);
    }
    style = src.style;
    position = src.position;
    bkColour = (src.bkColour < 0?COLOUR_WHITE:src.bkColour);
    borderColour = (src.borderColour < 0?COLOUR_BLACK:src.borderColour);
    textColour = (src.textColour < 0?COLOUR_BLACK:src.textColour);
}

//---------------------------------------------------------------------------
//--
//-- window object
//--
//--    "window" displayed on top of the screen
//--
//---------------------------------------------------------------------------

// Construction
//
window::window(){
    // Initialize members
    activated_ = false;
    infos_.clear();         // useless ?
    client_ = {0, 0, 0, 0};
}

// create() : Creation of a window
//
//  @info : Information concerning the new window
//
//  @return : true if created else false
//
bool window::create(winInfo& info){
    close(); // if previously opened

    infos_.copy(info);  // get struct. infos.

    // Position
    if (infos_.style & WIN_STYLE_VCENTER){
        if (!infos_.position.h || infos_.position.h > CASIO_HEIGHT){
            return false;
        }

        infos_.position.y = (CASIO_HEIGHT - infos_.position.h) / 2;
    }

    if (infos_.style & WIN_STYLE_HCENTER){
        if (!infos_.position.w || infos_.position.w > CASIO_WIDTH){
            return false;
        }

        infos_.position.x = (CASIO_WIDTH - infos_.position.w) / 2;
    }

#ifdef DEST_CASIO_CALC
    struct dwindow dest;
    _rect2Window(infos_.position, dest);

#ifndef TRACE_MODE
    dwindow_set(dest);
#endif // #ifndef TRACE_MODE

    dest.right--;   // included
    dest.bottom--;

    dclear((color_t)infos_.bkColour); // background

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
        dtext_opt(client_.x + (client_.w - width)/2 + WIN_BORDER_WIDTH, client_.y + WIN_BORDER_WIDTH, infos_.textColour, infos_.bkColour, DTEXT_LEFT, DTEXT_TOP, infos_.title, nLen);

        // New client dims with a title
        client_.y += height;
        client_.h -= height;
    }
#else
    std::cout << "--------------------------------" << std::endl;
    std::cout << "\t\t" << infos_.title << std::endl << std::endl;
#endif // #ifdef DEST_CASIO_CALC

    update();

    // done
    activated_ = true;
    return true;
}

// close() : Close the current window
//
void window::close(){
    if (activated_){
        // return to a whole screened window
#ifdef DEST_CASIO_CALC
#ifndef TRACE_MODE
        struct dwindow screen = {0, 0, CASIO_WIDTH, CASIO_HEIGHT};
        dwindow_set(screen);
#endif // #ifndef TRACE_MODE
#endif // #ifdef DEST_CASIO_CALC

        // end
        infos_.clear();
        activated_ = false;
    }
}

// update() : Update the screen
//
void window::update(){
#ifdef DEST_CASIO_CALC
    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}


// drawText() : Draw a line of text (in window coordinates)
//
//  @text : pointer to the text to draw
//  @x, @y : coordinates of text relative to top left corner of the window
//          if < 0, the text will be centered (horizontally for x  <0, vertically if y < 0)
//  @tCol : text colour. If is equal to -1, the default text colour will be used
//  @bCol : background colour. If is equal to -1, the default ground colour will be used
//
void window::drawText(const char* text, int x, int y, int tCol, int bCol){
    if (activated_){
        /*
        POINT dest;
        if (x < 0 ){

        }
        else{
            dest.x = x;
        }

        if (y < 0){
        }
        else{
            dest.y = y;
        }
        */
        POINT dest(x, y);
        win2Screen(dest);   // Change origin
        if (text && text[0]){
#ifdef DEST_CASIO_CALC
            dtext_opt(dest.x, dest.y, (tCol==-1)?infos_.textColour:tCol, (bCol==-1)?infos_.bkColour:bCol, DTEXT_LEFT, DTEXT_TOP, text);
#else
            std::cout << "\t- " << text << std::endl;
#endif // #ifdef DEST_CASIO_CALC
        }
    } // if (activated_)
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
