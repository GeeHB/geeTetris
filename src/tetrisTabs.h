//---------------------------------------------------------------------------
//--
//--	File	: tetrisTabs.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of tetrisTab and aboutTab
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_TETRIS_TABS_h__
#define __GEE_TETRIS_CASIO_TETRIS_TABS_h__    1

#include "shared/tabs.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus


//---------------------------------------------------------------------------
//--
//-- aboutTab - Splash screen
//--
//---------------------------------------------------------------------------

class aboutTab : public tab{
public:
    // Construction
    aboutTab(const char* tname)
    :tab(tname, ACTION_NONE){
    }

    // The current tab is selected => show splash screen
    uint8_t select();
};

//---------------------------------------------------------------------------
//--
//-- tetrisTab - Where the game is
//--
//---------------------------------------------------------------------------

class tetrisTab : public tab{
public:
    // Construction
    tetrisTab(const char* tname)
    :tab(tname, ACTION_NONE){
        params_ = NULL;     // No parameters
    }

    // Parameters
    void setParameters(tetrisParameters* params){
        params_ = params;
    }

    // The current tab is selected => let's play
    uint8_t select();

private:
    // Members
    tetrisParameters* params_;
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_TETRIS_TABS_h__

// EOF
