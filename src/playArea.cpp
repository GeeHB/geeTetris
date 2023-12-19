//---------------------------------------------------------------------------
//--
//--	File	: playArea.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Implementation of playArea object
//--
//---------------------------------------------------------------------------

#include "playArea.h"

// Fonts
#ifdef DEST_CASIO_CALC
extern font_t font_horz;
extern font_t font_vert;
#endif // #ifdef DEST_CASIO_CALC

//---------------------------------------------------------------------------
//--
//-- playArea object
//--
//--    methods, constants (coordinates, dimensions) for the casio calculators
//--
//---------------------------------------------------------------------------

// Construction
//
playArea::playArea(){

    rotatedDisplay_ = false;

    // Fonts
#ifdef DEST_CASIO_CALC
    hFont_ = &font_horz;
    vFont_ = &font_vert;

    // Start the gray engine
#ifdef FX9860G
    #ifdef GRAY_ENGINE_ON
        dgray(DGRAY_ON);
    #endif // #ifdef GRAY_ENGINE_ON
#endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC

    // Default keys
    keyFall_ = KEY_CODE_FALL;
    keyPause_ = KEY_CODE_PAUSE;
    keyRotateDisplay_ = KEY_CODE_ROTATE_DISPLAY;
    keyQuit_ = KEY_CODE_EXIT;
}

// Destruction
//
playArea::~playArea(){
    // Return to default font
#ifdef DEST_CASIO_CALC
    dfont(dfont_default());

    // Stop the gray engine
    #ifdef FX9860G
        #ifdef GRAY_ENGINE_ON
            dgray(DGRAY_OFF);
        #endif // #ifdef GRAY_ENGINE_ON
    #endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC
}

// (trigonometric) rotations
//  ... of a single point
void playArea::rotate(int16_t& x, int16_t& y){
    int16_t ny(CASIO_HEIGHT - x);
    x = y;
    y = ny;
}

// ... of a rect
void playArea::rotate(int16_t& xFrom, int16_t& yFrom, int16_t& xTo, int16_t& yTo){
    rotate(xFrom, yFrom);
    rotate(xTo, yTo);

    // The rect (xFrom, yFrom) -> (xTo, yTo)
    // turns and becomes (xFrom, yTo) -> (xTo, yFrom)
    int16_t oFrom(yFrom);
    yFrom = yTo;
    yTo = oFrom;
}

