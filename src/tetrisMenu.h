//----------------------------------------------------------------------
//--
//--    tetrisMenu.h
//--
//--        Definition of tetrisMenu
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_TETRIS_MENU_h__
#define __GEE_TETRIS_CASIO_TETRIS_MENU_h__    1



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

    // _inRange() : Ensure value is in a range
    //
    int8_t _inRange(int8_t val, uint8_t min, uint8_t max){
        return ((val < min)?min:((val > max)?max:val));
    }

    // Select a single value
    //
    void _selectValue(int8_t value, uint8_t min, uint8_t max,
                                int16_t  xPos, int16_t yPos,
                                bool select = true);

    // _createParamMenu() : Create sub-menu displayed on params' edition
    //
    //  @menu : Menu bar to modifiy
    //
    void _createParamMenu(menuBar& menu);

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
