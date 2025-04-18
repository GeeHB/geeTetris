//----------------------------------------------------------------------
//--
//--    playArea.c
//--
//--            playArea functions
//--
//----------------------------------------------------------------------

#include "playArea.h"
#include "shared/casioCalcs.h"

#include <string.h>

#ifdef DEST_CASIO_CALC
extern font_t font_horz;
extern font_t font_vert;
#endif // #ifdef DEST_CASIO_CALC

//
// Internal functions
//

char* __strdrag(char *str, int rightChars);

//
// playArea
//
//  Text and rect. drawing in both modes (vertical et horizontal)
//

// playArea_init() : Intialize the playArea
//
//  @area : pointer to an area
//
//  @return : TRUE if successfull
//
BOOL playArea_init(PPLAYAREA area){
    if (area){
        area->orientation = CALC_VERTICAL;

    // Fonts
#ifdef DEST_CASIO_CALC
        area->hFont = &font_horz;
        area->vFont = &font_vert;
#endif // #ifdef DEST_CASIO_CALC

        // Theses keys won't change with screen rotation
        area->keys[KEY_FALL] = KEY_CODE_FALL;
        area->keys[KEY_PAUSE] = KEY_CODE_PAUSE;
        area->keys[KEY_ROTATE_DISPLAY] = KEY_CODE_ROTATE_DISPLAY;
        area->keys[KEY_QUIT] =  KEY_CODE_EXIT;

        return TRUE;
    }

    return FALSE;
}

// playArea_clear() : Clear | delete the playArea
//
//  @area : pointer to an area
//
void playArea_clear(PPLAYAREA area){
    if (area){
        // Return to default font
        playArea_defaultFont();
    }
}

// playArea_clearScreen() : Clear the screen with the given colour
//
//  @color : color to use to fill the screen
//
void playArea_clearScreen(int color){
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
    dclear((color_t)color);
#else
    dclear(color);
#endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC
}

// defaultFont() : return to default font
//
void playArea_defaultFont(){
#ifdef DEST_CASIO_CALC
    dfont(dfont_default());
#endif // #ifdef DEST_CASIO_CALC
}

// playArea_rotateDisplay() : Update members on rotation
//
//  @area : Pointer to the area
//  @orientation : New area orientation
//
void playArea_rotateDisplay(PPLAYAREA const area, CALC_ORIENTATION orientation){
    if (area){
        if (CALC_VERTICAL == (area->orientation = orientation)){
            area->zones[ZONE_PLAYFIELD].boxWidth = CASIO_BOX_WIDTH;

            area->zones[ZONE_PLAYFIELD].pos.x = CASIO_PLAYFIELD_LEFT +
                    CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            area->zones[ZONE_PLAYFIELD].pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            area->zones[ZONE_PLAYFIELD].pos.w = PLAYFIELD_WIDTH * area->zones[ZONE_PLAYFIELD].boxWidth
                    + 2 * CASIO_BORDER_GAP;
            area->zones[ZONE_PLAYFIELD].pos.h = PLAYFIELD_HEIGHT * area->zones[ZONE_PLAYFIELD].boxWidth
                    + 2 * CASIO_BORDER_GAP;

            // Nextpiece zone dims
            area->zones[ZONE_NEXTPIECE].boxWidth = CASIO_BOX_WIDTH_NP;
            area->zones[ZONE_NEXTPIECE].pos.x = CASIO_INFO_LEFT + CASIO_INFO_GAP;
            if (area->zones[ZONE_NEXTPIECE].pos.x <= (area->zones[ZONE_PLAYFIELD].pos.x + area->zones[ZONE_PLAYFIELD].pos.w)){
                area->zones[ZONE_NEXTPIECE].pos.x = area->zones[ZONE_PLAYFIELD].pos.x
                    + area->zones[ZONE_PLAYFIELD].pos.w + 2 * CASIO_INFO_GAP;
            }
            area->zones[ZONE_NEXTPIECE].pos.y = CASIO_INFO_TOP;
            area->zones[ZONE_NEXTPIECE].pos.w = area->zones[ZONE_NEXTPIECE].pos.h =
                    4 * area->zones[ZONE_NEXTPIECE].boxWidth + 2 * CASIO_INFO_GAP;

            // Keys
            area->keys[KEY_LEFT] = KEY_CODE_LEFT;
            area->keys[KEY_RIGHT] = KEY_CODE_RIGHT;
            area->keys[KEY_ROTATE] = KEY_CODE_UP;
            area->keys[KEY_DOWN] = KEY_CODE_DOWN;

            // Use "default" font
#ifdef DEST_CASIO_CALC
            dfont(hFont_);
#endif // #ifdef DEST_CASIO_CALC
        }
        else {
            // "rotated" mode
            //
            area->zones[ZONE_PLAYFIELD].boxWidth = CASIO_BOX_WIDTH_ROTATED;    // Larger box

            area->zones[ZONE_PLAYFIELD].pos.x = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            area->zones[ZONE_PLAYFIELD].pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            area->zones[ZONE_PLAYFIELD].pos.w = PLAYFIELD_WIDTH * area->zones[ZONE_PLAYFIELD].boxWidth + 2 * CASIO_BORDER_GAP;
            area->zones[ZONE_PLAYFIELD].pos.h = PLAYFIELD_HEIGHT * area->zones[ZONE_PLAYFIELD].boxWidth + 2 * CASIO_BORDER_GAP;

            // Nextpiece
            area->zones[ZONE_NEXTPIECE].boxWidth = CASIO_BOX_WIDTH_NP_ROTATED;  // box in preview is smaller
            area->zones[ZONE_NEXTPIECE].pos.w = area->zones[ZONE_NEXTPIECE].pos.h = 4 * area->zones[ZONE_NEXTPIECE].boxWidth + 2 * CASIO_INFO_GAP;
            area->zones[ZONE_NEXTPIECE].pos.x = area->zones[ZONE_PLAYFIELD].pos.x + area->zones[ZONE_PLAYFIELD].pos.w + 1;
            area->zones[ZONE_NEXTPIECE].pos.y = CASIO_INFO_TOP;

            // Keys
            area->keys[KEY_LEFT] = KEY_CODE_DOWN;
            area->keys[KEY_RIGHT] = KEY_CODE_UP;
            area->keys[KEY_ROTATE] = KEY_CODE_LEFT;
            area->keys[KEY_DOWN] = KEY_CODE_RIGHT;


            // Install my font
#ifdef DEST_CASIO_CALC
            dfont(vFont_);
#endif // #ifdef DEST_CASIO_CALC
        }

        // Values indicators
        area->texts[0].x = area->texts[1].x = area->texts[2].x = area->zones[ZONE_NEXTPIECE].pos.x;
        for (uint8_t id = 0; id <VAL_COUNT; id++){
            area->texts[id].y = area->zones[ZONE_NEXTPIECE].pos.y + area->zones[ZONE_NEXTPIECE].pos.w +
                CASIO_INFO_DY * id + 6;
        }
    }
}

