//----------------------------------------------------------------------
//--
//--    tetrisGame.cpp
//--
//--
//--        Implementation of tetrisGame object
//--
//----------------------------------------------------------------------

#include "tetrisGame.h"
#include "consts.h"
#include "piece.h"
#include "playArea.h"
#include "shared/casioCalcs.h"

#include <math.h>
#include <time.h>
#include <string.h>

#ifdef DEST_CASIO_CALC
extern bopti_image_t g_imgPause;
#endif // #ifdef DEST_CASIO_CALC

//
// tetrisGame
//
//  Handle the gameplay and the game(without display !)
//

//
// Internal functions
//

void _cancel(PTETRISGAME const tetris);

void _addDirtyLine(PTETRISGAME const tetris, uint8_t lineID);

void _redraw(PTETRISGAME const tetris);
void _drawSinglePiece(PTETRISGAME const tetris, uint8_t* datas, uint16_t cornerX,
        uint16_t cornerY, BOOL inTetrisGame, uint8_t specialColourID);
void _drawBackGround(PTETRISGAME const tetris);
void _drawNextPiece(PTETRISGAME const tetris);
void _drawTetrisGame(PTETRISGAME const tetris);
void _drawNumValue(PTETRISGAME const tetris, uint8_t index);
void _eraseNextPiece(PTETRISGAME const tetris);

void _rotateDisplay(PTETRISGAME const tetris, BOOL start);
BOOL _canMove(PTETRISGAME const tetris, int8_t leftPos, uint8_t  topPos);
uint8_t _minTopPosition(PTETRISGAME const tetris);

void _handleGameKeys(PTETRISGAME const tetris);
BOOL _left(PTETRISGAME const tetris);
BOOL _right(PTETRISGAME const tetris);
BOOL _down(PTETRISGAME const tetris, BOOL newPiece);
void _fall(PTETRISGAME const tetris);
BOOL _rotateLeft(PTETRISGAME const tetris);

void _piecePosChanged(PTETRISGAME const tetris);
void _reachLowerPos(PTETRISGAME const tetris, uint8_t downRowcount);
void _clearLine(PTETRISGAME const tetris, uint8_t index);
void _newPiece(PTETRISGAME const tetris);
void _putPiece(PTETRISGAME const tetris);


uint8_t* _nextPieceDatas(PTETRISGAME const tetris);
uint8_t* _pieceDatas(PTETRISGAME const tetris, int8_t index, uint8_t  rotIndex);

int _getSpeed(int currentTicks, uint8_t incLevel);

#define _newPieceIndex() (rand() % TETRAMINOS_COUNT)


//
// Public functions
//

// tetrisGame_init() : Initialize a new game
//
//  @tetris : pointer to a tetrisGame struct.
//  params : pointer to the game's parameters
//
//  @return : TRUE if init done
//
BOOL tetrisGame_init(PTETRISGAME const tetris, PPARAMS const params){
    if (tetris){
        // Initialize rand num. generator
        srand((unsigned int)clock());

        // Set indicators name
        strcpy(tetris->values[SCORE_ID].name, SCORE_STR);
        strcpy(tetris->values[LEVEL_ID].name, LEVEL_STR);
        strcpy(tetris->values[COMPLETED_LINES_ID].name, COMPLETED_LINES_STR);

        // Build the tetraminos'list
        //

        // S
        piece_init(&tetris->tetraminos[0]);
        piece_addRotation(&tetris->tetraminos[0], S_0);
        piece_addRotation(&tetris->tetraminos[0], S_1);

        // Z
        piece_init(&tetris->tetraminos[1]);
        piece_addRotation(&tetris->tetraminos[1], Z_0);
        piece_addRotation(&tetris->tetraminos[1],Z_1);

        // I
        piece_init(&tetris->tetraminos[2]);
        piece_addRotation(&tetris->tetraminos[2], I_0);
        piece_addRotation(&tetris->tetraminos[2], I_1);

        // O
        piece_init(&tetris->tetraminos[3]);
        piece_addRotation(&tetris->tetraminos[3], O_0);

        // L
        piece_init(&tetris->tetraminos[4]);
        piece_addRotation(&tetris->tetraminos[4], L_0);
        piece_addRotation(&tetris->tetraminos[4], L_1);
        piece_addRotation(&tetris->tetraminos[4], L_2);
        piece_addRotation(&tetris->tetraminos[4], L_3);

        // J
        piece_init(&tetris->tetraminos[5]);
        piece_addRotation(&tetris->tetraminos[5], J_0);
        piece_addRotation(&tetris->tetraminos[5], J_1);
        piece_addRotation(&tetris->tetraminos[5], J_2);
        piece_addRotation(&tetris->tetraminos[5], J_3);

        // T
        piece_init(&tetris->tetraminos[6]);
        piece_addRotation(&tetris->tetraminos[6], T_0);
        piece_addRotation(&tetris->tetraminos[6], T_1);
        piece_addRotation(&tetris->tetraminos[6], T_2);
        piece_addRotation(&tetris->tetraminos[6], T_3);

        // Colours
        //
        tetris->colours[COLOUR_ID_BOARD] = COLOUR_WHITE;
        tetris->colours[1] = COLOUR_RED;           // Pieces (1 to 7)
        tetris->colours[2] = COLOUR_GREEN;
        tetris->colours[3] = COLOUR_YELLOW;
        tetris->colours[4] = COLOUR_BLUE;
        tetris->colours[5] = COLOUR_PURPLE;
        tetris->colours[6] = COLOUR_CYAN;
        tetris->colours[7] = COLOUR_ORANGE;
        tetris->colours[COLOUR_ID_SHADOW] = COLOUR_LT_GREY;
        tetris->colours[COLOUR_ID_TEXT] = COLOUR_BLACK;
        tetris->colours[COLOUR_ID_BORDER] = COLOUR_DK_GREY;
        tetris->colours[COLOUR_ID_BKGRND] = COLOUR_WHITE;  // could be different from board !

        tetrisGame_setParameters(tetris, params);

        return TRUE;
    }

    return FALSE;
}

