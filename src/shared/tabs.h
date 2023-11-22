//---------------------------------------------------------------------------
//--
//--	File	: tabs.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--        Definition of objects used for tab managment :
//--            - tabManager : tools that manages a list à tabs
//--            - tab : A basic tab
//--            - tabValue : A tab holding a value
//--            - tabRangedValue : A tab handling a numeric value in a range
//--
//--            - tabKeyboard : An object for handling keyboard events
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TETRIS_CASIO_TABS_h__
#define __GEE_TETRIS_CASIO_TABS_h__    1

#include "../playArea.h"
#include "keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//---------------------------------------------------------------------------
//--
//-- Constants
//--
//---------------------------------------------------------------------------

#define TAB_COUNT       6

// Dimensions in pixels
#define TAB_HEIGHT      22
#define TAB_WIDTH       (CASIO_WIDTH / TAB_COUNT)

#define TAB_ROUNDED_DIM  4
#define TAB_NAME_LEN    10      // max char var the tab name

#define TAB_RANGE_BOX_WIDTH     15      // width of a single box in pixels

#define TAB_RANGE_COMMENT_X     10

// Actions
//
enum TAB_ACTIONS{
    ACTION_NONE	 = 0,
    ACTION_REDRAW_TABS = 1,
    ACTION_QUIT = 2            // The app should quit
    // Other possible actions ???
};

//---------------------------------------------------------------------------
//--
//-- tabKeyboard object - A keyboard handler for tabs and tab manager
//--
//--------------------------------------------------------------------------

class tabKeyboard : public keyboard{
public:

    // Construction
    tabKeyboard():
    keyboard(){
        key_ = KEY_CODE_NONE;
    }

    // Destruction
    virtual ~tabKeyboard(){}

    // getKey() : Key event in the queue
    //
    //  @return : Key code or KEY_NONE (0) if no key pressed
    //
    virtual uint getKey();

    // addkey() : Add a key event
    //
    //  Add a key in the queue.
    //  The key code is not formelly added. But the code will
    //  be returned by the next call to getKey() method
    //
    //  @code : key code to add in the queue
    //  @mod : modifier value
    //
    void addKey(uint code, uint mod = MOD_NONE){
        key_ = code;
        mod_ = mod;
    }

private:
    uint key_;   // "injected" key
};

//---------------------------------------------------------------------------
//--
//-- tab object - base class for tab objects
//--
//---------------------------------------------------------------------------

class tab{
public:
    // Construction
    tab(const char* tname, int action = ACTION_NONE);

    // Destruction
    ~tab(){}

    // The current tab is selected (and can take control of the keyboard)
    // this method could/should be overloaded
    virtual uint8_t select(){
        selected_ = true;
        return action_;
    }

    // The current tab is unselected
    virtual void unSelect(){
        selected_ = false;
    }

    bool isSelected(){
        return selected_;
    }

    // Draw
     void draw(bool selected = false){
        draw(&rect_, selected, name_);
    }

    // Access
    const char* name(){
        return name_;
    }

    // Dimensions & position
    void getRect(RECT& position);
    void setRect(RECT& position);

    // static methods
    //

    // Draw a single tab
    static void draw(const RECT* position, bool selected, const char* name = NULL);

    // clear the whole screen (except tab lane)
    static void clearScreen(){
#ifdef DEST_CASIO_CALC
        drect(0, 0, CASIO_WIDTH - 1, CASIO_HEIGHT - TAB_HEIGHT - 1, C_WHITE);
#endif // #ifdef DEST_CASIO_CALC
    }

protected:
    // Members
    char        name_[TAB_NAME_LEN + 1];
    bool        selected_;
    RECT        rect_;
    int         action_;        // What todo when pressed ?
};

//---------------------------------------------------------------------------
//--
//-- tabValue object : A tab with a value / parameter
//--            by default the tabValue works as a 2 state button to
//--            set ou unset the internal boolean value
//--
//---------------------------------------------------------------------------

class tabValue : public tab{
public:
    // Construction
    tabValue(const char* tname, int* value, int action = ACTION_NONE);

    // Destruction
    virtual ~tabValue(){
        setComment(NULL, NULL);   // Free the resources
    }

    /*
    // Value
    void setValue(TAB_VALUE& val){
        // Any ...
        value_.iVal = val.iVal;
    }
    void value(TAB_VALUE& val){
        val.iVal = value_.iVal;
    }
    */

    // Comment
    void setComment(const char* comment, const char* ucomment = NULL);

    // The current tab is selected (and can take control of the keyboard)
    uint8_t select();

protected :
    char* _dup(char* source, const char* value);

protected:
    // Members
    char    *comment_, *ucomment_;
    int* value_;
};

//---------------------------------------------------------------------------
//--
//-- tabRangedValue object : A tab with a value in a range
//--
//---------------------------------------------------------------------------

class tabRangedValue : public tabValue{
public:
    // Construction
    tabRangedValue(const char* tname, int* val, uint8_t minVal, uint8_t maxVal, const tabKeyboard* keys = NULL);

    // Destruction
    ~tabRangedValue();

    // Range
    void setRange(uint8_t minVal, uint8_t maxVal);

    // The current tab is selected (and can take control of the keyboard)
    uint8_t select();

private:
    // Ensure value is in the range
    int8_t _inRange(int8_t val){
        return ((val < minVal_)?minVal_:((val > maxVal_)?maxVal_:val));
    }

    // Draw the whole range
    void _drawRange();

    // Select a single value
    void _selectValue(int8_t value, bool select = true);

protected:
    uint8_t minVal_, maxVal_;   // Range
    uint16_t xPos_, yPos_;      // Origin for range

    tabKeyboard* keys_;            // Keyboard handler
    bool      ownKeyboard_;
};

//
// tabManager - Manages the 6 possibles tabs
//
class tabManager{
public:
    // Constructor
    tabManager();

    // Destructor
    ~tabManager(){}

    // Add a tab
    bool add(tab* ptab, int8_t ID = -1);

    // Set active tab
    tab* select(int8_t ID);
    int8_t activeTab(){
        return active_;
    }

    // (Re)draw all tabs
    void update();

    // Private methods
    //
private:
    // Set a tab position
    void _ID2Rect(uint8_t ID, RECT& rect);

    // Search in the tab list
    int8_t _find(const tab* ptab);
    int8_t _findFreeID();

    // (de)activate a tab
    void _select(int8_t ID, bool activate);

    // Members
    tab*    tabs_[TAB_COUNT];
    int8_t  active_;    // ID of active tab
};

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_CASIO_TABS_h__

// EOF
