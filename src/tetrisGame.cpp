//---------------------------------------------------------------------------
//--
//--	File	: tetrisGame.cpp
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris / cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--	    Implementation of tetrisGame object
//--
//---------------------------------------------------------------------------

#include "tetrisGame.h"

#include <cstdio>
#include <cmath>
#include <time.h>

#include "shared/window.h"
#include "shared/bFile.h"

//---------------------------------------------------------------------------
//--
//-- tetrisGame object
//--
//--    Handle the gameplay and the game (without display !)
//--
//---------------------------------------------------------------------------

// Construction
//
tetrisGame::tetrisGame(tetrisParameters* params) {

    // Initialize rand num. generator
    srand((unsigned int)clock());

    // Set indicators name
    strcpy(values_[SCORE_ID].name, SCORE_STR);
    strcpy(values_[LEVEL_ID].name, LEVEL_STR);
    strcpy(values_[COMPLETED_LINES_ID].name, COMPLETED_LINES_STR);

    // Build the tetraminos'list
    //

    // S
    tetraminos_[0].addRotation(S_0);
    tetraminos_[0].addRotation(S_1);

    // Z
    tetraminos_[1].addRotation(Z_0);
    tetraminos_[1].addRotation(Z_1);

    // I
    tetraminos_[2].addRotation(I_0);
    tetraminos_[2].addRotation(I_1);

    // O
    tetraminos_[3].addRotation(O_0);

    // L
    tetraminos_[4].addRotation(L_0);
    tetraminos_[4].addRotation(L_1);
    tetraminos_[4].addRotation(L_2);
    tetraminos_[4].addRotation(L_3);

    // J
    tetraminos_[5].addRotation(J_0);
    tetraminos_[5].addRotation(J_1);
    tetraminos_[5].addRotation(J_2);
    tetraminos_[5].addRotation(J_3);

    // T
    tetraminos_[6].addRotation(T_0);
    tetraminos_[6].addRotation(T_1);
    tetraminos_[6].addRotation(T_2);
    tetraminos_[6].addRotation(T_3);

    // Colours
    //
    colours_[COLOUR_ID_BOARD] = COLOUR_WHITE;
    colours_[1] = COLOUR_RED;           // Pieces (1 to 7)
    colours_[2] = COLOUR_GREEN;
    colours_[3] = COLOUR_YELLOW;
    colours_[4] = COLOUR_BLUE;
    colours_[5] = COLOUR_PURPLE;
    colours_[6] = COLOUR_CYAN;
    colours_[7] = COLOUR_ORANGE;
    colours_[COLOUR_ID_SHADOW] = COLOUR_LT_GREY;
    colours_[COLOUR_ID_TEXT] = COLOUR_BLACK;
    colours_[COLOUR_ID_BORDER] = COLOUR_DK_GREY;
    colours_[COLOUR_ID_BKGRND] = COLOUR_WHITE;  // could be different from board !

    setParameters(params);
}

// setParameters() : Set game's parameters
//
//  @params : Struct. containining parameters for the game
//    These parameters are choosen by the user
//
void tetrisGame::setParameters(tetrisParameters* params) {
    // Copy (and init) parameters
    parameters_.copy(params);

    values_[SCORE_ID].value = 0;
    values_[LEVEL_ID].value = parameters_.startLevel_;
    values_[COMPLETED_LINES_ID].value = 0;
    nextIndex_ = -1;

    // Initialization of tetraminos(no rotation)
    for (uint8_t index = 0; index < TETRAMINOS_COUNT; index++) {
        tetraminos_[index].rotateBack();
    }

    //  Screen rotation
    casioDisplay_.rotatedDisplay(params->rotatedDisplay_ == TRUE, true);

    // Empty the playset
    _emptyTetrisGame();

    // Add dirty lines ...
    //
    uint8_t maxLines(PLAYFIELD_HEIGHT - PIECE_HEIGHT - 1);
    if (parameters_.dirtyLines_ > maxLines) {
        parameters_.dirtyLines_ = maxLines;
    }
    for (uint8_t index = 0; index < parameters_.dirtyLines_; index++) {
        _addDirtyLine(index);
    }

    // Ready for the game
    status_ = STATUS_READY;
}