// tetrisGame_setParameters() : Set game's parameters
//
//  @tetris : pointer to a tetrisGame struct.
//  @params : Struct. containining parameters for the game
//    These parameters are choosen by the user
//
void tetrisGame_setParameters(PTETRISGAME const tetris, PPARAMS const params){
    if (tetris && params){
        params_copy(&tetris->parameters, params);

        tetris->values[SCORE_ID].value = 0;
        tetris->values[LEVEL_ID].value = tetris->parameters.startLevel;
        tetris->values[COMPLETED_LINES_ID].value = 0;
        tetris->nextIndex = -1;

        // Initialization of tetraminos(no rotation)
        for (uint8_t index = 0; index < TETRAMINOS_COUNT; index++) {
            tetris->tetraminos[index].rotateID = 0;
        }

        //  Screen rotation
        playArea_rotateDisplay(&tetris->casioDisplay, params->orientation);

        // Empty the playset
        memset(tetris->playField, COLOUR_ID_BOARD, PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH);

        // Add dirty lines ...
        uint8_t maxLines = PLAYFIELD_HEIGHT - PIECE_HEIGHT - 1;
        if (tetris->parameters.dirtyLines > maxLines) {
            tetris->parameters.dirtyLines = maxLines;
        }
        for (uint8_t index = 0; index < params->dirtyLines; index++) {
            _addDirtyLine(tetris, index);
        }

        // Ready for the game
        tetris->status = STATUS_READY;
    }
}

// pause() : Pause or resume the game
//
//  @tetris : Pointer to atretrisGame struct.
//
#ifdef DEST_CASIO_CALC
void tetrisGame_pause(PTETRISGAME const tetris){
    if (tetris){
        char car = KEY_NONE;
        BOOL paused = TRUE;

        // draw the picture
        //

        // Top of image
        dsubimage(0, 0, &g_imgPause,
                0, 0, IMG_PAUSE_W, IMG_PAUSE_COPY_Y, DIMAGE_NOCLIP);

        // "middle"
        uint16_t y;
        for (y = IMG_PAUSE_COPY_Y;
            y < (IMG_PAUSE_COPY_Y + IMG_PAUSE_LINES); y++){
            dsubimage(0, y, &g_imgPause,
                0, IMG_PAUSE_COPY_Y,
                IMG_PAUSE_W, 1, DIMAGE_NOCLIP);
        }

        // bottom
        y = CASIO_HEIGHT - IMG_PAUSE_H + IMG_PAUSE_COPY_Y - 1;
        dsubimage(0, y, &g_imgPause,
                0, IMG_PAUSE_COPY_Y + 1,
                IMG_PAUSE_W, IMG_PAUSE_H - IMG_PAUSE_COPY_Y - 1,
                DIMAGE_NOCLIP);

        dupdate();

        // status_ = STATUS_PAUSED;
        do{
            car = getKey();

            // Resume ?
            if (tetris->casioDisplay.keys[KEY_ID_PAUSE] == car){
                _redraw(tetris);
                _drawNextPiece(tetris);
                dupdate();
                paused = FALSE;
            }
            else{
                // Exit ?
                if (tetris->casioDisplay.keys[KEY_ID_QUIT] == car){
                    _cancel(tetris);
                    paused = FALSE;
                }
            }

        }while (paused);
    }
}
#endif // #ifdef DEST_CASIO_CALC

//----------------------------------------------------------------------
//--
//-- tetrisGame object
//--
//--    Handle the gameplay and the game (without display !)
//--
//----------------------------------------------------------------------

#ifdef DEST_CASIO_CALC
static int __callbackTick(volatile int * pTick){
    *pTick = 1;
    return TIMER_CONTINUE;
}
#endif // #ifdef DEST_CASIO_CALC

