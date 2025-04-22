//----------------------------------------------------------------------
//--
//--    geeTetris.c
//--
//--        App. entry point
//--
//----------------------------------------------------------------------

#include "shared/menu.h"
#include "tetrisParameters.h"

#include "menuConsts.h"

#include "tetrisGame.h"

// Background image
#ifdef DEST_CASIO_CALC
extern bopti_image_t g_imgAbout;
#endif // #ifdef DEST_CASIO_CALC


// _onAbout() : Show "about" informations
//.
void _onAbout(){

    clearScreenEx(COLOUR_WHITE, 0, FALSE);

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

// _onStart() : start a new game with current params
//
void _onStart(POWNMENU menu, PPARAMS const params){
    clearScreen(COLOUR_WHITE);

    TETRISGAME game;
    tetrisGame_init(&game, params);

    if (tetrisGame_start(&game)){
        /*
        game.showScores(game.score(),
            game.lines(), game.level());  // Show final score
            */
    }

    menubar_selectByIndex(menu_getMenuBar(menu), 0, TRUE);
    menu_update(menu);
}

// _createMenu() : Create app. menu
//
POWNMENU _createMenu(PPARAMS params){
    POWNMENU menu = menu_create();
    if (menu){
        PMENUBAR bar = menu_getMenuBar(menu);
        menubar_appendItem(bar, IDM_START, IDS_START, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);

        // "Params" sub-menu
        PMENUBAR paramsMenu = menubar_create();
        if (paramsMenu){
            menubar_appendCheckbox(paramsMenu, IDM_PARAMS_SHADOW, IDS_PARAMS_SHADOW, params->shadow?ITEM_STATE_CHECKED:ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
            menubar_appendItem(paramsMenu, IDM_PARAMS_LINES, IDS_PARAMS_LINES, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
            menubar_appendItem(paramsMenu, IDM_PARAMS_LEVEL, IDS_PARAMS_LEVEL, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);

            menubar_appendSubMenu(bar, paramsMenu, IDM_PARAMS, IDS_PARAMS, ITEM_STATE_DEFAULT, ITEM_STATUS_DEFAULT);
        }

        menubar_appendItem(bar, IDM_ABOUT, IDS_ABOUT, ITEM_STATE_SELECTED, ITEM_STATUS_TEXT);
        menubar_addItem(bar, MENU_POS_RIGHT, IDM_QUIT, IDS_QUIT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);

        menu_update(menu);
    }

    return menu;
}

// _run() : start/run the app.
//
void _run(POWNMENU menu, PPARAMS params){
    // Handle user's choices
    BOOL end = FALSE;
    MENUACTION action;
    while (!end){
        // A menu action ?
        if (menu_handleKeyboard(menu, &action)){
            // user pushed a menu key ?
            if (ACTION_MENU == action.type){
                switch (action.value){
                    // Start a new game
                    case IDM_START:
                        _onStart(menu, params);
                        menubar_selectByIndex(menu_getMenuBar(menu), 0, TRUE);
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
                        params->shadow = isBitSet(action.state, ITEM_STATE_CHECKED);
                        break;

                    // Dirty lines
                    case IDM_PARAMS_LINES:
                    {
                        int newVal = params_changeNumValue(params->dirtyLines,
                                        MIN_DIRTY_LINES, MAX_DIRTY_LINES,
                                        COMMENT_PARAMS_LEVEL);
                        if (newVal != -1){
                            params->dirtyLines = newVal;
                        }

                        menu_update(menu);
                        break;
                    }

                    // Starting level
                    case IDM_PARAMS_LEVEL:
                    {
                        int newVal = params_changeNumValue(params->startLevel,
                                        MIN_LEVEL, MAX_LEVEL,
                                        COMMENT_PARAMS_LEVEL);
                        if (newVal != -1){
                            params->startLevel = newVal;
                        }

                        menu_update(menu);
                        break;
                    }

                    // Quit the application
                    case IDM_QUIT:
                        end = TRUE;
                        break;

                    default:
                        break;  // Unknown command
                }
            }
        }

    } // while (!end)
}

// Program entry point
//
int main(){
    POWNMENU mainMenu = NULL;
    tetrisParameters params;

    _onAbout();

    params_init(&params);       // Default parameters
    mainMenu = _createMenu(&params);

    if (mainMenu){
        _run(mainMenu, &params);
        menu_free(mainMenu);
    }

#ifdef DEST_CASIO_CALC
    gint_setrestart(1);
    //gint_osmenu();
#endif // #ifdef DEST_CASIO_CALC

    // Finished
    return 1;
}

// EOF
