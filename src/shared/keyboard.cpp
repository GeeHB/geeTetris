//---------------------------------------------------------------------------
//--
//--	File	: keyboard.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of keyboard object
//--
//---------------------------------------------------------------------------

#include "keyboard.h"

// Key event in the queue
//
uint keyboard::getKey(){
    uint key(KEY_CODE_NONE);

#ifdef DEST_CASIO_CALC
    key_event_t evt;
    evt = pollevent();
    if (evt.type == KEYEV_DOWN){
        key = evt.key;
        mod_ = MOD_TYPE::MOD_NONE;

        // ???
        // mod_  = (evt.mod?evt.shift + 2*evt.alpha:0);

        // A moodifier ?
        if (evt.mod){
            if (evt.shift){
                mod_+= MOD_TYPE::MOD_SHIFT;
            }

            if (evt.alpha){
                mod_+= MOD_TYPE::MOD_ALPHA;
            }
        }
    }
    else{
        key = KEY_CODE_NONE;
        mod_ = MOD_TYPE::MOD_NONE;
    }
#else
    key = getchar();
    mod_ = MOD_TYPE::MOD_NONE;
#endif // #ifdef DEST_CASIO_CALC

    // return the pressed key code
    return key;
}

// EOF
