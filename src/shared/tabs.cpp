//---------------------------------------------------------------------------
//--
//--	File	: tabs.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	:
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--        Implementation of objects used for tab managment :
//--            - tabManager : tools that manages a list à tabs
//--            - tab : A basic tab
//--            - tabValue : A tab holding a value
//--            - tabRangedValue : A tab handling a numeric value in a range
//--
//--            - tabKeyboard : An object for handling keyboard events
//--
//---------------------------------------------------------------------------

#include "tabs.h"

//---------------------------------------------------------------------------
//--
//--
//--
//---------------------------------------------------------------------------

// Fonts
#ifdef DEST_CASIO_CALC
extern font_t font_horz;
#endif // #ifdef DEST_CASIO_CALC

//---------------------------------------------------------------------------
//--
//-- Constants
//--
//---------------------------------------------------------------------------

#ifdef FX9860G
// tabs colors
#define COLOUR_SELECTED     C_WHITE
#define COLOUR_UNSELECTED   C_BLACK

#define COLOUR_BK_SELECTED     C_BLACK
#define COLOUR_BK_UNSELECTED   C_WHITE

// Ranged values colors - selected
#define COLOUR_VAL_TXT_SEL   COLOUR_WHITE
#define COLOUR_VAL_BK_SEL    COLOUR_BLACK

#define TABRANGED_COMMENT_Y_OFFESET 15

#else
// tabs colors
#define COLOUR_SELECTED     COLOUR_BLUE
#define COLOUR_UNSELECTED   COLOUR_GREY

#define COLOUR_BK_SELECTED     C_WHITE
#define COLOUR_BK_UNSELECTED   C_WHITE


// Ranged values colors - selected
#define COLOUR_VAL_TXT_SEL   COLOUR_WHITE
#define COLOUR_VAL_BK_SEL    COLOUR_LT_BLUE

#define TABRANGED_COMMENT_Y_OFFESET     25

#endif // #ifdef FX9860G

// Ranged values colors - unselected
#define COLOUR_VAL_TXT_UNSEL COLOUR_BLACK
#define COLOUR_VAL_BK_UNSEL  COLOUR_WHITE

//---------------------------------------------------------------------------
//--
//-- tabKeyboard object - A keyboard handler fot tabs and tab management
//--
//--------------------------------------------------------------------------

// Key event in the queue
//
uint tabKeyboard::getKey(){
    uint key(key_);

    if (KEY_CODE_NONE == key){
        key = keyboard::getKey();
    }
    else{
        key_ = KEY_CODE_NONE;   // Next time read the keyboard
    }

    // return the pressed key code
    return key;
}

//---------------------------------------------------------------------------
//--
//-- tab object - A simple basic tab - abstract class
//--
//---------------------------------------------------------------------------

// Construction
//
tab::tab(const char* tname, int action){

    action_ = action;
    selected_ = false;

    // Copy (and truncate) name
    size_t len(strlen(tname));
    if (len>TAB_NAME_LEN){
        len = TAB_NAME_LEN;
    }
    strncpy(name_, tname, len);
    name_[len] = 0;
}

// Dimensions & position
//
void tab::getRect(RECT& position){
    position.x = rect_.x;
    position.y = rect_.y;
    position.w = rect_.w;
    position.h = rect_.h;
}

void tab::setRect(RECT& rect){
    rect_.x =  rect.x;
    rect_. y = rect.y;
    rect_.w = rect.w;
    rect_.h = rect.h;
}

// Draw a single tab
//
void tab::draw(const RECT* anchor, bool selected, const char* name){
#ifdef DEST_CASIO_CALC
    // Draw back ground
    drect(anchor->x, anchor->y, anchor->x + anchor->w - 1, anchor->y + anchor->h - 1, selected?COLOUR_BK_SELECTED:COLOUR_BK_UNSELECTED);

    // Text
    if (name){
        int x,y, w, h;
        dsize(name, NULL, &w, &h);

        // center text
        x = anchor->x + (anchor->w - w) / 2;
        y = anchor->y + (anchor->h - h) / 2;

        // text too large ?

        // draw the text
        dtext(x, y, selected?COLOUR_SELECTED:COLOUR_UNSELECTED, name);
    }

    // frame
    if (selected){
        dline(anchor->x, anchor->y, anchor->x, anchor->y + anchor->h - 2, COLOUR_BLACK); // Left
        dline(anchor->x+1, anchor->y + anchor->h - 1, anchor->x + anchor->w -1 - TAB_ROUNDED_DIM, anchor->y + anchor->h - 1, COLOUR_BLACK);  // bottom
        dline(anchor->x + anchor->w -1 - TAB_ROUNDED_DIM, anchor->y + anchor->h - 1, anchor->x + anchor->w - 1, anchor->y + anchor->h - 1 - TAB_ROUNDED_DIM, COLOUR_BLACK);  // bottom
        dline(anchor->x + anchor->w - 1, anchor->y, anchor->x + anchor->w - 1, anchor->y + anchor->h - 1 - TAB_ROUNDED_DIM, COLOUR_BLACK);   // right
    }
    else{
        dline(anchor->x, anchor->y, anchor->x + anchor->w -1, anchor->y, COLOUR_BLACK);
    }
#endif // #ifdef DEST_CASIO_CALC
}

