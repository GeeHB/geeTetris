//---------------------------------------------------------------------------
//--
//--	File	: keyboard.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of keyboard object
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TOOLS_KEYBOARD_h__
#define __GEE_TOOLS_KEYBOARD_h__    1

#include "casioCalcs.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

#ifdef DEST_CASIO_CALC
#include <gint/keyboard.h>
#else
#include <cstdint>
#include <cstdio>
typedef unsigned int uint;  // !!!
#endif // #ifdef DEST_CASIO_CALC

#define KEY_CODE_NONE   0

// Type of key modifiers
//
enum MOD_TYPE{
    MOD_NONE = 0,
    MOD_SHIFT = 1,
    MOD_ALPHA
};


//  keyboard object - Access to keyboard events
//

class keyboard{
public:

    // Construction
    keyboard(){
        mod_ = MOD_NONE;
    }

    // Destruction
    virtual ~keyboard(){}

    // Key event in the queue
    virtual uint getKey();

    // Status of modifiers
    bool isPressed(uint mod){
        return (mod != MOD_NONE && ((mod_ & mod) == mod));
    }

protected:
    uint mod_;      // Keyboard modifier
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TOOLS_KEYBOARD_h__

// EOF