// start() : Start the tetris game
//
//  The entire game is handled by this method.
//  It retuns on error or when the game is over
//
//  @return :  false on error(s) or cancel
//
bool tetrisGame::start() {
    // Check the object state
    if (STATUS_READY != status_) {
        return false;
    }

    //Initializations ...
    currentPos_.valid(false);
    status_ = STATUS_RUNNING;
    _newPiece();

    // Set display's rotation mode
    _rotateDisplay(true);

#ifdef DEST_CASIO_CALC

    int seqCount(0);
    uint8_t nextLevel(parameters_.startLevel_);
    int levelTicks(parameters_.startLevel_==1? GAME_START_TICKS : _getSpeed(GAME_START_TICKS, parameters_.startLevel_ - 1));
    int tickCount(levelTicks);

    // Timer creation
    static volatile int tick = 1;
    int timerID = timer_configure(TIMER_ANY, GAME_TICK_DURATION*1000, GINT_CALL(__callbackTick, &tick));
    if (timerID< 0){
        return false;	// Unable to create a timer
    }

    timer_start(timerID);   // set the timer

    // Game main loop
    while (isRunning()){
        while(!tick){
            sleep();
        }
        tick = 0;

        _handleGameKeys();

        if (!(tickCount--)){
            _down();    // One line down ...

            // Accelerate ?
            seqCount += 1;
            if (0 == (seqCount % GAME_NEXT_LEVEL_MOVES)){
                nextLevel = (uint8_t)floor(seqCount / GAME_NEXT_LEVEL_MOVES) + 1;

                // Change level (if necessary) & accelerate
                if (nextLevel > values_[LEVEL_ID].value){
                    levelTicks = (nextLevel < GAME_MAX_ACC_LEVEL?_getSpeed(levelTicks, 1): levelTicks);

                    values_[LEVEL_ID].value = nextLevel;
                    _drawNumValue(LEVEL_ID);
                }
            }

            updateDisplay();
            tickCount = levelTicks;
        }
    }   // while (isRunning())

    // Game is Over
    timer_stop(timerID);    // stop the timer
#endif // #ifdef DEST_CASIO_CALC

    if (casioDisplay_.isRotated()){
        _rotateDisplay(); // Cancel rotation if any
    }
    
    return (!isCancelled());
}

// pause() : Pause or resume the game
//
void tetrisGame::pause(){
    char car(KEY_CODE_NONE);
    bool paused(true);

    // draw the picture
#ifdef DEST_CASIO_CALC
    dimage(0, 0, &img_pause);
#endif // #ifdef DEST_CASIO_CALC
    casioDisplay_.update();

    // status_ = STATUS_PAUSED;
    do{
        car = keyboard_.getKey();

        // Resume ?
        if (casioDisplay_.keyPause_== car){
            _redraw();
            updateDisplay();
            paused = false;
        }
        else{
            // Exit ?
            if (casioDisplay_.keyQuit_ == car){
                cancel();
                paused = false;
            }
        }

    }while (paused);
}

