//----------------------------------------------------------------------
//--
//--    tetrisMenu.cpp
//--
//--        Implementation of tetrisMenu
//--
//----------------------------------------------------------------------

#include "tetrisMenu.h"

// Background image
#ifdef DEST_CASIO_CALC
extern bopti_image_t g_imgAbout;
#endif // #ifdef DEST_CASIO_CALC

// Construction
//
tetrisMenu::tetrisMenu(){
}

// about() : Show about "box"
//.
void tetrisMenu::about(){
    // Draw the image and copyright
#ifdef DEST_CASIO_CALC
    clearScreen();
    dimage(0, 0, &g_imgAbout);

    char copyright[255];    // Should be enough !
    strcpy(copyright, APP_NAME);
#ifdef FXCG50
    strcat(copyright, " par ");
    strcat(copyright, APP_AUTHOR);
#endif // #ifdef FX9860G
    strcat(copyright, " v");
    strcat(copyright, APP_VERSION);

    int w, h;
    dsize(copyright, NULL, &w, &h);
    dtext(CASIO_WIDTH - w - 5, CASIO_HEIGHT - TAB_HEIGHT - h - 10,
            COLOUR_BLACK, copyright);

    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

// _onStart() : start a new game with current params
//
void tetrisMenu::_onStart(){
#ifdef DEST_CASIO_CALC
    clearScreen();
#endif // #ifdef DEST_CASIO_CALC
    tetrisGame game(&params_);
    if (game.start()){
        game.showScores(game.score(),
            game.lines(), game.level());  // Show final score
    }
}

// EOF