//
// tabValue: a tab holding a value (a parameter)
//

// Construction
//
tabValue::tabValue(const char* tname, int* value, int action)
:tab(tname, action){
    value_ = value;
    comment_ = ucomment_ = NULL;
}

// Set or change comment
//
void tabValue::setComment(const char* comment, const char* ucomment){
    comment_ = _dup(comment_, comment);
    ucomment_ = _dup(ucomment_, ucomment);
}

// The current tab is selected (and can take control of the keyboard)
//
uint8_t tabValue::select(){

    // Avoid to change the value when F key is first pressed
    if (isSelected()){
        // Change the value
        (*value_) = ! (*value_);
    }

    // update comment
    clearScreen();

#ifdef DEST_CASIO_CALC
    if (comment_){
        dtext(TAB_RANGE_COMMENT_X, int((CASIO_HEIGHT - TAB_HEIGHT) / 2), COLOUR_BLACK, (bool)(*value_)?comment_:(ucomment_?ucomment_:comment_));
    }

    dupdate();
#endif // #ifdef DEST_CASIO_CALC

    // Nothing special to do
    return tab::select();
}

// Duplicate a comment
//
char* tabValue::_dup(char* source, const char* val){
    // Free current string
    if (source){
        free(source);
    }

    // Duplicate comment
    return (val?strdup(val):NULL);
}

//
// tabRangedValue: A tab with a value in a range
//

// Construction
tabRangedValue::tabRangedValue(const char* tname, int* val, uint8_t minVal, uint8_t maxVal, const tabKeyboard* keys)
:tabValue(tname, val, ACTION_NONE){
    setRange(minVal, maxVal);

    // A keyboard ?
    if (keys){
        keys_ = (tabKeyboard*)keys;
        ownKeyboard_ = false;
    }
    else{
        keys_ = new tabKeyboard();  // Create my own keyboard
        ownKeyboard_ = (keys_ != nullptr);
    }
}

// Destruction
tabRangedValue::~tabRangedValue(){
    // free keyboard if owned by object
    if (keys_ && ownKeyboard_){
        delete keys_;
    }
}

// Range
//
void tabRangedValue::setRange(uint8_t minVal, uint8_t maxVal){
    // Set new values
    if (minVal < maxVal){
        minVal_ = minVal;
        maxVal_ = maxVal;
    }
    else{
        minVal_ = maxVal;
        maxVal_ = (minVal==maxVal?maxVal+1:minVal);
    }

    // Position (centered)
    xPos_ = (CASIO_WIDTH - TAB_RANGE_BOX_WIDTH * (maxVal_ - minVal_ + 1)) / 2;
    yPos_ = int((CASIO_HEIGHT - TAB_HEIGHT) / 2);

    // Current value must be in the range
    (*value_) = (uint8_t)_inRange((int8_t)(*value_));
}

// Change the value
//
uint8_t tabRangedValue::select(){

    // Ensure a keyboard is present
    if (!keys_){
        return ACTION_NONE;
    }

    tab::select();
    clearScreen();

    // Draw all possible numbers
    _drawRange();

    // Select current val
    int key(0);
    int8_t oldVal(-1), newVal((int8_t)(*value_));
    bool stay(true);
    _selectValue(newVal);

#ifdef DEST_CASIO_CALC
    dupdate();
#endif // #ifdef DEST_CASIO_CALC

    // Value may change using the keyboard
    do{
        key = keys_->getKey();

        // Exit on "Exit" or F{n} key pressed
        if ((key >= KEY_CODE_F1 && key <= KEY_CODE_F6) || key == KEY_CODE_EXIT){
            // Exit and pass key code to the tabmanager
            stay = false;
            //keys_->addKey(key);
        }
        else{
            // Change selection
            if (key == KEY_CODE_LEFT){
                newVal = _inRange(--newVal);
            }
            else{
                if (key == KEY_CODE_RIGHT){
                    newVal = _inRange(++newVal);
                }
            }

            // Any change ?
            if (newVal != oldVal){
                _selectValue(oldVal, false);
                _selectValue(newVal);

#ifdef DEST_CASIO_CALC
                dupdate();
#endif // #ifdef DEST_CASIO_CALC

                oldVal = newVal;
            }
        }
    }
    while (stay);

    // "return" key pressed (or 0 if none)
    keys_->addKey(key);
    return ACTION_NONE;
}

