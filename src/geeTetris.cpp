//---------------------------------------------------------------------------
//--
//--	File	: jTetris.cpp
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

#include "shared/tabs.h"
#include "tetrisTabs.h"

#include "shared/keyboard.h"

#ifdef DEST_CASIO_CALC
#include <gint/gint.h>
#endif // #ifdef DEST_CASIO_CALC

// Program entry point
//
int main(){

    tetrisParameters params;
    TAB_VALUE value;

    // Create tabs
    //
    aboutTab tabAbout(TAB_ABOUT);
    tabValue tabShadow(TAB_SHADOW);

    // starting level
    tabRangedValue tabLevel(TAB_LEVEL, MIN_LEVEL, MAX_LEVEL);
    value.uVal = params.startLevel_;
    tabLevel.setValue(value);
    tabLevel.setComment(TAB_LEVEL_STR);

    // # of dirty lines
    tabRangedValue tabLines(TAB_LINES, MIN_DIRTY_LINES, MAX_DIRTY_LINES);
    value.uVal = params.dirtyLines_;
    tabLines.setValue(value);
    tabLines.setComment(TAB_DIRTY_LINES_STR);

    // display shadows ?
    value.bVal = params.shadow_;
    tabShadow.setValue(value);
    tabShadow.setComment(TAB_SHADOW_ON_STR, TAB_SHADOW_OFF_STR);

    // the game
    tetrisTab tabTetris(TAB_PLAY);
    tabTetris.setParameters(&params);

    tab tabExit(TAB_QUIT, ACTION_QUIT);

    // Add tabs ...
    //
    tabManager tmanager;
    TAB_STATUS tStatus;
    tab::clearScreen();

    tmanager.add(&tabAbout, 0);
    tmanager.add(&tabLevel);    // should be 1
    tmanager.add(&tabLines);
    tmanager.add(&tabShadow);
    tmanager.add(&tabTetris, 4);
    tmanager.add(&tabExit, 5);

    // Handle keyboard
    tabKeyboard keys;
    bool quitApp(false);
    uint car(0);
    int8_t sel(0);

    tab* currentTab(tmanager.select(0));    // Select first tab
    if (currentTab){
        currentTab->select(tStatus);
    }

    do{
        // Wait for a key
        car = keys.getKey();

        if (car >= KEY_CODE_F1 && car <= KEY_CODE_F6){
            sel = car - KEY_CODE_F1;    // "F" key index - 1

            // Update drawings
            if (NULL != (currentTab = tmanager.select(sel))){
                // Give control to the tab
                currentTab->select(tStatus);

                // Retreive parameters
                switch (sel){
                    // Level (IDTab = 1)
                    case 1:
                        tabLevel.value(value);
                        params.startLevel_ = value.uVal;
                        break;

                    // Dirty lines
                    case 2:
                        tabLines.value(value);
                        params.dirtyLines_ = value.uVal;
                        break;

                    // A shadow ?
                    case 3:
                        tabShadow.value(value);
                        params.shadow_ = value.bVal;
                        break;
                }

                // An (exit) char ?
                keys.addKey(tStatus.exitKey);

                // What's next ?
                //
                switch (tStatus.action){
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
    //gint_setrestart(1);
    gint_osmenu();
#endif // #ifdef DEST_CASIO_CALC

    // Finished
    return 1;
}

// EOF
