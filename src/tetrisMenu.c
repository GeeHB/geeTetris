//----------------------------------------------------------------------
//--
//--    tetrisMenu.c
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
                    menu_.selectByIndex(0, true, true); // redraw menu
                    break;

                // About
                case IDM_ABOUT:
                    _onAbout();
                    break;

                // Parameters
                //

                // Shadow (or not shadow ...)
                case IDM_PARAMS_SHADOW:
                    // update bool
                    params_.shadow_ = menuBar::isBitSet(action.state,
                                                ITEM_STATE_CHECKED);
                    break;

                // Dirty lines
                case IDM_PARAMS_LINES:
                {
                    int newVal = _onChangeNumParam(params_.dirtyLines_,
                                    MIN_DIRTY_LINES, MAX_DIRTY_LINES,
                                    COMMENT_PARAMS_LEVEL);
                    if (newVal != -1){
                        params_.dirtyLines_ = newVal;
                    }

                    menu_.update();
                    break;
                }

                // Starting level
                case IDM_PARAMS_LEVEL:
                {
                    int newVal = _onChangeNumParam(params_.startLevel_,
                                    MIN_LEVEL, MAX_LEVEL,
                                    COMMENT_PARAMS_LEVEL);
                    if (newVal != -1){
                        params_.startLevel_ = newVal;
                    }

                    menu_.update();
                    break;
                }

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
    paramsMenu.appendCheckbox(IDM_PARAMS_SHADOW, IDS_PARAMS_SHADOW,
            params_.shadow_?ITEM_STATE_CHECKED:ITEM_STATE_DEFAULT);
    paramsMenu.appendItem(IDM_PARAMS_LINES, IDS_PARAMS_LINES);
    paramsMenu.appendItem(IDM_PARAMS_LEVEL, IDS_PARAMS_LEVEL);
    menu_.appendSubMenu(&paramsMenu, IDM_PARAMS, IDS_PARAMS);

    menu_.appendItem(IDM_ABOUT, IDS_ABOUT, ITEM_STATE_SELECTED);
    menu_.addItem(MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT);

    menu_.update();
}


// _onStart() : start a new game with current params
//
void tetrisMenu::_onStart(){
    _clearScreen();

    tetrisGame game(&params_);
    if (game.start()){
        game.showScores(game.score(),
            game.lines(), game.level());  // Show final score
    }

    menu_.selectByIndex(0, true);
    menu_.update();
}

// _onAbout() : Show "about" informations
//.
void tetrisMenu::_onAbout(){

    _clearScreen();

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

// Select a single value
//
void tetrisMenu::_selectValue(int8_t value, uint8_t min, uint8_t max,
                            int16_t  xPos, int16_t yPos, bool select){
    if (value >= min && value <= max){

#ifdef DEST_CASIO_CALC
        uint16_t x(xPos + (value - min) * VAL_RANGE_BOX_WIDTH);
        drect(x + 1, yPos + 1 ,
            x + VAL_RANGE_BOX_WIDTH - 1, yPos + VAL_RANGE_BOX_WIDTH - 1 ,
            select?VAL_COL_BK_SEL:VAL_COL_BK_UNSEL);
        dprint(x + 5, yPos + 3,
            select?VAL_COL_TXT_SEL:VAL_COL_TXT_UNSEL, "%d", value);
#endif // #ifdef DEST_CASIO_CALC
    }
}

// _createParamMenu() : Create sub-menu displayed on params' edition
//
//  @menu : Menu bar to modifiy
//
void tetrisMenu::_createParamMenu(menuBar& menu){
    menu.appendItem(IDM_PARAMS_OK, IDS_PARAMS_OK);
    menu.addItem(2, IDM_PARAMS_PREV, IDS_PARAMS_PREV);
    menu.addItem(3, IDM_PARAMS_NEXT, IDS_PARAMS_NEXT);
    menu.addItem(MENU_POS_RIGHT, IDM_PARAMS_CANCEL, IDS_PARAMS_CANCEL);
}

// EOF