// rotatedDisplay() : Update members on rotation
//
//  @doRotate : indicates wether display must rotate or not
//
void playArea::_rotatedDisplay(bool doRotate, bool force){
    if (force || rotatedDisplay_ != doRotate){
        if (false == (rotatedDisplay_ = doRotate)){
            playfield_.boxWidth = CASIO_BOX_WIDTH;

            playfield_.pos.x = CASIO_PLAYFIELD_LEFT + CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            playfield_.pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            playfield_.pos.w = PLAYFIELD_WIDTH * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;
            playfield_.pos.h = PLAYFIELD_HEIGHT * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;

            // Nextpiece zone dims
            nextPiece_.boxWidth = CASIO_BOX_WIDTH_NP;
            nextPiece_.pos.x = CASIO_INFO_LEFT + CASIO_INFO_GAP;
            if (nextPiece_.pos.x <= (playfield_.pos.x + playfield_.pos.w)){
                nextPiece_.pos.x = playfield_.pos.x + playfield_.pos.w + 2 * CASIO_INFO_GAP;
            }
            nextPiece_.pos.y = CASIO_INFO_TOP;
            nextPiece_.pos.w = nextPiece_.pos.h = 4 * nextPiece_.boxWidth + 2 * CASIO_INFO_GAP;

            // Keys
            keyLeft_ = KEY_CODE_LEFT;
            keyRight_ = KEY_CODE_RIGHT;
            keyRotatePiece_ = KEY_CODE_UP;
            keyDown_ = KEY_CODE_DOWN;

            // Use "default" font
#ifdef DEST_CASIO_CALC
            dfont(hFont_);
#endif // #ifdef DEST_CASIO_CALC
        }
        else {
            // "rotated" mode
            //
            playfield_.boxWidth = CASIO_BOX_WIDTH_ROTATED;    // Larger box

            playfield_.pos.x = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;
            playfield_.pos.y = CASIO_PLAYFIELD_BORDER + CASIO_BORDER_GAP;

            playfield_.pos.w = PLAYFIELD_WIDTH * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;
            playfield_.pos.h = PLAYFIELD_HEIGHT * playfield_.boxWidth + 2 * CASIO_BORDER_GAP;

            // Nextpiece
            nextPiece_.boxWidth = CASIO_BOX_WIDTH_NP_ROTATED;  // box in preview is smaller
            nextPiece_.pos.w = nextPiece_.pos.h = 4 * nextPiece_.boxWidth + 2 * CASIO_INFO_GAP;
            nextPiece_.pos.x = playfield_.pos.x + playfield_.pos.w + 1;
            nextPiece_.pos.y = CASIO_INFO_TOP;

            // Keys
            keyLeft_ = KEY_CODE_DOWN;
            keyRight_ = KEY_CODE_UP;
            keyRotatePiece_ = KEY_CODE_LEFT;
            keyDown_ = KEY_CODE_RIGHT;

            // Install my font
#ifdef DEST_CASIO_CALC
            dfont(vFont_);
#endif // #ifdef DEST_CASIO_CALC
        }

        // Values indicators
        textsPos_[0].x = textsPos_[1].x = textsPos_[2].x = nextPiece_.pos.x;
        for (uint8_t id(0); id <VAL_COUNT; id++){
            textsPos_[id].y = nextPiece_.pos.y + nextPiece_.pos.w +
                CASIO_INFO_DY * id + 6;
        }
    }
}

// dtext() : Draw a line of text horizontally or vertically
//          according to the display orientation
//
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea::dtext(int x, int y, int fg, const char* text){
    if (isRotated()){
        _dtextV(x, y, fg, text);
    }
    else{
#ifdef DEST_CASIO_CALC
        ::dtext(x, y, fg, text);
#endif // #ifdef DEST_CASIO_CALC
    }
}

// drawRectangle() : Draw a single coloured rectangle
//
//   @x,@y : top left starting point
//   @width, @height : dimensions
//   @fillColour : Filling colour or NO_COLOR (-1) if none
//   @borderColour : Colour of the border or NO_COLOR (-1) if none
//
void playArea::drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int fillColour, [[maybe_unused]]  int borderColour){
    int16_t xFrom(x), yFrom(y);
    int16_t xTo(xFrom + width - 1), yTo(yFrom + height - 1);

    // Horizontal display ?
    if (isRotated()){
        rotate(xFrom, yFrom, xTo, yTo);
    }

    // Draw the rect
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
    // Coloured rect
    drect(xFrom, yFrom, xTo - 1, yTo - 1, fillColour);

    // Half a border (bottom and right) for "small" pieces
    /*
    if (NO_COLOR != borderColour){
        dline(xFrom, yTo, xTo, yTo, borderColour);
        dline(xTo, yFrom, xTo, yTo, borderColour);
    }
    */
    int bColour(NO_COLOR==borderColour?C_WHITE:borderColour);
    dline(xFrom, yTo, xTo, yTo, bColour);
    dline(xTo, yFrom, xTo, yTo, bColour);
#else
    drect_border(xFrom, yFrom, xTo, yTo, fillColour, 1, borderColour);
#endif // #ifdef FX9860G
#endif // #ifdef DEST_CASIO_CALC
}

