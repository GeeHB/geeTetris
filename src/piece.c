//----------------------------------------------------------------------
//--
//--    piece.c
//--
//--            Implementation of piece & pieceStatus objects :
//--            a tetramino and all the informations for its drawing
//--
//----------------------------------------------------------------------

#include "piece.h"
#include "consts.h"

#include <string.h>

//
// pieceStatus
//   All the informations concerning a tetramino :
//   index, rotation, position ...
//

BOOL pieceStatus_init(PPIECESTATUS const status){
    if (status){
        status->valid = FALSE;
        status->index = -1;
        status->leftPos = status->topPos  = 0;
        status->rotationIndex = 0;
        status->shadowTopPos = -1;

    }
    return FALSE;
}

BOOL pieceStatus_copy(PPIECESTATUS const status, PPIECESTATUS const other){
    if (status != other && status && other) {
        status->index = other->index;
        status->leftPos = other->leftPos;
        status->topPos = other->topPos;
        status->rotationIndex = other->rotationIndex;
        status->shadowTopPos = other->shadowTopPos;

        status->valid = TRUE;
        return TRUE;
    }

    return FALSE;
}

BOOL pieceStatus_equal(PPIECESTATUS const status, PPIECESTATUS const other){
    return (status != other && status && other &&
            status->index == other->index &&
            status->leftPos == other->leftPos &&
            status->topPos == other->topPos &&
            status->rotationIndex == other->rotationIndex);
}

//
//  piece
//
//      a tetramino and all it's rotation states
//

// piece_init() : Intialize piece's members
//
//  @piece : pointer to a piece
//
//  @return : TRUE if the rotation has been successfully added
//
BOOL piece_init(PPIECE const piece){
    if (piece){
        piece->points = NULL;  // No rotation
        piece->maxAllocated = 0;
        piece->rotateID = 0;
        piece->maxRotate = 0;
        piece->vertOffset = -1;

        return TRUE;
    }

    return FALSE;
}

// piece_addRotation() : Add a new piece from a template
//
//  @piece : pointer to a piece
//  @tempPiece : List a strings defining a tetramino from a template
//
//  @return : TRUE if the rotation has been successfully added
//
BOOL piece_addRotation(PPIECE const piece, const char* tempPiece){
    if (!piece || !tempPiece) {
        return FALSE;
    }

    // Create an empty piece
    uint8_t* newRotation = (uint8_t*)malloc(PIECE_SIZE * sizeof(uint8_t));
    if (NULL == newRotation) {
        return FALSE;
    }

    // By default, no colour (ie. the board colour)
    memset(newRotation, COLOUR_ID_BOARD, PIECE_SIZE);

    // Transfer values
    BOOL cont = TRUE;
    char car = 0;
    uint8_t pos = 0;
    for (uint8_t li = 0; cont && li < PIECE_HEIGHT; li++) {
        for (uint8_t col = 0; cont && col < PIECE_WIDTH; col++) {
            car = tempPiece[pos++];
            if (car){
                if (car != TEMPLATE_EMPTY_CHAR) {
                    newRotation[li * PIECE_WIDTH + col] = car - '0';
                }
            }
            else {
                // String is too short
                cont = FALSE;
            }
        }
    }

    // Added ?
    if (FALSE == piece_int_addRotation(piece, newRotation)) {
        free(newRotation);
        return FALSE;
    }

    // First piece => compute vertical offset
    if (1 == piece->maxRotate) {
        piece->vertOffset = PIECE_HEIGHT - 1; // On line visible
        while (piece_int_isLineEmpty(piece, 0, piece->vertOffset) && piece->vertOffset > 0) {
            piece->vertOffset -= 1;
        }
    }

    // Success
    return TRUE;
}

// clear() : Clear the current piece
//
//  @piece : pointer to a piece
//
void piece_clear(PPIECE const piece){
    if (piece){
        if (piece->points) {
            // Free the differents pieces (rotations)
            for (uint8_t index = 0; index < piece->maxRotate; index++) {
                free(piece->points[index]);
            }

            // Free the array
            free(piece->points);
            piece->points = NULL;
        }

        // Ensures all datas are initialized
        piece_init(piece);
    }
}

// piece_rotateLeft() : Trigonometric rotation
//
//  @piece : Pointer to a piece
//
//  @return : rotation ID or -1 if error
//
int8_t piece_rotateLeft(PPIECE const piece){
    if (piece){
        // 360° ?
        if ((++piece->rotateID) >= piece->maxRotate){
            piece->rotateID = 0;
        }

        return piece->rotateID;
    }

    return -1;
}

// piece_rotateRight() : Clockwise rotation
//
//  @piece : Pointer to a piece
//
//  @return : rotation ID or -1 if error
//
int8_t piece_rotateRight(PPIECE const piece){
    return (piece?piece->rotateID = (!piece->maxRotate?0: (piece->rotateID == 0 ? piece->maxRotate - 1 : piece->rotateID - 1)):-1);
}

// piece_int_addRotation() : Add a rotation to the current piece
//
//  @piece : pointer to a piece
//  @tempPiece : List of blocks
//
//  return : TRUE if the rotation is valid and has been successfully added
//
BOOL piece_int_addRotation(PPIECE const piece, uint8_t* tempPiece){
    if (piece || !tempPiece) {
        return FALSE;
    }

    // Size the "points" buffer
    if (piece->maxRotate >= piece->maxAllocated) {
        // New size
        piece->maxAllocated += ALLOCATE_STEP;
        piece->points = (uint8_t**)realloc(piece->points, piece->maxAllocated * sizeof(uint8_t*));
        if (NULL == piece->points) {
            return FALSE;
        }
    }

    // Copy ...
    piece->points[piece->maxRotate++] = tempPiece;
    return TRUE;
}

// piece_int_isLineEmpty(): Is the "line" empty ?
//
//  @piece : pointer to a piece
//  @rotIndex : Index of the rotation to check
//  @lineIndex : Index of the line in the tetramino
//
//  Returns true if the tested line is empty (ie. no colored block)
//
BOOL piece_int_isLineEmpty(PPIECE const piece, uint8_t rotIndex, uint8_t lineIndex){
    if (!piece || rotIndex >= piece->maxRotate || lineIndex >= PIECE_HEIGHT) {
        return FALSE;
    }

    uint8_t total = 0;
    for (uint8_t col = 0; !total && col < PIECE_WIDTH; col++) {
        total += piece->points[rotIndex][lineIndex * PIECE_WIDTH + col];
    }

    // Yes !
    return (total?FALSE:TRUE);    // Sum of colors == 0 => empty line
}

// EOF
