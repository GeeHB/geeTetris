//----------------------------------------------------------------------
//--
//--    tetrisMenu.h
//--
//--        Definition of tetrisMenu
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_TETRIS_MENU_h__
#define __GEE_TETRIS_CASIO_TETRIS_MENU_h__    1

#include "consts.h"

#include "menus.h"
#include "shared/scrCapture.h"

#include "tetrisGame.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//----------------------------------------------------------------------
//--
//-- tetrisMenu - Menu bar for app.
//--
//----------------------------------------------------------------------

class tetrisMenu{
public:
    // Construction
    tetrisMenu();

    // clearScreen() : Clear the whole screen
    //
    void clearScreen();

    // about() : Show "About params."
    //.
    void about(){
        _onAbout();
    }

    // run() : start/run the app.
    //
    void run();

    // Internal methods
private:

    // _createMenu() : Create app. menu bar
    //
    void _createMenu();

    // _onAbout() : Show "about" informations
    //
    void _onAbout();

    // _onStart() : start a new game with current params
    //
    void _onStart();

    // Members
private:
    menuBar     menu_;         // Application menu
    tetrisParameters  params_; // Game's parameters
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_TETRIS_MENU_h__

// EOF