// start() : Start the tetris game
//
//  The entire game is handled by this method.
//  It retuns on error or when the game is over
//
//  @tetris : pointer to the tetris struct.
//
//  @return :  FALSE on error(s) or if canceled by user
//
BOOL tetrisGame_start(PTETRISGAME const tetris){
    if (!tetris || STATUS_READY != tetris->status) {
        return FALSE;
    }

    //Initializations ...
    tetris->currentPos.valid = FALSE;
    tetris->status = STATUS_RUNNING;
    _newPiece(tetris);

    // Set display's rotation mode
    _rotateDisplay(tetris, TRUE);

#ifdef DEST_CASIO_CALC
    int seqCount = 0;
    uint8_t nextLevel = tetris->parameters.startLevel;
    int levelTicks = tetris->parameters.startLevel==1?
            GAME_START_TICKS :
            _getSpeed(GAME_START_TICKS, tetris->parameters.startLevel - 1);
    int tickCount = levelTicks;

    // Timer creation
    static volatile int tick = 1;
    int timerID = timer_configure(TIMER_ANY, GAME_TICK_DURATION*1000,
                    GINT_CALL(__callbackTick, &tick));
    if (timerID< 0){
        return FALSE;   // Unable to create a timer
    }

    timer_start(timerID);   // set the timer

    // Game main loop
    while (tetris->status == STATUS_RUNNING){
        while(!tick){
            sleep();
        }
        tick = 0;

        _handleGameKeys(tetris);

        if (!(tickCount--)){
            _down(tetris, FALSE);    // One line down ...

            // Accelerate ?
            seqCount += 1;
            if (0 == (seqCount % GAME_NEXT_LEVEL_MOVES)){
                nextLevel = (uint8_t)floor(seqCount / GAME_NEXT_LEVEL_MOVES) + 1;

                // Change level (if necessary) & accelerate
                if (nextLevel > tetris->values[LEVEL_ID].value){
                    levelTicks = (nextLevel < GAME_MAX_ACC_LEVEL?
                        _getSpeed(levelTicks, 1): levelTicks);

                    tetris->values[LEVEL_ID].value = nextLevel;
                    _drawNumValue(tetris, LEVEL_ID);
                }
            }

            dupdate();
            tickCount = levelTicks;
        }
    }   // while (isRunning())

    // Game is Over
    timer_stop(timerID);    // stop the timer
#endif // #ifdef DEST_CASIO_CALC

    if (tetris->casioDisplay.orientation == CALC_HORIZONTAL){
        _rotateDisplay(tetris, FALSE);
    }

    playArea_defaultFont();

    return ((tetris->status & STATUS_CANCELED) != STATUS_CANCELED);
}

// _showScores() : Show best scores and current one (if in the list)
//
//  @score : new score. If equal to -1, the bests scores are shown.
//  @lines : # completed lines
//  @level : end level
//
/*
void tetrisGame::showScores(int32_t score, uint32_t lines, uint32_t level){
    sList scores;
    char data[SIZE_SCORES_FILE];
    memset(data, 0x00, SIZE_SCORES_FILE);  // scores list is empty

    // Load scores
    bFile scoresFile;
    if (scoresFile.open((FONTCHARACTER)SCORES_FILENAME, BFile_ReadOnly)){
        if (SIZE_SCORES_FILE == scoresFile.read((void*)data, SIZE_SCORES_FILE, -1)){
            _scores2List(data, scores);
        }
        scoresFile.close();
    }

    // Add the current score
    if (score != -1 && scores.add(score, lines, level)){
        scoresFile.remove((FONTCHARACTER)SCORES_FILENAME);

        // Try to create (and open) the file
        int size(SIZE_SCORES_FILE);
        scoresFile.createEx((FONTCHARACTER)SCORES_FILENAME,
                    BFile_File, &size, BFile_WriteOnly);

        // Save the new list
        if (BFILE_NO_ERROR == scoresFile.getLastError()){
            _list2Scores(scores, data);
            scoresFile.write(data, SIZE_SCORES_FILE);
        }

        scoresFile.close();
    }

    // Display scores
    //

#ifdef DEST_CASIO_CALC
    dfont(dfont_default());     // return to default font
#endif // #ifdef DEST_CASIO_CALC

    window scWin;
    window::winInfo wInf;
    wInf.title = (char*)"Best scores";
    wInf.style = WIN_STYLE_DBORDER | WIN_STYLE_HCENTER;
    wInf.pos.y = WIN_X;
    wInf.pos.w = WIN_WIDTH;
    wInf.pos.h = WIN_HEIGHT;
    wInf.bkColour = COLOUR_LT_GREY;
    scWin.create(wInf);

    sList::PNODE current(scores.head());
    if (nullptr == current){
        scWin.drawText("La liste est vide", -1, -1, COLOUR_RED);
    }
    else{
        uint8_t count(0);
        int px(15), py(8);
        char line[26];
        while (current && count < MAX_SCORES){
            line[0] = 0;
            playArea::__valtoa(++count, NULL, line, 3);
            playArea::__valtoa(current->record.score, NULL, line + 3, 9); // score
            playArea::__valtoa(current->record.lines, NULL, line + 12, 5); // lines
#ifndef FX9860G
            // No "levels" for FX9860G
            playArea::__valtoa(current->record.level, NULL, line + 17, 5); // level
#endif // FX9860G

#ifndef DEST_CASIO_CALC
            if (score == (int32_t)current->record.score){
                line[0] = '>';
            }
#endif // #ifndef DEST_CASIO_CALC

            scWin.drawText(line, px, py, (score == (int32_t)current->record.score)?
                COLOUR_RED:COLOUR_BLUE);

            // next ...
            py+=11;
            current = current->next;
        }

        scWin.update();

#ifdef DEST_CASIO_CALC
        // Wait for any key to be pressed
        getkey();
#endif // DEST_CASIO_CALC

        // Close the window
        scWin.close();
    }
}
*/