// Draw the range
void tabRangedValue::_drawRange(){
    uint8_t max(maxVal_ - minVal_);
    uint16_t x(xPos_);

    for (uint8_t index=0; index<=max; index++){
#ifdef DEST_CASIO_CALC
        drect_border(x, yPos_, x + TAB_RANGE_BOX_WIDTH, yPos_ + TAB_RANGE_BOX_WIDTH, NO_COLOR, 1, COLOUR_BLACK);
        dprint(x + 5, yPos_ + 3, COLOUR_BLACK, "%d", (index+minVal_));
#endif // #ifdef DEST_CASIO_CALC
        x+=TAB_RANGE_BOX_WIDTH;
    }

#ifdef DEST_CASIO_CALC
    if (comment_){
        dtext(TAB_RANGE_COMMENT_X,
            yPos_ - TABRANGED_COMMENT_Y_OFFESET,
            COLOUR_BLACK, comment_);
    }

    dupdate();
#endif // #ifdef DEST_CASIO_CALC
}

// Select a single value
//
void tabRangedValue::_selectValue(int8_t value, bool select){
    if (value >= minVal_ && value <= maxVal_){
        uint16_t x(xPos_ + (value - minVal_) * TAB_RANGE_BOX_WIDTH);

#ifdef DEST_CASIO_CALC
        drect(x + 1, yPos_ + 1 , x + TAB_RANGE_BOX_WIDTH - 1, yPos_ + TAB_RANGE_BOX_WIDTH - 1 ,
                select?COLOUR_VAL_BK_SEL:COLOUR_VAL_BK_UNSEL);
        dprint(x + 5, yPos_ + 3, select?COLOUR_VAL_TXT_SEL:COLOUR_VAL_TXT_UNSEL, "%d", value);
#else
        x++;    // for compiler
#endif // #ifdef DEST_CASIO_CALC
    }

    // Update the value (if selected)
    if (select){
        *value_ = (uint8_t)value;
    }
}

//
// tabManager - Manages the 6 possibles tabs
//

// Constructor
//
tabManager::tabManager(){
    // Initialize members
    memset(tabs_, 0x00, sizeof(tabs_));
    active_ = -1;

#ifdef FX9860G
    dfont((font_t*)&font_horz);
#endif // #ifdef FX9860G
}

// Add a tab
//
bool tabManager::add(tab* ptab, int8_t ID){
    if (!ptab || ID >= TAB_COUNT || -1 != _find(ptab)){
        return false;
    }

    int8_t index((ID==-1)?_findFreeID():ID);
    if (-1 == index){
        return false;   // no free place
    }

    // Add the new tab
    tabs_[index] = ptab;

    // set its position
    RECT pos;
    _ID2Rect(index, pos);
    ptab->setRect(pos);

    // Automatically select the first added tab.
    if (-1 == active_){
        select(index);
    }

    // Ok
    update();
    return true;
}

// Set active tab
//
tab* tabManager::select(int8_t ID){
    // Valid and different
    if (ID != active_ && ID < TAB_COUNT && tabs_[ID]){
        // Unselect
        _select(active_, false);

        // Select
        _select(ID, true);

        active_ = ID;

#ifdef DEST_CASIO_CALC
        dupdate();
#endif // DEST_CASIO_CALC

        return tabs_[ID];
    }

    // Not a valid tab ?
    return (ID == active_ && active_ != -1?tabs_[ID]:NULL);
}

// Redraw all tabs
//
void tabManager::update(){
    tab* ptab(NULL);
    RECT anchor;
    for (uint8_t index(0); index < TAB_COUNT; index++){
        ptab = tabs_[index];
        if (ptab){
            ptab->draw(index == active_);
        }
        else{
            _ID2Rect(index, anchor);
            tab::draw(&anchor, index==active_);
        }
    }
#ifdef DEST_CASIO_CALC
    dupdate();
#endif // DEST_CASIO_CALC
}

// Set a tab position
//
void tabManager::_ID2Rect(uint8_t ID, RECT& rect){
    if (ID < TAB_COUNT){
        rect.x = ID * TAB_WIDTH;
        rect. y = CASIO_HEIGHT - TAB_HEIGHT;
        rect.w = TAB_WIDTH;
        rect.h = TAB_HEIGHT;
    }
}

// Search the current tab.
//
int8_t tabManager::_find(const tab* ptab){
    if (!ptab){
        return -1;
    }

    for (uint index(0); index < TAB_COUNT; index++){
        if (tabs_[index] == ptab){
            return index;   // found
        }
    }

    // Not found
    return -1;
}

// Search first free place
//
int8_t tabManager::_findFreeID(){
    uint index(0);
    while (tabs_[index] && index < TAB_COUNT){
        index++;
    }

    // ID of first free place or -1
    return (index>=TAB_COUNT)?-1:index;
}

// (de)activate a tab
//
void tabManager::_select(int8_t ID, bool activate){
    if (-1 != ID){
        tab* ptab = tabs_[ID];

        if (ptab){
            // Change selection status
            if (!activate)/*{
                ptab->select();     // No need to preselect, tab::select(status) will soon be called !
            }
            else*/{
                ptab->unSelect();   // Tell tab it's unselected
            }

            ptab->draw(activate);
        }
        else{
            RECT rect;
            _ID2Rect(ID, rect);
            tab::draw(&rect, activate);
        }
    }
}

// EOF
