//---------------------------------------------------------------------------
//--
//--	File	: tetrisTabs.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of tetrisTab and aboutTab
//--
//---------------------------------------------------------------------------

#include "tetrisTabs.h"
#include "tetrisGame.h"

// Background image
#ifdef DEST_CASIO_CALC
extern bopti_image_t img_geeTetris;
#endif // #ifdef DEST_CASIO_CALC

//
// aboutTab
//

// The current tab is selected => show splash screen
//
uint8_t aboutTab::select(){
    // Draw the image and text
#ifdef DEST_CASIO_CALC
    clearScreen();
    dimage(0, 0, &img_geeTetris);

    char copyright[255];    // Should be enough !
    strcpy(copyright, APP_NAME);
    strcat(copyright, " par ");
    strcat(copyright, APP_AUTHOR);
    strcat(copyright, " version ");
    strcat(copyright, APP_VERSION);

    int w, h;
    dsize(copyright, NULL, &w, &h);
    dtext(CASIO_WIDTH - w - 5, CASIO_HEIGHT - TAB_HEIGHT - h - 5, COLOUR_BLACK, copyright);

    dupdate();
#endif // #ifdef DEST_CASIO_CALC

    // Nothing special
    return tab::select();
}

//
// tetrisTab
//

// The current tab is selected => let's play
//
uint8_t tetrisTab::select(){

    uint8_t action(tab::select());

    if (params_){
#ifdef DEST_CASIO_CALC
        clearScreen();
#endif // #ifdef DEST_CASIO_CALC

        tetrisGame game(params_);
        if (game.start()){
            game.showScores(game.score(), game.lines(), game.level());  // Show final score
        }

        // Ready for a new game !
        action = ACTION_REDRAW_TABS;
    }
    else{
        // No parameters => no game
        action = ACTION_NONE;
    }

    return action;
}

// EOF
