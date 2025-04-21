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

    clearScreen(COLOUR_WHITE, MENUBAR_DEF_HEIGHT, FALSE);

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
     _selectValue(newVal, min, max, xPos, yPos);
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
    menuBar menu;
    MENUACTION action;
    _createParamMenu(menu);
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


// EOF
