//---------------------------------------------------------------------------
//--
//--	File	: casioCalc.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of casioCalc object
//--
//---------------------------------------------------------------------------

#include "casioCalc.h"

// Fonts
#ifdef DEST_CASIO_CALC
extern font_t font_horz;
extern font_t font_vert;
#endif // #ifdef DEST_CASIO_CALC

//---------------------------------------------------------------------------
//--
//-- casioCalc object
//--
//--    methods, constants (coordinates, dimensions) for the casio calculators
//--
//---------------------------------------------------------------------------

// Construction
//
casioCalc::casioCalc(){

    rotatedDisplay_ = false;

    // Fonts
#ifdef DEST_CASIO_CALC
    hFont_ = &font_horz;
    vFont_ = &font_vert;
#endif // #ifdef DEST_CASIO_CALC

    // Default keys
    keyFall_ = KEY_CODE_FALL;
    keyPause_ = KEY_CODE_PAUSE;
    keyRotateDisplay_ = KEY_CODE_ROTATE_DISPLAY;
    keyQuit_ = KEY_CODE_EXIT;

    // set parameters
    rotatedDisplay(rotatedDisplay_);
}

// capture() : Set/unset screen capture
//
//  @activate : set (=true) or remove (=false) screen capture
//
//  return the status
bool casioCalc::capture(bool activate){
#ifdef DEST_CASIO_CALC
    if (activate){
        capture_.install();
    }
    else{
        capture_.remove();
    }

    return capture_.isSet();
#else
    return false;
#endif // #ifdef DEST_CASIO_CALC
}

// rotatedDisplay() : Update members on rotation
//
//  @doRotate : indicates wether display must rotate or not
//
void casioCalc::rotatedDisplay(bool doRotate){
    if (false == (rotatedDisplay_ = doRotate)){
        boxWidth_ = CASIO_BOX_WIDTH;

        playfield_pos_.x = CASIO_PLAYFIELD_LEFT + CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
        playfield_pos_.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

        playfield_width = PLAYFIELD_WIDTH * boxWidth_ + 2 * CASIO_BORDER_GAP;
        playfield_height = PLAYFIELD_HEIGHT * boxWidth_ + 2 * CASIO_BORDER_GAP;

        NP_boxWidth_ = CASIO_BOX_WIDTH_NP;
        NP_width_ = 4 * NP_boxWidth_ + 2 * CASIO_INFO_GAP;

        NP_pos_.x = CASIO_INFO_LEFT + CASIO_INFO_GAP;
        if (NP_pos_.x <= (playfield_pos_.x + playfield_width)){
            NP_pos_.x = playfield_pos_.x + playfield_width + 2 * CASIO_INFO_GAP;
        }

        NP_pos_.y = CASIO_INFO_TOP;

        // Keys
        keyLeft_ = KEY_CODE_LEFT;
        keyRight_ = KEY_CODE_RIGHT;
        keyRotatePiece_ = KEY_CODE_UP;
        keyDown_ = KEY_CODE_DOWN;

        // Use "default" font
#ifdef DEST_CASIO_CALC
        dfont((font_t*)&font_horz);
#endif // #ifdef DEST_CASIO_CALC
    }
    else {
        // "rotated" mode
        //
        boxWidth_ = CASIO_BOX_WIDTH_ROTATED;    // Larger box

        playfield_pos_.x = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
        playfield_pos_.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

        playfield_width = PLAYFIELD_WIDTH * boxWidth_ + 2 * CASIO_BORDER_GAP;
        playfield_height = PLAYFIELD_HEIGHT * boxWidth_ + 2 * CASIO_BORDER_GAP;

        NP_boxWidth_ = CASIO_BOX_WIDTH_NP_ROTATED;  // ... but preview is smaller
        NP_width_ = 4 * NP_boxWidth_ + 2 * CASIO_INFO_GAP;

        NP_pos_.x = playfield_pos_.x + playfield_width + CASIO_BORDER_GAP;
        NP_pos_.y = CASIO_INFO_TOP;

        // Keys
        keyLeft_ = KEY_CODE_DOWN;
        keyRight_ = KEY_CODE_UP;
        keyRotatePiece_ = KEY_CODE_LEFT;
        keyDown_ = KEY_CODE_RIGHT;

        // Install my font
#ifdef DEST_CASIO_CALC
        dfont((font_t*)&font_vert);
#endif // #ifdef DEST_CASIO_CALC
    }

    // Values indicators
    textsPos_[0].x = textsPos_[1].x = textsPos_[2].x = NP_pos_.x;
    for (uint8_t id(0); id <VAL_COUNT; id++){
        textsPos_[id].y = NP_pos_.y + NP_width_ + boxWidth_ * ( 2 * id + 1);
    }
}

// EOF