//
// "Private" methods
//

// _cancel()
//
void _cancel(PTETRISGAME const tetris){
    if ( ((tetris->status & STATUS_CANCELED) != STATUS_CANCELED)) {
        tetris->status |= STATUS_CANCELED;
    }
}

// _addDirtyLine() : Add a randomly generated dirty line in the gameplay
//
//  @tetris : pointer to a tetrisGame struct.
//  @lineID : Index of the line to fill in range [0 , PLAYFIELD_HEIGHT[
//
void _addDirtyLine(PTETRISGAME const tetris, uint8_t lineID){
    if (tetris){
        uint16_t cubes = (1 + rand() % (int)(pow(2, PLAYFIELD_WIDTH) - 1));
        uint16_t sBit = 1; // 2 ^ 0

        // Convert 'cubes' bits into coloured blocks
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
            // Is the bit set ?
            if ((cubes & sBit) > 0) {
                // yes = > add a colored block
                tetris->playField[lineID][col] = 1 + rand() % TETRAMINOS_COUNT;
            }

            // next bit value
            sBit *= 2;
        }
    }
}

// _redraw() : redraw the whole screen
//
void _redraw(PTETRISGAME const tetris){

    clearScreen(tetris->colours[COLOUR_ID_BOARD], 0, TRUE);

    _drawBackGround(tetris);
    _drawTetrisGame(tetris);

    _drawNumValue(tetris, SCORE_ID);
    _drawNumValue(tetris, LEVEL_ID);
    _drawNumValue(tetris, COMPLETED_LINES_ID);
}

// _drawBackGround() : Draw entire background
//
//  This methods will redraw all the window except the next piece preview and
//  the tetris game playfield
//
void _drawBackGround(PTETRISGAME const tetris){
    // Border around the playfield
    playArea_drawBorder(&tetris->casioDisplay,
        tetris->casioDisplay.zones[ZONE_PLAYFIELD].pos.x - CASIO_BORDER_GAP,
        tetris->casioDisplay.zones[ZONE_PLAYFIELD].pos.y - CASIO_BORDER_GAP,
        tetris->casioDisplay.zones[ZONE_PLAYFIELD].pos.w,
        tetris->casioDisplay.zones[ZONE_PLAYFIELD].pos.h,
        tetris->colours[COLOUR_ID_BORDER]);

    // Border for 'Next piece'
    playArea_drawBorder(&tetris->casioDisplay,
        tetris->casioDisplay.zones[ZONE_NEXTPIECE].pos.x - CASIO_BORDER_GAP,
        tetris->casioDisplay.zones[ZONE_NEXTPIECE].pos.y - CASIO_BORDER_GAP,
        tetris->casioDisplay.zones[ZONE_NEXTPIECE].pos.w + 1,
        tetris->casioDisplay.zones[ZONE_NEXTPIECE].pos.h + 1,
        tetris->colours[COLOUR_ID_BORDER]);
}

// _drawNextPiece() : Display the next piece
//
//  The next piece will be drawn in the preview box.
//  This zone, prior to drawinings, will be erased
//
void _drawNextPiece(PTETRISGAME const tetris){
    // Erase the "previous" next piece
    _eraseNextPiece(tetris);

    // ... and then draw the new one
    if (-1 != tetris->nextIndex) {
        _drawSinglePiece(tetris, _nextPieceDatas(tetris), 0, 0, FALSE, COLOUR_ID_NONE);
    }
}

// _eraseNextPiece() : Erase the "next piece" tetramino
//
void _eraseNextPiece(PTETRISGAME const tetris){
    uint16_t x =0, y = 0, w, h;
    playArea_shitfToZone(&tetris->casioDisplay, ZONE_PREVIEW, &x, &y, &w, &h);
    playArea_drawRectangle(&tetris->casioDisplay, x, y,
                w * PIECE_WIDTH, h * PIECE_HEIGHT,
                tetris->colours[COLOUR_ID_BOARD],
                tetris->colours[COLOUR_ID_BOARD]);
}

// _drawTetrisGame() : Draw the tetrisGame
//
void _drawTetrisGame(PTETRISGAME const tetris){
    uint16_t left, leftFirst = 0, top = 0, w, h;
    playArea_shitfToZone(&tetris->casioDisplay, ZONE_GAME, &leftFirst, &top, &w, &h);

    // Draw all the blocks (coloured or not)
    for (uint8_t y = 0; y < PLAYFIELD_HEIGHT; y++) {
        left = leftFirst;
        for (uint8_t x = 0; x < PLAYFIELD_WIDTH; x++) {
            playArea_drawRectangle(&tetris->casioDisplay, left, top, w, h, tetris->colours[tetris->playField[y][x]], NO_COLOR);
            left += w;
        }
        top -= h;
    }
}

