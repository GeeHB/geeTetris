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
    MOD_ALPHA = 2
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
        return (mod != MOD_NONE && _isSet(mod));
    }

private:
    // bitwise ops for modifiers
    bool _isSet(uint8_t bit){
        return ((mod_& bit) == bit);
    }
    void _set(uint8_t bit){
        mod_ |= bit;
    }
    void _unSet(uint8_t bit){
        mod_ = (mod_ & (~bit));
    }

protected:
    // Members
    uint mod_;      // Keyboard modifiers
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TOOLS_KEYBOARD_h__

// EOF