// _showScores() : Show best scores and current one (if in the list)
//
//  @score : new score. If equal to -1, the bests scores are shown.
//  @lines : # completed lines
//  @level : end level
//
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

        // Try to create the file
        int size(SIZE_SCORES_FILE);
        scoresFile.create((FONTCHARACTER)SCORES_FILENAME, BFile_File, &size);

        // Save the new list
        if (scoresFile.getLastError() == 0){
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
    wInf.position.y = WIN_X;
    wInf.position.w = WIN_WIDTH;
    wInf.position.h = WIN_HEIGHT;
#ifndef FX9860G
    wInf.bkColour = COLOUR_LT_GREY;
#endif // #ifdef FX9860G
    scWin.create(wInf);

    sList::PNODE current(scores.head());
    if (nullptr == current){
        scWin.drawText("La liste est vide", -1, -1
#ifndef FX9860G
            , COLOUR_RED
#endif // #ifndef FX9860G
            );
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
#ifndef FX9860G
                COLOUR_RED:COLOUR_BLUE
#else
                C_BLACK:C_LIGHT
#endif // FX9860G
            );

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

//
// "Private" methods
//

// _rotateDisplay() : Rotate the display
//
//  @first : Game is starting ?
//
void tetrisGame::_rotateDisplay(bool first){

    // (new) rotation mode
    casioDisplay_.rotatedDisplay(first?parameters_.rotatedDisplay_:!casioDisplay_.isRotated());

    _redraw();

    if (!first){
        // Redraw the piece and it's shadow
        if (-1 != nextPos_.shadowTopPos_) {
            // first : the shadow
            _drawSinglePiece(_pieceDatas(nextPos_.index_, nextPos_.rotationIndex_), nextPos_.leftPos_, nextPos_.shadowTopPos_, true, COLOUR_ID_SHADOW);
        }

        // and then the tetramino(can recover the shadow !!!!)
        _drawSinglePiece(_pieceDatas(nextPos_.index_, nextPos_.rotationIndex_), nextPos_.leftPos_, nextPos_.topPos_);
    }

    // The next pice
    _drawNextPiece(nextIndex_);

    // go !!!
    updateDisplay();
}


// _left() : Move left
//
//  @return : done ?
//
bool tetrisGame::_left(){
    // Test position
    if (_canMove(nextPos_.leftPos_ - 1, nextPos_.topPos_)) {
        // Correct
        nextPos_.leftPos_ -= 1;
        _piecePosChanged();
        return true;
    }

    // Impossible
    return false;
}

// _right() : Move right
//
//  @return : done ?
//
bool tetrisGame::_right(){
    // Test position
    if (_canMove(nextPos_.leftPos_ + 1, nextPos_.topPos_)){
        // Correct
        nextPos_.leftPos_ += 1;
        _piecePosChanged();
        return true;
    }

    // Impossible
    return false;
}

// _down() : Can the piece go down ?
//
//  Test wether the current piece can go down one row
//  When the piece has been added newly to the game and going down is
//  not possible, it means the game is over
//
//  @newPiece : The piece has just been added ?
//
//  @return : true if the piece can move one row down
//
bool tetrisGame::_down(bool newPiece) {
    // Test position
    if (_canMove(nextPos_.leftPos_, nextPos_.topPos_ - 1)) {
        // correct
        nextPos_.topPos_ -= 1;
        _piecePosChanged();
        return true;
    }

    if (!newPiece) {
        _reachLowerPos();
    }

    return false;
}

// _fall() : Go down (as many lines as possible)
//
void tetrisGame::_fall(){
    uint8_t bottom(_minTopPosition());
    uint8_t delta(nextPos_.topPos_ - bottom);
    nextPos_.topPos_ = bottom;

    // updates ...
    _piecePosChanged();
    _reachLowerPos(delta);
}

// _rotateLeft() : anti-clockwise rotation
//
//  @return : done ?
//
bool tetrisGame::_rotateLeft() {

    // Try to rotate
    uint8_t rotIndex = tetraminos_[nextPos_.index_].rotateLeft();

    // Possible ?
    if (_canMove(nextPos_.leftPos_, nextPos_.topPos_)){
        nextPos_.rotationIndex_ = rotIndex;

        // Apply rotation
        _piecePosChanged();
        return true;
     }

    // No = > cancel rotation
    tetraminos_[nextPos_.index_].rotateRight();
    return false;
}

// _piecePosChanged() : The position of the piece has just changed
//
//  Called when the screen needs to be updated
//
void tetrisGame::_piecePosChanged() {
    // Compute the pos or the shadow ?
    if (parameters_.shadow_) {
        nextPos_.shadowTopPos_ = _minTopPosition();
    }

    // Any changes (or rotation) ?
    if (!currentPos_.isValid() || currentPos_ != nextPos_) {
        // Erase the tetramino(and maybe it's shadow)
        if (currentPos_.isValid()) {
            _drawSinglePiece(_pieceDatas(currentPos_.index_, currentPos_.rotationIndex_), currentPos_.leftPos_, currentPos_.topPos_, true, COLOUR_ID_BOARD);
            if (-1 != currentPos_.shadowTopPos_) {
                // then the shadow
                _drawSinglePiece(_pieceDatas(currentPos_.index_, currentPos_.rotationIndex_), currentPos_.leftPos_, currentPos_.shadowTopPos_, true, COLOUR_ID_BOARD);
            }
        }

        // redraw
        if (-1 != nextPos_.shadowTopPos_) {
            // first : the shadow
            _drawSinglePiece(_pieceDatas(nextPos_.index_, nextPos_.rotationIndex_), nextPos_.leftPos_, nextPos_.shadowTopPos_, true, COLOUR_ID_SHADOW);
        }

        // and then the tetramino(can recover the shadow !!!!)
        _drawSinglePiece(_pieceDatas(nextPos_.index_, nextPos_.rotationIndex_), nextPos_.leftPos_, nextPos_.topPos_);

        updateDisplay();
        currentPos_ = nextPos_;
    }
}

// _getSpeed() : Get the game speed in ticks according to the level
//
//  The player can move the piece every 'tick' but the piecce will automatically
//  go down one line every #currentTicks
//
//  This 'speed' is linked to the level in the game
//
//  @currentTicks : current tick count
//  @incLevel : value of current increment for the level (1 by default)
//
//  @return : new duration in ticks
//
int tetrisGame::_getSpeed(int currentTicks, uint8_t incLevel){
        /*
        if (!incLevel || level >= GAME_MAX_ACC_LEVEL){
            return currentTicks;
        }
        */

        // newTicks = currentTicks * acc ^ incLevel
        int ticks(currentTicks);
        int count(incLevel);
        int accRate(1000 -GAME_ACC_RATE);
        while (count--){
            ticks = ticks* accRate / 1000;
        }

        return ticks;
}

// _handleGameKeys() : Handle keyboard events
//
//  This methods returns even if no event is in the queue
//
void tetrisGame::_handleGameKeys() {
    char car(keyboard_.getKey());

	if(car != KEY_CODE_NONE) {
        if (casioDisplay_.keyQuit_ == car){
            cancel();
            return;
        }

        if (casioDisplay_.keyPause_ == car){
            pause();
            return;
        }

        if (casioDisplay_.keyRotateDisplay_ == car){
            _rotateDisplay();
            return;
        }

        if (casioDisplay_.keyLeft_ == car){
            _left();
            return;
        }

        if (casioDisplay_.keyRight_ == car){
            _right();
            return;
        }

        if (casioDisplay_.keyRotatePiece_ == car){
            _rotateLeft();
            return;
        }

        if (casioDisplay_.keyDown_ == car){
            _down();
            return;
        }

        if (casioDisplay_.keyFall_ == car){
            _fall();
            return;
        }
	}
}

// _canMove : Can the current piece be at the given position ?
//
//  Since a tetramino doesn't fill the whole 4x4 matrix,
//  leftPos can be negative (ie empty spaces are on the left of the screen)
//
//  @leftPos, @topPos : Position to test
//
//  @return : true if the position is free and can be used by the tetramino
//
bool tetrisGame::_canMove(int8_t leftPos, uint8_t  topPos) {
    // Piece's datas (in its current state)
    uint8_t* datas = tetraminos_[nextPos_.index_].currentDatas();

    // Max index visible on desk
    uint8_t maxY = (topPos >= PLAYFIELD_HEIGHT) ? PIECE_HEIGHT - 1 + PLAYFIELD_HEIGHT - topPos : PIECE_HEIGHT - 1;

    // Test all the contained blocks starting from bottom
    int8_t realX(0), realY(0);
    for (int8_t y = maxY; y >=0; y--){
        for (uint8_t x = 0; x < PIECE_WIDTH; x++) {
            if (COLOUR_ID_BOARD != datas[y * PIECE_WIDTH + x]) {
                // "real" position of the block
                realX = x + leftPos;
                realY = topPos - y;

                // out of the gameplay's limits ?
                if (realX < 0 || realY < 0 || realX >= PLAYFIELD_WIDTH) {
                    return false;
                }

                // Is there a block at this place ?
                if (realY < PLAYFIELD_HEIGHT && playField_[realY][realX] != COLOUR_ID_BOARD) {
                    return false;
                }
            }
        }
    }

    // Yes = > the position is valid
    return true;
}

// _minTopPosition() : Get a piece min.pos.index(vertical value)
//
//  @return : index of the lowest possible position for the current piece
//
uint8_t tetrisGame::_minTopPosition(){
    uint8_t currentTop(nextPos_.topPos_);

    // Try to move one line down
    while (_canMove(nextPos_.leftPos_, currentTop)){
        currentTop -= 1;
    }

    // current pos is invalid = > go up one line
    return currentTop + 1;
}

// _newPiece() : New piece (in the game)
//
//  The piece in the preview area will appear in the playfield
//  A new "next piece" will be shown in the preview area
//
void tetrisGame::_newPiece() {
    // Next piece => current
    nextPos_.index((-1 == nextIndex_) ? _newPieceIndex() : nextIndex_);

    // Next one
    nextIndex_ = _newPieceIndex();

    // The piece is a the top of the game play, centered horizontally
    nextPos_.leftPos_ = int((PLAYFIELD_WIDTH - PIECE_WIDTH) / 2);
    nextPos_.topPos_ = PLAYFIELD_HEIGHT + tetraminos_[nextPos_.index_].verticalOffset();
    nextPos_.shadowTopPos_ = -1;
    nextPos_.rotationIndex_ = 0;
    tetraminos_[nextPos_.index_].rotateBack();

    // Next piece
    _drawNextPiece(nextIndex_);

    // Can I go on line down ?
    if (!_down(true)) {
        // No = > the game is over
        end();
    }
}

// _clearLine() : Clear and remove a completed line
//
//  When a line is completed (ie. all horizontal boxes are colored)
//  it disapperas from the game. All lines "below" will move down.
//
//  @index : index of the line to clear in range [0 , PLAYFIELD_HEIGHT[
//
void tetrisGame::_clearLine(uint8_t index) {
    if (index < PLAYFIELD_HEIGHT) {
        // Remove the line from the screen
        for (uint8_t line = index; line < (PLAYFIELD_HEIGHT - 1); line++) {
            for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
                playField_[line][col] = playField_[line + 1][col];
            }
        }

        // Add a new empty line
        for (uint8_t col=0; col<PLAYFIELD_WIDTH; col++){
            playField_[PLAYFIELD_HEIGHT-1][col] = COLOUR_ID_BOARD;
        }
    }
}