// _rotateDisplay() : Rotate the display
//
//  @tetris : Pointer to the struct.
//  @start : Game is starting ?
//
void _rotateDisplay(PTETRISGAME const tetris, BOOL start){
    if (tetris){
        // (new) rotation mode
        playArea_rotateDisplay(&tetris->casioDisplay, start?tetris->parameters.orientation:tetris->casioDisplay.orientation);

        _redraw(tetris);

        if (!start){
            // Redraw the piece and it's shadow
            if (-1 != tetris->nextPos.shadowTopPos) {
                // first : the shadow
                _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->nextPos.index, tetris->nextPos.rotationIndex),
                        tetris->nextPos.leftPos, tetris->nextPos.shadowTopPos, TRUE, COLOUR_ID_SHADOW);
            }

            // and then the tetramino(can recover the shadow !!!!)
            _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->nextPos.index,
                        tetris->nextPos.rotationIndex),
                        tetris->nextPos.leftPos, tetris->nextPos.topPos, TRUE, COLOUR_ID_NONE);
        }

        // The next pice
        _drawNextPiece(tetris);

        // go !!!
#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC
    }
}

// _left() : Move left
//
BOOL _left(PTETRISGAME const tetris){
    // Test position
    if (_canMove(tetris, tetris->nextPos.leftPos - 1, tetris->nextPos.topPos)) {
         tetris->nextPos.leftPos -= 1;
        _piecePosChanged(tetris);
        return TRUE;    // done
    }

    return FALSE;
}

// _right() : Move right
//
BOOL _right(PTETRISGAME const tetris){
    // Test position
    if (_canMove(tetris, tetris->nextPos.leftPos + 1, tetris->nextPos.topPos)){
         tetris->nextPos.leftPos += 1;
        _piecePosChanged(tetris);
        return TRUE;    // done
    }

    return FALSE;
}

// _down() : Can the piece go down ?
//
//  Test wether the current piece can go down one row
//  When the piece has been added newly to the game and going down is
//  not possible, it means the game is over
//
BOOL down(PTETRISGAME const tetris, BOOL newPiece) {
    // Test position
    if (_canMove(tetris, tetris->nextPos.leftPos, tetris->nextPos.topPos - 1)){
        // correct
        tetris->nextPos.topPos -= 1;
        _piecePosChanged(tetris);
        return TRUE;
    }

    if (!newPiece) {
        _reachLowerPos(tetris, 0);
    }

    return FALSE;
}

// _fall() : Go down (as many lines as possible)
//
void _fall(PTETRISGAME const tetris){
    uint8_t bottom = _minTopPosition(tetris);
    uint8_t delta = tetris->nextPos.topPos - bottom;
    tetris->nextPos.topPos = bottom;

    // updates ...
    _piecePosChanged(tetris);
    _reachLowerPos(tetris, delta);
}

// _rotateLeft() : anti-clockwise rotation
//
BOOL _rotateLeft(PTETRISGAME const tetris){
    // Try to rotate
    uint8_t rotIndex = piece_rotateLeft(&tetris->tetraminos[tetris->nextPos.index]);

    // Possible ?
    if (_canMove(tetris, tetris->nextPos.leftPos, tetris->nextPos.topPos)){
        tetris->nextPos.rotationIndex = rotIndex;

        // Apply rotation
        _piecePosChanged(tetris);
        return TRUE;
     }

    // No = > cancel rotation
    piece_rotateRight(&tetris->tetraminos[tetris->nextPos.index]);
    return FALSE;
}

// _piecePosChanged() : The position of the piece has just changed
//
//  Called when the screen needs to be updated
//
void _piecePosChanged(PTETRISGAME const tetris){
    // Compute the pos or the shadow ?
    if (tetris->parameters.shadow) {
        tetris->nextPos.shadowTopPos = _minTopPosition(tetris);
    }

    // Any changes (or rotation) ?
    if (!tetris->currentPos.valid || !pieceStatus_isEqual(&tetris->currentPos, &tetris->nextPos)) {
        // Erase the tetramino(and maybe it's shadow)
        if (tetris->currentPos.valid) {
            _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->currentPos.index,
                    tetris->currentPos.rotationIndex), tetris->currentPos.leftPos,
                    tetris->currentPos.topPos, TRUE, COLOUR_ID_BOARD);
            if (-1 != tetris->currentPos.shadowTopPos) {
                // then the shadow
                _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->currentPos.index,
                    tetris->currentPos.rotationIndex), tetris->currentPos.leftPos,
                    tetris->currentPos.shadowTopPos, TRUE, COLOUR_ID_BOARD);
            }
        }

        // redraw
        if (-1 != tetris->nextPos.shadowTopPos) {
            // first : the shadow
            _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->nextPos.index,
                    tetris->nextPos.rotationIndex), tetris->nextPos.leftPos,
                    tetris->nextPos.shadowTopPos, TRUE, COLOUR_ID_SHADOW);
        }

        // and then the tetramino(can recover the shadow !!!!)
        _drawSinglePiece(tetris, _pieceDatas(tetris, tetris->nextPos.index,
                    tetris->nextPos.rotationIndex),
                    tetris->nextPos.leftPos, tetris->nextPos.topPos, TRUE, COLOUR_ID_NONE);

