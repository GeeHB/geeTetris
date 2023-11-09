//---------------------------------------------------------------------------
//--
//--	File	: tabs.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: jtetris - cpp version
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
#ifdef DEST_CASIO_FXCG50
extern bopti_image_t img_geeTetris;
#endif // #ifdef DEST_CASIO_FXCG50

//
// aboutTab
//

// The current tab is selected => show splash screen
//
void aboutTab::select(TAB_STATUS& status){
    // Draw the image
#ifdef DEST_CASIO_FXCG50
    dimage(0, 0, &img_geeTetris);
#endif // #ifdef DEST_CASIO_FXCG50

    // Nothing special
    status.action = ACTION_NONE;
    status.exitKey = KEY_NONE;
}

//
// tetrisTab
//

// The current tab is selected => let's play
//
void tetrisTab::select(TAB_STATUS& status){

    if (params_){
        tetrisGame game(params_);
        if (game.start()){
            _showScores(game.score());  // Show final score
        }

        // Ready for a new game !
        status.action = ACTION_REDRAW_TABS;
        status.exitKey = KEY_NONE;
    }
    else{
        // No parameters => no game
        status.action = ACTION_NONE;
        status.exitKey = KEY_CODE_F1;       // return to main screen
    }
}

// EOF