// drawBorder() : Draw a border around a rectangle
//
//   @x,@y : top left starting point
//   @width, @height : dimensions
//   @borderColour : Colour of the border
//
void playArea::drawBorder(uint16_t x, uint16_t y, uint16_t width, uint16_t height, int borderColour){
    // Draw the rect
#ifdef DEST_CASIO_CALC
#ifdef FX9860G
    int16_t xFrom(x), yFrom(y);
    int16_t xTo(xFrom + width - 1), yTo(yFrom + height - 1);

    // Horizontal display ?
    if (isRotated()){
        rotate(xFrom, yFrom, xTo, yTo);
    }

    drect_border(xFrom, yFrom, xTo, yTo, C_NONE, 1, borderColour);
#else
    drawRectangle(x, y, width, height, C_NONE, borderColour);
#endif // #ifdef FX9860G
#endif // #ifdef FX9860G
}

// _shitfToZone() : Change the origin and the coordinate system
//          according to a selected drawing zone
//
//  @zoneID  : ID of the desintation zone
//  @x, @y [i/o] : coordinates to change
//  @width, @height : (new) dimensions in pixels of a single block in the choosen area
//
void playArea::shitfToZone(uint8_t zoneID, uint16_t& x, uint16_t& y, uint16_t& width, uint16_t& height) {
    if (zoneID == ZONE_GAME){
        x = playfield_.pos.x + x * playfield_.boxWidth;
        y = playfield_.pos.y + (PLAYFIELD_HEIGHT - 1 - y) * playfield_.boxWidth;
        width = height = playfield_.boxWidth;
    }
    else{
        // For next piece zone
        x = nextPiece_.pos.x + CASIO_INFO_GAP;
        y = nextPiece_.pos.y + CASIO_INFO_GAP;
        width = height = nextPiece_.boxWidth;
    }
}

// _dtextV() : Draw a line of text vertically
//
//  @x, @y : Anchor point coordinates
//  @fg : font colour
//  @text : string to draw
//
void playArea::_dtextV(int x, int y, int fg, const char* text){
    if (strlen(text) > 0){
        int16_t xFrom(x), yFrom(y), xTo, yTo;

        // dimensions of the first char.
        char* current = (char*)text;
        int w, h;
#ifdef DEST_CASIO_CALC
        dnsize(current, 1, font(false), &w, &h);
#else
        w = h = 10; // for debug tests
#endif // #ifdef DEST_CASIO_CALC

        // Get new coordinates of the anchor
        xTo = xFrom + w;
        yTo = yFrom + h;
        rotate(xFrom, yFrom, xTo, yTo);

        // Draw the string (char. by char.)
        while (*current){
#ifdef DEST_CASIO_CALC
            dtext_opt(xFrom, yFrom,  fg, C_NONE, DTEXT_RIGHT, DTEXT_BOTTOM, current, 1);
            dnsize(current, 1, font(false), &w, &h);
#endif // #ifdef DEST_CASIO_CALC

            // Update anchor pos.
            yFrom-=h;

            // Next char
            current++;
        }
    }
}

// __valtoa() : Transform a numeric value into a string
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
char* playArea::__valtoa(int num, const char* name, char* str, size_t rLength){
    char* strVal(str);

    // Add name
	if (name){
	    strcpy(str, name);
	    strVal+=strlen(str);    // num. value starts here
	}

	// Append num. value
	int sum ((num < 0)?-1*num:num);
	uint8_t i(0), digit, dCount(0);
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
		size_t len(strlen(str));
		if (rLength > len){
		    __strdrag(strVal, rLength - len); // just drag the value
        }
	}

	return str;
}

// __strrev() : Reverse a string
//
//  @str : String to reverse
//
void playArea::__strrev(char *str){
	int i, j;
	unsigned char a;
	size_t len = strlen((const char *)str);
	for (i = 0, j = len - 1; i < j; i++, j--){
		a = str[i];
		str[i] = str[j];
		str[j] = a;
	}
}

// __strdrag() : Drag a string to the right
//
//	Drag the original string to the right.  Chars on the left will be fill
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
char* playArea::__strdrag(char *str, int rightChars){
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
