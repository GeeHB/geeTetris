//----------------------------------------------------------------------
//--
//--    tetrisParameters.c
//--
//--        Game's parameters
//--
//----------------------------------------------------------------------

#include"tetrisParameters.h"
#include "shared/menu.h"

#ifdef DEST_CASIO_CALC
#include <gint/clock.h>
#include <gint/timer.h>
#endif // #ifdef DEST_CASIO_CALC

//
// Parameters' menu
//

#define IDM_PARAMS_OK           201
#define IDS_PARAMS_OK           "Ok"

#define IDM_PARAMS_PREV         202
#define IDS_PARAMS_PREV         "<<"

#define IDM_PARAMS_NEXT         203
#define IDS_PARAMS_NEXT         ">>"

#define IDM_PARAMS_CANCEL       204
#define IDS_PARAMS_CANCEL       "Cancel"

// Parameters sub-menu
//
#define IDM_PARAMS_SHADOW       21
#define IDS_PARAMS_SHADOW       "Shadow"

#define IDM_PARAMS_LINES        22
#define IDS_PARAMS_LINES        "Lines"
#define COMMENT_PARAMS_LINES    "'Dirty' lines :"

#define IDM_PARAMS_LEVEL        23
#define IDS_PARAMS_LEVEL        "Level"
#define COMMENT_PARAMS_LEVEL    "Starting level :"

//
// Int. functions
//

void _createMenu(PMENUBAR menu);
void _selectValue(int8_t value, uint8_t min, uint8_t max, int16_t  xPos, int16_t yPos, BOOL select);

//
// Public functions
//

BOOL params_init(PPARAMS const params){
    if (params){
        // Set default parameters
        params->startLevel = MIN_LEVEL;
        params->dirtyLines = MIN_DIRTY_LINES;
        params->shadow = (DEF_SHADOW == 1);
        params->orientation = CALC_VERTICAL; // by default, no screen rotation
        return TRUE;
    }

    return FALSE;
}

BOOL params_copy(PPARAMS const params, PPARAMS const other){
    if (params && other){
        params->startLevel = other->startLevel;
        params->dirtyLines = other->dirtyLines;
        params->shadow = other->shadow;
        params->orientation = other->orientation;

        return TRUE;
    }

    return FALSE;
}

#ifdef DEST_CASIO_CALC
// __callbackTickMenu() : Call back function for timer
// This function is used during param. edition to make item blink
//
//  @pTick : pointer to blinking state indicator
//
//  @return : TIMER_CONTINUE if valid
//
static int __callbackTickMenu(volatile int *pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}
#endif // #ifdef DEST_CASIO_CALC


