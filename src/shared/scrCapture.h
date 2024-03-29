//----------------------------------------------------------------------
//--
//--    scrCapture.h
//--
//--    Definition of scrCapture - Screen capture using fxlink
//--
//----------------------------------------------------------------------

#ifndef __CASIO_CALC_SCREEN_CAPTURE_h__
#define __CASIO_CALC_SCREEN_CAPTURE_h__    1

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//-- scrCapture object - Screen capture using fxlink command -[ fxlink -iw ]
//--
class scrCapture{
public:
    // Construction
    scrCapture();

    // Destruction
    ~scrCapture();

    // Already installed ?
    bool isSet();

    // Install
    bool install();
    bool set(){
        return install();
    }

    // Remove
    void remove();
    void unset(){
        return remove();
    }

    //
    // Handle activity
    //

    // pause() : pause the capture
    //
    // @return : paused ?
    //
    bool pause();

    // resume() : resume the capture
    //
    // @return : paused ?
    //
    bool resume();


private:
    // Members
    bool    set_;   // Is the capture on ?
    bool    paused_;
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __CASIO_CALC_SCREEN_CAPTURE_h__

// EOF