#ifdef DEST_CASIO_CALC
        dupdate();
#endif // #ifdef DEST_CASIO_CALC

        pieceStatus_copy(&tetris->currentPos, &tetris->nextPos);
    }
}

// _getSpeed() : Get the game speed in ticks according to the level
//
//  The player can move the piece every 'tick' but the piecce will
//  automatically go down one line every #currentTicks
//
//  This 'speed' is linked to the level in the game
//
//  @currentTicks : current tick count
//  @incLevel : value of current increment for the level (1 by default)
//
//  @return : new duration in ticks
//
int _getSpeed(int currentTicks, uint8_t incLevel){

        // newTicks = currentTicks * acc ^ incLevel
        int ticks = currentTicks;
        int count = incLevel;
        int accRate = 1000 -GAME_ACC_RATE;
        while (count--){
            ticks = ticks* accRate / 1000;
        }

        return ticks;
}

// _handleGameKeys() : Handle keyboard events
//
//  This methods returns even if no event is in the queue
//
void _handleGameKeys(PTETRISGAME const tetris){
    char car = getKey();

    if(car != KEY_NONE) {
        if (tetris->casioDisplay.keys[KEY_ID_QUIT] == car){
            _cancel(tetris);
            return;
        }

#ifdef DEST_CASIO_CALC
        if (tetris->casioDisplay.keys[KEY_ID_PAUSE] == car){
            tetrisGame_pause(tetris);
            return;
        }
#endif // #ifdef DEST_CASIO_CALC

        if (tetris->casioDisplay.keys[KEY_ID_ROTATE_DISPLAY] == car){
            _rotateDisplay(tetris, FALSE);
            return;
        }

        if (tetris->casioDisplay.keys[KEY_ID_LEFT] == car){
            _left(tetris);
            return;
        }

        if (tetris->casioDisplay.keys[KEY_ID_RIGHT] == car){
            _right(tetris);
            return;
        }

        if (tetris->casioDisplay.keys[KEY_ID_ROTATE] == car){
            _rotateLeft(tetris);
            return;
        }

        if (tetris->casioDisplay.keys[KEY_ID_DOWN] == car){
            _down(tetris, FALSE);
            return;
        }

        if (tetris->casioDisplay.keys[KEY_ID_FALL] == car){
            _fall(tetris);
            return;
        }
    }
}

// _canMove : Can the current piece be at the given position ?
//
//  Since a tetramino doesn't fill the whole 4x4 matrix,
//  leftPos can be negative (ie empty spaces are on the left of the screen)
//
//  @tetris : pointer to the struct
//  @leftPos, @topPos : Position to test
//
//  @return : TRUE if the position is free and can be used by the tetramino
//
BOOL _canMove(PTETRISGAME const tetris, int8_t leftPos, uint8_t  topPos){
    // Piece's datas (in its current state)
    uint8_t* datas = piece_currentDatas(tetris->tetraminos[tetris->nextPos.index]);

    // Max index visible on desk
    uint8_t maxY = (topPos >= PLAYFIELD_HEIGHT) ?
            PIECE_HEIGHT - 1 + PLAYFIELD_HEIGHT - topPos :
            PIECE_HEIGHT - 1;

    // Test all the contained blocks starting from bottom
    int8_t realX = 0, realY = 0;
    for (int8_t y = maxY; y >=0; y--){
        for (uint8_t x = 0; x < PIECE_WIDTH; x++) {
            if (COLOUR_ID_BOARD != datas[y * PIECE_WIDTH + x]) {
                // "real" position of the block
                realX = x + leftPos;
                realY = topPos - y;

                // out of the gameplay's limits ?
                if (realX < 0 || realY < 0 || realX >= PLAYFIELD_WIDTH) {
                    return FALSE;
                }

                // Is there a block at this place ?
                if (realY < PLAYFIELD_HEIGHT &&
                        tetris->playField[realY][realX] != COLOUR_ID_BOARD) {
                    return FALSE;
                }
            }
        }
    }

    // Yes = > the position is valid
    return TRUE;
}

// _minTopPosition() : Get a piece min.pos.index(vertical value)
//
//  @tetris : pointer to the struct
//  @return : index of the lowest possible position for the current piece
//
uint8_t _minTopPosition(PTETRISGAME const tetris){
    uint8_t currentTop = tetris->nextPos.topPos;

    // Try to move one line down
    while (_canMove(tetris, tetris->nextPos.leftPos, currentTop)){
        currentTop -= 1;
    }

    // current pos is invalid = > go up one line
    return currentTop + 1;
}