// _addDirtyLine() : Add a randomly generated dirty line in the gameplay
//
//  @lineID : Index of the line to fill in range [0 , PLAYFIELD_HEIGHT[
//
void tetrisGame::_addDirtyLine(uint8_t lineID) {
    uint16_t cubes(1 + rand() % int(pow(2, PLAYFIELD_WIDTH) - 1));
    uint16_t sBit(1); // 2 ^ 0

    // Convert 'cubes' bits into coloured blocks
    for (uint8_t col = 0; col < PLAYFIELD_WIDTH; col++) {
        // Is the bit set ?
        if ((cubes & sBit) > 0) {
            // yes = > add a colored block
            playField_[lineID][col] = 1 + rand() % TETRAMINOS_COUNT;
        }

        // next bit value
        sBit *= 2;
    }
}

// _putPiece : Put the tetramino at the current position
//
//  This methods is used to put a tetramino on the playing area.
//  The tetramino is no longer mobile.
//
void tetrisGame::_putPiece() {
    uint8_t* datas = tetraminos_[nextPos_.index_].currentDatas();
    uint8_t bColour(0);

    // Copy all the colored blocks in the gameplay
    int8_t y;
    for (uint8_t row = 0; row < PIECE_HEIGHT; row++) {
        y = nextPos_.topPos_ - row;
        if (y>=0 && y < PLAYFIELD_HEIGHT){
            for (uint8_t x = 0; x < PIECE_WIDTH; x++) {
                bColour = datas[row * PIECE_WIDTH + x];
                if (COLOUR_ID_BOARD != bColour) {
                    playField_[y][x + nextPos_.leftPos_] = bColour;
                }
            }
        }
    }
}

