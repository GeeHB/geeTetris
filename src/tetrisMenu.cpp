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
    _createMenu();
}

// clearScreen() : Clear the whole screen
//
void tetrisMenu::clearScreen(){
#ifdef DEST_CASIO_CALC
    dclear(COLOUR_WHITE);
    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

// run() : start/run the app.
//
void tetrisMenu::run(){
    // Handle user's choices
    bool end(false);
    MENUACTION action;
    while (!end){
        // A menu action ?
        action = menu_.handleKeyboard();

        // user pushed a menu key ?
        if (ACTION_MENU == action.type){
            switch (action.value){

                // Start a new game
                case IDM_START:
                    _onStart();
                    break;

                // About
                case IDM_ABOUT:
                    _onAbout();
                    break;

                // Quit the application
                case IDM_QUIT:
                    end = true;
                    break;

                default:
                    break;  // Unknown command
            }
        }

    } // while (!end)
}

//
// Internal methods
//

// _createMenu() : Create app. menu bar
//
void tetrisMenu::_createMenu(){

    menu_.appendItem(IDM_START, IDS_START);

    // "Params" sub-menu
    menuBar paramsMenu;
    paramsMenu.appendItem(IDM_PARAMS_SHADOW, IDS_PARAMS_SHADOW);
    paramsMenu.appendItem(IDM_PARAMS_LINES, IDS_PARAMS_LINES);
    paramsMenu.appendItem(IDM_PARAMS_LEVEL, IDS_PARAMS_LEVEL);
    menu_.appendSubMenu(&paramsMenu, IDM_PARAMS, IDS_PARAMS);

    menu_.appendItem(IDM_ABOUT, IDS_ABOUT);
    menu_.addItem(MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT);

    menu_.update();
}


// _onStart() : start a new game with current params
//
void tetrisMenu::_onStart(){
    clearScreen();

    tetrisGame game(&params_);
    if (game.start()){
        game.showScores(game.score(),
            game.lines(), game.level());  // Show final score
    }
}

// _onAbout() : Show "about" informations
//.
void tetrisMenu::_onAbout(){

    clearScreen();

    // Draw the image and copyright
#ifdef DEST_CASIO_CALC
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
    dtext(CASIO_WIDTH - w - 5,
            CASIO_HEIGHT - menu_.getHeight() - h - 10,
            COLOUR_BLACK, copyright);

    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

// EOF