// _drawSinglePiece() : Draw a whole tetramino using the given colour
//
//  @tetris : pointer to the game
//  @datas is the piece'datas in its current rotation state
//  @cornerX, @cornerY are the coordinates of the upper left corner
//                  in blocks coordinates
//  @inTetrisGame : True = > draw in the tetrisGame, False = > draw "next" piece
//  @specialColourID is the colour to use for the tetramino.
//   If set to COLOUR_ID_NONE the tetramino's colour will be used
//
void _drawSinglePiece(PTETRISGAME const tetris, uint8_t* datas, uint16_t cornerX,
        uint16_t cornerY, BOOL inTetrisGame, uint8_t specialColourID){
    // First visible row ID
    int8_t rowFirst = 0;
    if (inTetrisGame) {
        rowFirst = (cornerY > (PLAYFIELD_HEIGHT - 1))?cornerY+1-PLAYFIELD_HEIGHT:0;
    }

    uint16_t x, xFirst = cornerX, y = (cornerY - rowFirst), w, h;
    playArea_shitfToZone(&tetris->casioDisplay, inTetrisGame?ZONE_GAME:ZONE_PREVIEW,
            &xFirst, &y, &w, &h);  // Get coords and dims. in the screen

    uint8_t colourID;
    for (uint8_t row = rowFirst; row < PIECE_HEIGHT; row++) {
        x = xFirst;
        for (uint8_t col = 0; col < PIECE_WIDTH; col++) {
            colourID = datas[row * PIECE_WIDTH + col];
            if (colourID != COLOUR_ID_BOARD) {

                // Draw the block

                // A shadow ?
                if (COLOUR_ID_SHADOW == specialColourID){
                    playArea_drawRectangle(&tetris->casioDisplay, x, y, w, h, tetris->colours[COLOUR_ID_SHADOW], NO_COLOR);
                }
                else{
                    playArea_drawRectangle(&tetris->casioDisplay, x, y, w, h,
                        tetris->colours[(COLOUR_ID_NONE != specialColourID) ?
                        specialColourID : colourID], NO_COLOR);
                }
            }
            x += w;
        }
        y += h;
    }
}

// _newPiece() : New piece (in the game)
//
//  The piece in the preview area will appear in the playfield
//  A new "next piece" will be shown in the preview area
//
void _newPiece(PTETRISGAME const tetris){
    // Next piece => current
    tetris->nextPos.index = (-1 == tetris->nextIndex ? _newPieceIndex() : tetris->nextIndex);

    // Next one
    tetris->nextIndex = _newPieceIndex();

    // The piece is a the top of the game play, centered horizontally
    tetris->nextPos.leftPos = (int)((PLAYFIELD_WIDTH - PIECE_WIDTH) / 2);
    tetris->nextPos.topPos = PLAYFIELD_HEIGHT + tetris->tetraminos[tetris->nextPos.index].vertOffset;
    tetris->nextPos.shadowTopPos = -1;
    tetris->nextPos.rotationIndex = 0;
    tetris->tetraminos[tetris->nextPos.index].rotateID = 0;

    // Next piece
    _drawNextPiece(tetris);

    // Can I go on line down ?
    if (!_down(tetris, TRUE)) {
        // No = > the game is over
        tetris->status |= STATUS_STOPPED;
    }
}

// _clearLine() : Clear and remove a completed line
//
//  When a line is completed (ie. all horizontal boxes are colored)
//  it disapperas from the game. All lines "below" will move down.
//
//  @tetris : pointer to the tris struct.
//  @index : index of the line to clear in range [0 , PLAYFIELD_HEIGHT[
//
void _clearLine(PTETRISGAME const tetris, uint8_t index){
    if (index < PLAYFIELD_HEIGHT) {
        // Remove the line from the screen
        for (uint8_t line = index; line < (PLAYFIELD_HEIGHT - 1); line++) {
            for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
                tetris->playField[line][col] = tetris->playField[line + 1][col];
            }
        }

        // Add a new empty line
        for (uint8_t col=0; col<PLAYFIELD_WIDTH; col++){
            tetris->playField[PLAYFIELD_HEIGHT-1][col] = COLOUR_ID_BOARD;
        }
    }
}

// _putPiece : Put the tetramino at the current position
//
//  This methods is used to put a tetramino on the playing area.
//  The tetramino is no longer mobile.
//
void _putPiece(PTETRISGAME const tetris){
    uint8_t* datas = piece_currentDatas(tetris->tetraminos[tetris->nextPos.index]);
    uint8_t bColour = 0;

    // Copy all the colored blocks in the gameplay
    int8_t y;
    for (uint8_t row = 0; row < PIECE_HEIGHT; row++) {
        y = tetris->nextPos.topPos - row;
        if (y>=0 && y < PLAYFIELD_HEIGHT){
            for (uint8_t x = 0; x < PIECE_WIDTH; x++) {
                bColour = datas[row * PIECE_WIDTH + x];
                if (COLOUR_ID_BOARD != bColour) {
                    tetris->playField[y][x + tetris->nextPos.leftPos] = bColour;
                }
            }
        }
    }
}

