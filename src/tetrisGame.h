//----------------------------------------------------------------------
//--
//--    tetrisGame.h
//--
//--        Definition of tetrisGame
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_GAME_h__
#define __GEE_TETRIS_GAME_h__   1

#include "consts.h"
#include "piece.h"
#include "templates.h"
#include "playArea.h"
#include "sList.h"
#include "shared/keyboard.h"

#ifdef DEST_CASIO_CALC
#include <gint/timer.h>
#include <gint/clock.h>
#endif // #ifdef DEST_CASIO_CALC

#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cpluscplus

// Timer & game speed
//
#define GAME_TICK_DURATION      25      //  in ms
#define GAME_START_TICKS        48      // Initial piece speed in ticks ( 1200 ms / GAME_TICK_DURATION)

#define GAME_MAX_ACC_LEVEL      15      // No more acceleration when this level is reached
#define GAME_ACC_RATE           175     // Game acceleration in /000

#define GAME_NEXT_LEVEL_MOVES   250     // Level++ after # movements


// Game status
//
enum GAME_STATUS{
    STATUS_READY     = 1,
    STATUS_RUNNING   = 2,
    STATUS_STOPPED   = 8,
    STATUS_CANCELED  = 16
};

//----------------------------------------------------------------------
//--
//-- tetrisGame object
//--
//--    Handle the gameplay and the game(without display !)
//--
//----------------------------------------------------------------------

class tetrisGame {

    // Public methods
    //
    public:

        // Construction
        tetrisGame(tetrisParameters* params);

        // Destruction
        ~tetrisGame() {
            end();
        }

        // setParameters() : Set game's parameters
        //
        //  @params : Struct. containining parameters for the game
        //    These parameters are choosen by the user
        //
        void setParameters(tetrisParameters* params);

        // start() : Start the tetris game
        //
        //  The entire game is handled by this method.
        //  It retuns on error or when the game is over
        //
        //  @return :  false on error(s) or if canceled by user
        //
        bool start();

        // Pause or resume the game
#ifdef FXCG50
        void pause();
#endif // FXCG50

        // Status
        //
        bool isRunning() {
            return STATUS_RUNNING == status_;
        }

        // Cancel the game
        void cancel() {
            // Already escaped ?
            if (!isCancelled()) {
                status_ |= STATUS_CANCELED;
            }
        }

        // Escaped / canceled by the user ?
        bool isCancelled() {
            return ((status_ & STATUS_CANCELED) == STATUS_CANCELED);
        }

        // Force the end of the game
        void end() {
            status_ |= STATUS_STOPPED;
        }

        // Update the display
        void updateDisplay(){
#ifdef DEST_CASIO_CALC
            dupdate();
#endif // #ifdef DEST_CASIO_CALC
        }

        //
        // Access
        //

        // Current score
        int32_t score(){
            return values_[SCORE_ID].value;
        }

        // Completed lines
        int32_t lines(){
            return values_[COMPLETED_LINES_ID].value;
        }

        // Current level
        int32_t level(){
            return values_[LEVEL_ID].value;
        }

       // _showScores() : Show best scores and current one (if in the list)
        //
        //  @score : new score. If equal to -1, the bests scores are shown.
        //  @lines : # completed lines
        //  @level : end level
        //
        static void showScores(int32_t score = -1, uint32_t lines = 0, uint32_t level = 0);


    // Internal methods
    //
    private:

        // The tetrisGame is empty ...
        void _emptyTetrisGame() {
            memset(playField_, COLOUR_ID_BOARD, PLAYFIELD_HEIGHT * PLAYFIELD_WIDTH);
        }

        // Change the game speed (in ticks)
        int _getSpeed(int currentTicks, uint8_t incLevel = 1);

        // Handle keyboard events
        void _handleGameKeys();

        //
        // Pieces management
        //

        // Datas of a piece
        uint8_t* _nextPieceDatas() {
            return ((nextIndex_ < 0
                || nextIndex_ >= TETRAMINOS_COUNT) ?
                 nullptr : tetraminos_[nextIndex_].datas(0));
        }
        uint8_t* _pieceDatas(int8_t index, uint8_t  rotIndex) {
            return ((index < 0 || index >= TETRAMINOS_COUNT
                    || rotIndex >= tetraminos_[index].maxRotations()) ?
                    nullptr : tetraminos_[index].datas(rotIndex));
        }

        // New piece (in the game)
        void _newPiece();

        // The position of the piece just changed
        void _piecePosChanged();

        // Get a new index for the next piece
        uint8_t _newPieceIndex() {
            return (rand() % TETRAMINOS_COUNT);
        }

        //
        // Movements
        //
        bool _rotateLeft();
        bool _left();
        bool _right();
        bool _down(bool newPiece = false);
        void _fall();

        // Rotate the display
        void _rotateDisplay(bool first = false);

        // Can the current piece be at the given position ?
        bool _canMove(int8_t leftPos, uint8_t  topPos);

        // Get a piece min.pos.index (vertical value)
        uint8_t _minTopPosition();

        // Add a randomly generated dirty line in the gameplay
        void _addDirtyLine(uint8_t lineID);

        // Clear and remove a completed line
        void _clearLine(uint8_t index);

        // Put the tetramino at the current position
        void _putPiece();

        // The piece is at the lowest possible level
        void _reachLowerPos(uint8_t downRowcount = 0);

        // redraw the whole screen
        void _redraw();

        // Display the next piece
        void _drawNextPiece(int8_t pieceIndex);

        // Erase the "next piece" tetramino
        void _eraseNextPiece();

        // Draw a value and its name
        void _drawNumValue(uint8_t index);

        // Draw entire background
        void _drawBackGround();

        // Draw a tetramino using the given colour
        void _drawSinglePiece(uint8_t* datas, uint16_t cornerX,
                        uint16_t cornerY, bool inTetrisGame = true,
                        uint8_t specialColourID = COLOUR_ID_NONE);

        // Draw the tetrisGame
        void _drawTetrisGame();

        // Lt of scores management
        static void _scores2List(char* data, sList& scores);
        static void _list2Scores(sList& scores, char* data);

#ifdef DEST_CASIO_CALC
        // Callback for game's timer
        static int __callbackTick(volatile int *pTick){
            *pTick = 1;
            return TIMER_CONTINUE;
        }
#endif // #ifdef DEST_CASIO_CALC

    // Members
    //
    protected:

        uint8_t status_; // Game status

        uint8_t playField_[PLAYFIELD_HEIGHT][PLAYFIELD_WIDTH];      // The playfield
        piece   tetraminos_[TETRAMINOS_COUNT];                      // The tetraminos' list

        tetrisParameters parameters_;

        int32_t colours_[LAST_COLOUR_ID+1];     // Colours in rgb

        playArea casioDisplay_;

        // Current iece and next piece
        int8_t nextIndex_;  // -1 = None
        pieceStatus nextPos_, currentPos_;

        // Indicators (and associated names)
        UVALUE  values_[VAL_COUNT];

        keyboard    keyboard_;      // Keyboard handler
};

#ifdef __cplusplus
}
#endif // #ifdef __cpluscplus

#endif // __GEE_TETRIS_GAME_h__

// EOF
