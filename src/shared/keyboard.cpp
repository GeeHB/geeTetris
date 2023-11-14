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
uint16_t keyboard::getKey(){
    uint16_t key(KEY_CODE_NONE);
#ifdef DEST_CASIO_CALC
    key_event_t evt;
#endif // #ifdef DEST_CASIO_CALC

#ifdef DEST_CASIO_CALC
    evt = pollevent();
    if (evt.type == KEYEV_DOWN){
        key = evt.key;

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
        else{
            mod_ = MOD_TYPE::MOD_NONE;
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