// playArea_dtext() : Draw a line of text horizontally or vertically
//          according to the display orientation
//
//  @area : Pointer to the area
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea_dtext(PPLAYAREA const area, int x, int y, int fg, const char* text){
    if (area){
        if (area->orientation == CALC_HORIZONTAL){
            playArea_dtextV(area, x, y, fg, text);
        }
        else{
    #ifdef DEST_CASIO_CALC
            dtext(x, y, fg, text);
    #endif // #ifdef DEST_CASIO_CALC
        }
    }
}

// playArea_dtextV() : Draw a line of text vertically
//
// used when calc orientation is HORIZONTAL ...
//
//  @area : Pointer to the area
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea_dtextV(PPLAYAREA const area, int x, int y, int fg, const char* text){
    if (!area){
        return;
    }

    if (strlen(text) > 0){
        RECT pos;
        pos.x = x;
        pos.y = y;

        // dimensions of the first char.
        char* current = (char*)text;
        int w, h;
#ifdef DEST_CASIO_CALC
        dnsize(current, 1, font(false), &pos.w, &pos.hh);
#else
        w = h = 10; // for debug tests
#endif // #ifdef DEST_CASIO_CALC

        // Get new coordinates of the anchor
        rotateRect(&pos);

        // Draw the string (char. by char.)
        while (*current){
#ifdef DEST_CASIO_CALC
            dtext_opt(rect.x, rect.y,  fg, C_NONE, DTEXT_RIGHT, DTEXT_BOTTOM, current, 1);
            dnsize(current, 1, font(false), &w, &h);
#endif // #ifdef DEST_CASIO_CALC

            // Update anchor pos.
            pos.y-=h;

            // Next char
            current++;
        }
    }
}

// playArea_drawRectangle() : Draw a single coloured rectangle
//
//  @area : Pointer to the area
//  @x,@y : top left starting point
//  @width, @height : dimensions
//  @fillColour : Filling colour or NO_COLOR (-1) if none
//  @borderColour : Colour of the border or NO_COLOR (-1) if none
//
void playArea_drawRectangle(PPLAYAREA const area, uint16_t x, uint16_t y, uint16_t width,
    uint16_t height, int fillColour, [[maybe_unused]]  int borderColour){
    if (!area){
        return;
    }

    RECT pos = {.x = x, .y = y, .w = width, .h = height};

    // Horizontal display ?
    if (area->orientation == CALC_HORIZONTAL){
        rotateRect(&pos);
    }

    // Draw the rect
#ifdef DEST_CASIO_CALC
    int bColour= (NO_COLOR==borderColour?C_WHITE:borderColour);
    int xTo = pos.x + pos.w - 1;
    int yTo = pos.y + pos.h - 1;
    drect(pos.x, pos.y, xTo, yTo - 1, fillColour);
    dline(pos.x, pos.y, xTo, yTo, bColour);
    dline(xTo, pos.y, xTo, yTo, bColour);
#endif // #ifdef DEST_CASIO_CALC
}