// _reachLowerPos() : Update the datas when a tetramino has just ended its fall
//
//  @downRowCount : count of down'rows
//
void tetrisGame::_reachLowerPos(uint8_t downRowcount){
    // put it
    _putPiece();

    // Don't erase this piece !!!
    currentPos_.valid(false);

    // Are line(s) completed ?
    // Check the 4 possible lines
    uint8_t completedLines[4];
    uint8_t completedCount(0);       // # of completed lines

    int8_t minY(nextPos_.topPos_ - PIECE_HEIGHT + 1);
    int8_t maxY(minY + PIECE_HEIGHT -1);

    // Not too high
    if (maxY > PLAYFIELD_HEIGHT){
        maxY = PLAYFIELD_HEIGHT;
    }
    else{
        if (minY < 0){
            minY = 0;   // Not too low
        }
    }

    bool foundEmpty(false);
    for (uint8_t line = minY; line <= maxY; line++){
        foundEmpty = false;
        for (uint8_t col = 0; col < PLAYFIELD_WIDTH && !foundEmpty; col++){
            if (COLOUR_ID_BOARD == playField_[line][col]){
                foundEmpty = true;
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
        _clearLine(completedLines[lineID]);
    }

    // Update the score
    if (completedCount){
        double delta(0.0);
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

        double mult(100. + SCORE_SPEED_GAME * downRowcount + SCORE_DIRTY_LINES * parameters_.dirtyLines_ + SCORE_LEVEL_VALUATION * values_[COMPLETED_LINES_ID].value);
        if (!parameters_.shadow_){
            mult += SCORE_NO_SHADOW;
        }

        // Updates
        values_[SCORE_ID].value+=uint32_t(delta * mult / 100.0);
        values_[COMPLETED_LINES_ID].value+=completedCount;

        _drawNumValue(SCORE_ID);
        _drawNumValue(COMPLETED_LINES_ID);
        _drawTetrisGame();
    } // if (completedCount)

    // Get a new piece
    _newPiece();
    updateDisplay();
}

// _redraw() : redraw the whole screen
//
void tetrisGame::_redraw(){
    // Clear the screen
    casioDisplay_.clear(colours_[COLOUR_ID_BOARD]);

    _drawBackGround();
    _drawTetrisGame();

    _drawNumValue(SCORE_ID);
    _drawNumValue(LEVEL_ID);
    _drawNumValue(COMPLETED_LINES_ID);
}

// _drawSinglePiece() : Draw a whole tetramino using the given colour
//
//  @datas is the piece'datas in its current rotation state
//  @cornerX, @cornerY are the coordinates of the upper left corner in blocks coordinates
//  @inTetrisGame : True = > draw in the tetrisGame, False = > draw "next" piece
//  @specialColourID is the colour to use for the tetramino.
//   If set to COLOUR_ID_NONE the tetramino's colour will be used
//
void tetrisGame::_drawSinglePiece(uint8_t* datas, uint16_t cornerX, uint16_t cornerY, bool inTetrisGame, uint8_t specialColourID) {
    // First visible row ID
    int8_t rowFirst(0);
    if (inTetrisGame) {
        rowFirst = (cornerY > (PLAYFIELD_HEIGHT - 1))?cornerY+1-PLAYFIELD_HEIGHT:0;
    }

    uint16_t x, xFirst(cornerX), y(cornerY - rowFirst), w, h;
    casioDisplay_.shitfToZone(inTetrisGame?ZONE_GAME:ZONE_PREVIEW, xFirst, y, w, h);  // Get coords and dims. in the screen

    uint8_t colourID;
    for (uint8_t row = rowFirst; row < PIECE_HEIGHT; row++) {
        x = xFirst;
        for (uint8_t col = 0; col < PIECE_WIDTH; col++) {
            colourID = datas[row * PIECE_WIDTH + col];
            if (colourID != COLOUR_ID_BOARD) {
                // Draw the block

                // A shadow ?
                if (COLOUR_ID_SHADOW == specialColourID){
#ifdef FX9860G
                // Gray engine on
    #ifdef GRAY_ENGINE_ON
                casioDisplay_.drawRectangle(x, y, w, h, colours_[COLOUR_ID_SHADOW]);
    #else
                // No gray engine => draw borders of shadow
                //casioDisplay_.drawRectangle(x, y, w, h, C_NONE, C_BLACK);
                casioDisplay_.drawBorder(x, y, w, h, C_BLACK);
    #endif // GRAY_ENGINE_ON
#else
                casioDisplay_.drawRectangle(x, y, w, h, colours_[COLOUR_ID_SHADOW]);
#endif // #ifdef FX9860G
                }
                else{
                    casioDisplay_.drawRectangle(x, y, w, h, colours_[(COLOUR_ID_NONE != specialColourID) ? specialColourID : colourID]);
                }
            }
            x += w;
        }
        y += h;
    }
}

// _drawNextPiece() : Display the next piece
//
//  The next piece will be drawn in the preview box.
//  This zone, prior to drawinings, will be erased
//
//  @pieceIndex : index of the piece
//
void tetrisGame::_drawNextPiece(int8_t pieceIndex) {
    // Erase the "previous" next piece
    _eraseNextPiece();

    // ... and then draw the new one
    if (-1 != pieceIndex) {
        _drawSinglePiece(_nextPieceDatas(), 0, 0, false);
    }
}

// _drawTetrisGame() : Draw the tetrisGame
//
void tetrisGame::_drawTetrisGame() {
    uint16_t left, leftFirst(0), top(0), w, h;
    casioDisplay_.shitfToZone(ZONE_GAME, leftFirst, top, w, h);

    // Draw all the blocks (coloured or not)
    for (uint8_t y = 0; y < PLAYFIELD_HEIGHT; y++) {
        left = leftFirst;
        for (uint8_t x = 0; x < PLAYFIELD_WIDTH; x++) {
            casioDisplay_.drawRectangle(left, top, w, h, colours_[playField_[y][x]]);
            left += w;
        }
        top -= h;
    }
}

// _eraseNextPiece() : Erase the "next piece" tetramino
//
//  @left, @top : top left corner of next-piece preview
//  @width, @height : dimensions of the preview (in box units)
//  @colourID : ID of the colour to use
//
void tetrisGame::_eraseNextPiece(){
    uint16_t x(0), y(0), w, h;
    casioDisplay_.shitfToZone(ZONE_PREVIEW, x, y, w, h);
    casioDisplay_.drawRectangle(x, y, w * PIECE_WIDTH, h * PIECE_HEIGHT, colours_[COLOUR_ID_BOARD], colours_[COLOUR_ID_BOARD]);
}

// _drawBackGround() : Draw entire background
//
//  This methods will redraw all the window except the next piece preview and
//  the tetris game playfield
//
void tetrisGame::_drawBackGround(){
    // Border around the playfield
    casioDisplay_.drawBorder(casioDisplay_.playfield()->pos.x - CASIO_BORDER_GAP,
        casioDisplay_.playfield()->pos.y - CASIO_BORDER_GAP,
        casioDisplay_.playfield()->pos.w, casioDisplay_.playfield()->pos.h, colours_[COLOUR_ID_BORDER]);

    // Border for 'Next piece'
    casioDisplay_.drawBorder(casioDisplay_.nextPiece()->pos.x - CASIO_BORDER_GAP,
                casioDisplay_.nextPiece()->pos.y - CASIO_BORDER_GAP,
                casioDisplay_.nextPiece()->pos.w + 1, casioDisplay_.nextPiece()->pos.h + 1, colours_[COLOUR_ID_BORDER]);
}

// _drawNumValue() : Draw a value and its name
//
//  @index of the VALUE object to be drawn
//
void tetrisGame::_drawNumValue(uint8_t index){
    
    // No room for indicators on BW calcs in rotated mode !
#ifdef FX9860G
    if (casioDisplay_.isRotated()){
        return;
    }
#endif // #ifdef FX9860G
    
    char valStr[MAX_VALUE_NAME_LEN + 1];

    // Erase previous value ?
    if (-1 != values_[index].previous){
        playArea::__valtoa(values_[index].previous, values_[index].name, valStr);

        casioDisplay_.dtext(casioDisplay_.textsPos_[index].x, casioDisplay_.textsPos_[index].y, colours_[COLOUR_ID_BKGRND], valStr);
    }

    // print new value
    playArea::__valtoa(values_[index].value, values_[index].name, valStr);
    casioDisplay_.dtext(casioDisplay_.textsPos_[index].x, casioDisplay_.textsPos_[index].y, colours_[COLOUR_ID_TEXT], valStr);

    values_[index].previous = values_[index].value; // to erase the value next time
}

// _scores2List() : Transfer file content to the list os scores
//
//  @data : Buffer read from scores file
//  @scores : List
//
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

// _scores2List() : Transfer list of scores to a buffer
//
//  @scores : List
//  @data : Destination buffer
//
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

// EOF