// _reachLowerPos() : Update the datas when a tetramino has just ended its fall
//
//  @tetris : pointer to tetris struct.
//  @downRowCount : count of down'rows
//
void _reachLowerPos(PTETRISGAME const tetris, uint8_t downRowcount){
    // put it
    _putPiece(tetris);

    // Don't erase this piece !!!
    tetris->currentPos.valid = FALSE;

    // Are line(s) completed ?
    // Check the 4 possible lines
    uint8_t completedLines[4];
    uint8_t completedCount = 0;       // # of completed lines

    int8_t minY = tetris->nextPos.topPos - PIECE_HEIGHT + 1;
    int8_t maxY = minY + PIECE_HEIGHT -1;

    // Not too high
    if (maxY > PLAYFIELD_HEIGHT){
        maxY = PLAYFIELD_HEIGHT;
    }
    else{
        if (minY < 0){
            minY = 0;   // Not too low
        }
    }

    BOOL foundEmpty = FALSE;
    for (uint8_t line = minY; line <= maxY; line++){
        foundEmpty = FALSE;
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH && !foundEmpty; col++){
            if (COLOUR_ID_BOARD == tetris->playField[line][col]){
                foundEmpty = TRUE;
            }
        }

        // Is the line complete ?
        if (!foundEmpty){
            completedLines[completedCount++] = line;
        }
    }

    // Remove lines in reverse order (max -> min)
    for (int8_t lineID = (completedCount-1); lineID >=0; lineID--){
        // Update datas
        _clearLine(tetris, completedLines[lineID]);
    }

    // Update the score
    if (completedCount){
        double delta = 0.0;
        switch(completedCount){
            case 1:
                delta = 100.0;
                break;

            case 2:
                delta = 300.0;
                break;

            case 3:
                delta = 500.0;
                break;

            // 4
            default:
                delta = 800.0;
                break;
        }

        double mult = (100. + SCORE_SPEED_GAME * downRowcount
                + SCORE_DIRTY_LINES * tetris->parameters.dirtyLines
                + SCORE_LEVEL_VALUATION * tetris->values[COMPLETED_LINES_ID].value);
        if (!tetris->parameters.shadow){
            mult += SCORE_NO_SHADOW;
        }

        // Updates
        tetris->values[SCORE_ID].value+=(uint32_t)(delta * mult / 100.0);
        tetris->values[COMPLETED_LINES_ID].value+=completedCount;

        _drawNumValue(tetris, SCORE_ID);
        _drawNumValue(tetris, COMPLETED_LINES_ID);
        _drawTetrisGame(tetris);
    } // if (completedCount)

    // Get a new piece
    _newPiece(tetris);

#ifdef DEST_CASIO_CACL
    dupdate();
#endif // #ifdef DEST_CASIO_CACL
}

// _drawNumValue() : Draw a value and its name
//
//  @tetris : pointer to the tetris struct.
//  @index of the VALUE object to be drawn
//
void _drawNumValue(PTETRISGAME const tetris, uint8_t index){

    char valStr[MAX_VALUE_NAME_LEN + 1];

    // Erase previous value ?
    if (-1 != tetris->values[index].previous){
        playArea_valtoa(tetris->values[index].previous, tetris->values[index].name, valStr, 0);

        playArea_dtext(&tetris->casioDisplay,
            tetris->casioDisplay.texts[index].x,
            tetris->casioDisplay.texts[index].y,
            tetris->colours[COLOUR_ID_BKGRND], valStr);
    }

    // print new value
    playArea_valtoa(tetris->values[index].value, tetris->values[index].name, valStr, 0);
    playArea_dtext(&tetris->casioDisplay,
            tetris->casioDisplay.texts[index].x,
            tetris->casioDisplay.texts[index].y,
            tetris->colours[COLOUR_ID_TEXT], valStr);

    tetris->values[index].previous = tetris->values[index].value; // to erase the value next time
}

// _scores2List() : Transfer file content to the list os scores
//
//  @data : Buffer read from scores file
//  @scores : List
//
/*
void tetrisGame::_scores2List(char* data, sList& scores){
    scores.clear();

    if (data){
        sList::RECORD record;
        char* pos(data);
        for (uint8_t i=0; i<MAX_SCORES; i++){
            memcpy(&record, pos, SIZE_SCORE);

            // append to list (no need to add, values are already ordered)
            if (record.score){
                scores.append(record.score, record.lines
#ifndef FX9860G
                    , record.level
#endif // #ifndef FX9860G
                );
            }
            pos+=SIZE_SCORE;    // next record
        }
    }
}
*/

// _scores2List() : Transfer list of scores to a buffer
//
//  @scores : List
//  @data : Destination buffer
//
/*
void tetrisGame::_list2Scores(sList& scores, char* data){
    memset(data, 0x00, SIZE_SCORES_FILE);  // dest. buffer is empty

    sList::PNODE item(scores.head());
    uint8_t index(0);
    char* pos(data);
    sList::RECORD* prec;
    while (item && index++ < MAX_SCORES){
        prec = &item->record;
        memcpy(pos, (char*)prec, SIZE_SCORE);
        item = item->next;  // Next score
        pos+=SIZE_SCORE;    // forward
    }
}
*/

// Datas of a piece
uint8_t* _nextPieceDatas(PTETRISGAME const tetris){
    return ((tetris->nextIndex < 0 || tetris->nextIndex >= TETRAMINOS_COUNT) ?
         NULL : tetris->tetraminos[tetris->nextIndex].points[0]);
}

uint8_t* _pieceDatas(PTETRISGAME const tetris, int8_t index, uint8_t  rotIndex){
    return ((index < 0 || index >= TETRAMINOS_COUNT
            || rotIndex >= tetris->tetraminos[index].maxRotate) ?
            NULL : tetris->tetraminos[index].points[rotIndex]);
}

// EOF