// playArea_drawBorder() : Draw a border around a rectangle
//
//  @area : Pointer to the area
//  @x,@y : top left starting point
//  @width, @height : dimensions
//  @borderColour : Colour of the border
//
void playArea_drawBorder(PPLAYAREA const area, uint16_t x, uint16_t y,
        uint16_t width, uint16_t height, int borderColour){
    if (!area){
        return;
    }

    RECT pos = {.x = x, .y = y, .w = width, .h = height};

    // Horizontal display ?
    if (area->orientation == CALC_HORIZONTAL){
        rotateRect(&pos);
    }

#ifdef DEST_CASIO_CALC
    int16_t xTo = pos.x + pos.w - 1;
    int16_t yTo = pos.y + pos.h - 1;

    // Horizontal display ?
    if (isRotated()){
        rotate(xFrom, yFrom, xTo, yTo);
    }

    drect_border(pos.x, pos.y, xTo, yTo, C_NONE, 1, borderColour);
#endif // #ifdef FX9860G
}

// playArea_shitfToZone() : Change the origin and the coordinate system
//          according to a selected drawing zone
//
//  @area : Pointer to the area
//  @zoneID  : ID of the desintation zone
//  @x, @y [i/o] : coordinates to change
//  @width, @height : (new) dimensions in pixels of a single block in the choosen area
//
void playArea_shitfToZone(PPLAYAREA const area, uint8_t zoneID, uint16_t* x, uint16_t* y,
    uint16_t* width, uint16_t* height){
    if (area){
        if (zoneID == ZONE_GAME){
            *x = area->zones[ZONE_PLAYFIELD].pos.x + x * area->zones[ZONE_PLAYFIELD].boxWidth;
            *y = area->zones[ZONE_PLAYFIELD].pos.y + (PLAYFIELD_HEIGHT - 1 - y) * area->zones[ZONE_PLAYFIELD].boxWidth;
            *width = *height = area->zones[ZONE_PLAYFIELD].boxWidth;
        }
        else{
            // For next piece zone
            *x = area->zones[ZONE_NEXTPIECE].pos.x + CASIO_INFO_GAP;
            *y = area->zones[ZONE_NEXTPIECE].pos.y + CASIO_INFO_GAP;
            *width = *height = area->zones[ZONE_NEXTPIECE].boxWidth;
        }
    }
}

// playArea_valtoa() : Transform a numeric value into a string
//
//  This specific method creates a string composed of the name of the value
//  and the value it self. It is equivalent to a sprintf(out, "%s : %d", name, value)
//
//  The base can't be changed it is always equal to 10
//
//  This method assumes the output buffer - ie. str - is large enough to contain
//  the name and the formated value.
//
//  @num : Numeric value to transform
//  @name : Name of the value (can be NULL)
//  @str : Pointer to output string
//  @rLength : Align to right ?
//
//  @return : pointer to formated string
//
char* playArea_valtoa(int num, const char* name, char* str, size_t rLength){
    char* strVal = str;

    // Add name
    if (name){
        strcpy(str, name);
        strVal+=strlen(str);    // num. value starts here
    }

    // Append num. value
    int sum = (num < 0?-1*num:num);
    uint8_t i = 0, digit, dCount = 0;
    do{
        digit = sum % 10;
        strVal[i++] = '0' + digit;
        if (!(++dCount % 3)){
            strVal[i++] = ' ';  // for large numbers lisibility
        }

        sum /= 10;
    }while (sum);

    // A sign ?
    if (num < 0){
        strVal[i++] = '-';
    }
    strVal[i] = '\0';

    // Reverse the string (just the num. part)
    __strrev(strVal);

    // Shift to the right ?
    if (rLength){
        size_t len = strlen(str);
        if (rLength > len){
            __strdrag(strVal, rLength - len); // just drag the value
        }
    }

    return str;
}

// __strdrag() : Drag a string to the right
//
//  Drag the original string to the right.  Chars on the left will be fill
//  with spaces.
//
//  This function assumes str is large enough to complete successfully
//  with at least (strlen(str) + rightChars + 1) bytes
//
//  @str : String to slide
//  @rightChars : Count of chars str should be dragged to
//
//  @return : pointer to the string
//
char* __strdrag(char *str, int rightChars){
    size_t len, i;
    if (!str || 0 == (len = strlen(str)) || rightChars <= 0){
        return str;
    }

    str[len + rightChars] = '\0';  // New string size

    // Drag the string
    for (i=len; i; i--){
        str[i+rightChars - 1] = str[i-1];
    }

    // Put spaces on the left
    for (i=0; i<(size_t)rightChars; i++){
        str[i] = ' ';
    }

    // Finished
    return str;
}

// EOF
