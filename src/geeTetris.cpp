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

#include "tabs.h"
#include "tetrisTabs.h"

#ifdef DEST_CASIO_FXCG50
#include <gint/gint.h>

#ifdef CAPTURE_MODE
#include <gint/usb.h>
#include <gint/usb-ff-bulk.h>
#endif // CAPTURE_MODE

#endif // #ifdef DEST_CASIO_FXCG50

// Program entry point
//
int main(){

#ifdef CAPTURE_MODE
    // Liste des interfaces à ouvrir
    usb_interface_t const *intf[] = { &usb_ff_bulk, NULL };

    // On ouvre et on attend que la connexion soit établie
    usb_open((usb_interface_t const **)&intf, GINT_CALL_NULL);
    usb_open_wait();

    // Mise en place du hook au niveau de dupdate
    dupdate_set_hook(GINT_CALL(usb_fxlink_videocapture, 0));
#endif // #ifdef CAPTURE_MODE

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

    // Select the first
    tmanager.select(0)->select(tStatus);

    // Handle options
    bool quitApp(false), getNextKey(true);
    uint car(0);
    int8_t sel(0);
    tab* currentTab;

#ifdef DEST_CASIO_FXCG50
    key_event_t evt;
#endif // #ifdef DEST_CASIO_FXCG50
    do{
        if (getNextKey){
#ifdef DEST_CASIO_FXCG50
            evt = pollevent();
            if (evt.type == KEYEV_DOWN){
                car = evt.key;
            }
            else{
                car = 0;    // ie. no char ...
            }
#else
            car = getchar();
#endif // #ifdef DEST_CASIO_FXCG50
        }
        else{
            getNextKey = true; // Next time, read the keyboard state
        }

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
                car = tStatus.exitKey;
                getNextKey = (car == KEY_NONE); // no char. from previous tab => read keyboard

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
    } while (!quitApp);

    // Return to default state
    tmanager.select(0);

    // End of capture
#ifdef CAPTURE_MODE
    dupdate_set_hook(GINT_CALL_NULL);
#endif // #ifdef CAPTURE_MODE

    // Free memory
#ifdef DEST_CASIO_FXCG50
    gint_setrestart(1);
#endif // #ifdef DEST_CASIO_FXCG50

    // Finished
    return 1;
}

// EOF
