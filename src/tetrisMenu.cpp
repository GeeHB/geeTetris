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

// _onChangeNumParam() : Change the value of a numeric parameter
//
//  @value : current parameter's value
//  @min, @max : min and max value
//  @comment : string to display (defining the parameter)
//
//  @result : new parameter value or -1 if canceled or on error
//
int8_t tetrisMenu::_onChangeNumParam(uint8_t value,
                    uint8_t min, uint8_t max, const char* comment){

    _clearScreen(false);

#ifdef DEST_CASIO_CALC
    // Position (centered)
    uint16_t xPos((CASIO_WIDTH - VAL_RANGE_BOX_WIDTH * (max - min + 1)) / 2);
    uint16_t yPos(int((CASIO_HEIGHT - menu_.getHeight()) / 2));

    // Draw all possible numbers
    uint8_t maxCount(max - min);
    uint16_t x(xPos);

    for (uint8_t index(0); index<=maxCount; index++){
        drect_border(x, yPos,
                    x + VAL_RANGE_BOX_WIDTH,
                    yPos + VAL_RANGE_BOX_WIDTH,
                    NO_COLOR, 1, VAL_COL_BORDER);
        dprint(x + 5, yPos + 3,
                VAL_COL_TXT_UNSEL,
                "%d", (index+min));
        x+=VAL_RANGE_BOX_WIDTH;
    }

    if (comment){
        dtext(VAL_RANGE_COMMENT_X, yPos - VAL_COMMENT_Y_OFFSET,
            VAL_COL_BORDER, comment);
    }

    // Select current val.
    int8_t oldVal(-1), newVal(value);
     _selectValue(newVal, min, max, xPos, yPos);
    dupdate();

    // Timer for blinking effect
    int tickCount(BLINK_TICKCOUNT);
    static volatile int tick(1);
    int timerID = timer_configure(TIMER_ANY, BLINK_TICK_DURATION*1000,
                    GINT_CALL(__callbackTick, &tick));
    if (timerID >= 0){
        timer_start(timerID);   // set the timer
    }
    // Install parameters' specific menu-bar
    menuBar menu;
    MENUACTION action;
    _createParamMenu(menu);
    menu.update();

    // Handle menu
    bool cont(true), redraw(false), showSelected(false);
    while (cont){
        if (timerID >= 0){
            while(!tick){
                sleep();
            }
            tick = 0;

            // Time to blink ?
            if (!(tickCount--)){
                // Blink
                showSelected = !showSelected;
                tickCount = BLINK_TICKCOUNT;
                redraw = true;
            }
        }

        // A keyboard event ?
        action = menu.handleKeyboard();
        switch (action.value){

            // Prev. value
            case KEY_CODE_LEFT:
            case IDM_PARAMS_PREV:
                --newVal;
                newVal = IN_RANGE(newVal, min, max);
                menu.selectByIndex(2, true, true);
                break;

            // Next value
            case KEY_CODE_RIGHT:
            case IDM_PARAMS_NEXT:
                ++newVal;
                newVal = IN_RANGE(newVal, min, max);
                menu.selectByIndex(3, true, true);
                break;

            // End
            case IDM_PARAMS_CANCEL:
            case IDM_PARAMS_OK:
                cont = false;
                break;

            default:
                break;
        } // switch(action.value)

         if (redraw || oldVal != newVal){
            if (oldVal != newVal){
                // Erase previous val.
                _selectValue(oldVal, min, max, xPos, yPos, false);
            }

            _selectValue(newVal, min, max, xPos, yPos, showSelected);
            dupdate();
            oldVal = newVal;
            redraw = false;
         }
    }   // while(cont)

    if (action.value == IDM_PARAMS_OK){
        _selectValue(newVal, min, max, xPos, yPos, true);
        dupdate();

        return newVal;
    }

    if (timerID >= 0){
        timer_stop(timerID);    // stop the timer
    }

    // Canceled
    _selectValue(newVal, min, max, xPos, yPos, false);
    _selectValue(value, min, max, xPos, yPos, true);
    dupdate();

    return -1;
#endif // #ifdef DEST_CASIO_CALC

    return -1;  // !!!
}

// _clearScreen() : Clear the whole screen
//
//  @update : update screen ?
//
void tetrisMenu::_clearScreen(bool update){
#ifdef DEST_CASIO_CALC
    //dclear(COLOUR_WHITE);
    drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - menu_.getHeight() - 1,
            C_WHITE);

    if (update){
        dupdate();
    }
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

#ifdef DEST_CASIO_CALC
// __callbackTick() : Call back function for timer
// This function is used during param. edition to make item blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
int tetrisMenu::__callbackTick(volatile int *pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}
#endif // #ifdef DEST_CASIO_CALC

// EOF
