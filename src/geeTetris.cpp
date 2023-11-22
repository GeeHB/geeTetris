//---------------------------------------------------------------------------
//--
//--	File	: geeTetris.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris / cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--	    Entry point
//--
//---------------------------------------------------------------------------

#include "consts.h"

#include "shared/tabs.h"
#include "tetrisTabs.h"

#include "shared/keyboard.h"

#ifdef TRACE_MODE
#include "tetrisGame.h"
#endif

// Program entry point
//
int main(){

    tetrisParameters params;
    tabKeyboard keys;

    // Create tabs
    //
    aboutTab tabAbout(TAB_ABOUT);

    tabValue tabShadow(TAB_SHADOW, &params.shadow_);
    tabShadow.setComment(TAB_SHADOW_ON_STR, TAB_SHADOW_OFF_STR);

    // starting level
    tabRangedValue tabLevel(TAB_LEVEL, &params.startLevel_, MIN_LEVEL, MAX_LEVEL, &keys);
    tabLevel.setComment(TAB_LEVEL_STR);

    // # of dirty lines
    tabRangedValue tabLines(TAB_LINES, &params.dirtyLines_, MIN_DIRTY_LINES, MAX_DIRTY_LINES, &keys);
    tabLines.setComment(TAB_DIRTY_LINES_STR);

    // the game
    tetrisTab tabTetris(TAB_PLAY);
    tabTetris.setParameters(&params);

    // exit
    tab tabExit(TAB_QUIT, ACTION_QUIT);

    // Add tabs ...
    //
    tabManager tmanager;
    tab::clearScreen();

    tmanager.add(&tabAbout, 0);
    tmanager.add(&tabLevel);    // should be 1
    tmanager.add(&tabLines);
    tmanager.add(&tabShadow);
    tmanager.add(&tabTetris, 4);
    tmanager.add(&tabExit, 5);

    // Handle keyboard events
    bool quitApp(false);
    uint car(0);
    int8_t sel(0), action(ACTION_NONE);

    tab* currentTab(tmanager.select(0));    // Select first tab
    if (currentTab){
        currentTab->select();
    }

    do{
        // Wait for a key
        car = keys.getKey();

        if (car >= KEY_CODE_F1 && car <= KEY_CODE_F6){
            sel = car - KEY_CODE_F1;    // "F" key index - 1

            // Update drawings
            if (NULL != (currentTab = tmanager.select(sel))){
                // Give control to the tab
                action = currentTab->select();

                // What's next ?
                switch (action){
                    case ACTION_REDRAW_TABS:
                        tmanager.update();
                        break;
                    case ACTION_QUIT:
                        quitApp = true;
                        break;
                    default:
                        break;
                }
            }
        }else{
            // The "Exit" key
            if (KEY_CODE_EXIT == car){
                quitApp = true;
            }
        }

    }while(!quitApp);

    // Return to default state
    tmanager.select(0);

    // Free memory
#ifdef DEST_CASIO_CALC
    gint_setrestart(1);
    //gint_osmenu();
#endif // #ifdef DEST_CASIO_CALC

    // Finished
    return 1;
}

// EOF
