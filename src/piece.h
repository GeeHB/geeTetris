//----------------------------------------------------------------------
//--
//--    piece.h
//--
//--
//--            Definition of piece & pieceStatus objects :
//--                a tetramino and all the informations for its drawing
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_PIECE_h__
#define __GEE_TETRIS_PIECE_h__    1

#include "shared/casioCalcs.h"
#include "templates.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Tetramino max dims
//

#define PIECE_WIDTH     4
#define PIECE_HEIGHT    PIECE_WIDTH

// Tetramino size in byte
#define PIECE_SIZE      (PIECE_WIDTH * PIECE_HEIGHT)

// Allocation step
#define ALLOCATE_STEP   4

//
// pieceStatus
//   All the informations concerning a tetramino :
//   index, rotation, position ...
//
typedef struct _pieceStatus{
    BOOL valid;
    int8_t index;      // Index of the piece (-1 = don't draw)
    int8_t leftPos;    // Initial pos.
    uint8_t topPos;
    uint8_t rotationIndex;     // ID of current rotation
    int8_t shadowTopPos;       // yPos of shadow (-1 = no shadow)
} pieceStatus, PIECESTATUS, * PPIECESTATUS;


BOOL pieceStatus_init(PPIECESTATUS const status);
BOOL pieceStatus_copy(PPIECESTATUS const status, PPIECESTATUS const other);
BOOL pieceStatus_equal(PPIECESTATUS const status, PPIECESTATUS const other);

//
//  piece
//
//      a tetramino and all it's rotation states
//

typedef struct _piece{
    uint8_t** points;      // Matrix of the piece whith all possible rotations (each rotation is a line)
    uint8_t rotateID;      // rotation index (ie. index of the piece to draw)
    uint8_t maxRotate;     // max. rotation(s) allowed for the piece ( = 360°)
    uint8_t maxAllocated;  // Size of points_ in "rotation" count

    int8_t vertOffset;     // Initial vert. offset (ie. count of empty lines starting from bottom)
    uint8_t colourIndex;   // Colour ID (0 = invisible)
} piece, PIECE, * PPIECE;

// piece_init() : Intialize piece's members
//
//  @piece : pointer to a piece
//
//  @return : TRUE if the rotation has been successfully added
//
BOOL piece_init(PPIECE const piece);

// piece_rotateLeft() : Trigonometric rotation
//
//  @piece : Pointer to a piece
//
//  @return : rotation ID or -1 if error
//
int8_t piece_rotateLeft(PPIECE const piece);

// piece_rotateRight() : Clockwise rotation
//
//  @piece : Pointer to a piece
//
//  @return : rotation ID or -1 if error
//
int8_t piece_rotateRight(PPIECE const piece);

// piece_addRotation() : Add a new piece from a template
//
//  @piece : pointer to a piece
//  @tempPiece : List a strings defining a tetramino from a template
//
//  @return : TRUE if the rotation has been successfully added
//
BOOL piece_addRotation(PPIECE const piece, const char* tempPiece);

// clear() : Clear the current piece
//
//  @piece : pointer to a piece
//
void piece_clear(PPIECE const piece);

// piece_int_addRotation() : Add a rotation to the current piece
//
//  @piece : pointer to a piece
//  @tempPiece : List of blocks
//
//  return : TRUE if the rotation is valid and has been successfully added
//
BOOL piece_int_addRotation(PPIECE const piece, uint8_t* tempPiece);

// piece_int_isLineEmpty(): Is the "line" empty ?
//
//  @piece : pointer to a piece
//  @rotIndex : Index of the rotation to check
//  @lineIndex : Index of the line in the tetramino
//
//  Returns true if the tested line is empty (ie. no colored block)
//
BOOL piece_int_isLineEmpty(PPIECE const piece, uint8_t rotIndex, uint8_t lineIndex);


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_PIECE_h_

// EOF