// params_changeNumValue() : Change the value of a numeric parameter
//
//  @value : current parameter's value
//  @min, @max : min and max value
//  @comment : string to display (defining the parameter)
//
//  @result : new parameter value or -1 if canceled or on error
//
int8_t params_changeNumValue(uint8_t value,
                    uint8_t min, uint8_t max, const char* comment){

    clearScreenEx(COLOUR_WHITE, MENUBAR_DEF_HEIGHT, FALSE);

#ifdef DEST_CASIO_CALC
    // Position (centered)
    uint16_t xPos = (CASIO_WIDTH - VAL_RANGE_BOX_WIDTH * (max - min + 1)) / 2;
    uint16_t yPos = (int)((CASIO_HEIGHT - menu_.getHeight()) / 2);

    // Draw all possible numbers
    uint8_t maxCount = max - min;
    uint16_t x = xPos;

    for (uint8_t index = 0; index<=maxCount; index++){
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
    int8_t oldVal = -1, newVal = value;
     _selectValue(newVal, min, max, xPos, yPos, TRUE);
    dupdate();

    // Timer for blinking effect
    int tickCount = BLINK_TICKCOUNT;
    static volatile int tick = 1;
    int timerID = timer_configure(TIMER_ANY, BLINK_TICK_DURATION*1000,
                    GINT_CALL(__callbackTickMenu, &tick));
    if (timerID >= 0){
        timer_start(timerID);   // set the timer
    }
    // Install parameters' specific menu-bar
    MENUACTION action;
    POWNMENU menu = menu_create();
    PMENUBAR bar = menu_getMenuBar(menu);

    _createMenu(bar);
    menu.update();

    // Handle menu
    BOOL cont = TRUE, redraw = FALSE, showSelected = FALSE;
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
                redraw = TRUE;
            }
        }

        // A keyboard event ?
        if (menu_handleKeyboard(menu, &action)){
            switch (action.value){

                // Prev. value
                case KEY_CODE_LEFT:
                case IDM_PARAMS_PREV:
                    --newVal;
                    newVal = SET_IN_RANGE(newVal, min, max);
                    menubar_selectByIndex(bar, 2, TRUE);
                    break;

                // Next value
                case KEY_CODE_RIGHT:
                case IDM_PARAMS_NEXT:
                    ++newVal;
                    newVal = SET_IN_RANGE(newVal, min, max);
                    menubar_selectByIndex(bar, 3, TRUE);
                    break;

                // End
                case IDM_PARAMS_CANCEL:
                case IDM_PARAMS_OK:
                    cont = FALSE;
                    break;

                default:
                    break;
            } // switch(action.value)

            if (redraw || oldVal != newVal){
                if (oldVal != newVal){
                    // Erase previous val.
                    _selectValue(oldVal, min, max, xPos, yPos, FALSE);
                }

                _selectValue(newVal, min, max, xPos, yPos, showSelected);
                dupdate();
                oldVal = newVal;
                redraw = FALSE;
            }
        }
    }   // while(cont)

    menu_free(menu);

    if (action.value == IDM_PARAMS_OK){
        _selectValue(newVal, min, max, xPos, yPos, TRUE);
        dupdate();

        return newVal;
    }

    if (timerID >= 0){
        timer_stop(timerID);    // stop the timer
    }

    // Canceled
    _selectValue(newVal, min, max, xPos, yPos, FALSE);
    _selectValue(value, min, max, xPos, yPos, TRUE);
    dupdate();

    return -1;
#endif // #ifdef DEST_CASIO_CALC

    return -1;  // !!!
}

//
// Int. functions
//

// Select a single value
//
void _selectValue(int8_t value, uint8_t min, uint8_t max, int16_t  xPos, int16_t yPos, BOOL select){
    if (value >= min && value <= max){

#ifdef DEST_CASIO_CALC
        uint16_t x = (xPos + (value - min) * VAL_RANGE_BOX_WIDTH);
        drect(x + 1, yPos + 1 ,
            x + VAL_RANGE_BOX_WIDTH - 1, yPos + VAL_RANGE_BOX_WIDTH - 1 ,
            select?VAL_COL_BK_SEL:VAL_COL_BK_UNSEL);
        dprint(x + 5, yPos + 3,
            select?VAL_COL_TXT_SEL:VAL_COL_TXT_UNSEL, "%d", value);
#endif // #ifdef DEST_CASIO_CALC
    }
}

// _createMenu() : Create sub-menu displayed on params' edition
//
//  @menu : Menu bar to modifiy
//
void _createMenu(PMENUBAR menu){
    menubar_appendItem(menu, IDM_PARAMS_OK, IDS_PARAMS_OK, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
    menubar_addItem(menu, 2, IDM_PARAMS_PREV, IDS_PARAMS_PREV, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
    menubar_addItem(menu, 3, IDM_PARAMS_NEXT, IDS_PARAMS_NEXT, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
    menubar_addItem(menu, MENU_POS_RIGHT, IDM_PARAMS_CANCEL, IDS_PARAMS_CANCEL, ITEM_STATE_DEFAULT, ITEM_STATUS_TEXT);
}

// EOF
