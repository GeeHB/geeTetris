//----------------------------------------------------------------------
//--
//--    scores.h
//--
//--            Scores management
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_SCORES_h__
#define __GEE_TETRIS_SCORES_h__    1

#include "shared/keys.h"
#include "consts.h"

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Filename
#ifdef DEST_CASIO_CALC
#define SCORES_FILENAME ".geeTetris.scores"
#else
#define SCORES_FILENAME "/home/jhb/.geeTetris.scores"
#endif // #ifdef DEST_CASIO_CALC

#define SCORES_DEF_TITLE    "Best scores"

// # of scores in file
#define MAX_SCORES      10

// Size in bytes of a score
#define SIZE_SCORE      8   // (sizeof(int32_t) + 2*sizeof(int16_t))

// Size of the file
#define SIZE_SCORES_FILE    (SIZE_SCORE*MAX_SCORES)

// Size of the Window
#define WIN_X       20
#define WIN_WIDTH   190
#define WIN_HEIGHT  140

// score
//  Values of a score
//
typedef struct _score{
    uint32_t score;
    uint16_t lines;
    uint16_t level;
}SCORE, * PSCORE;

//   scoreNode
//
//      A node with a score
//
typedef struct _scoreNode{
    SCORE score;
    struct _scoreNode* next;
}SCORENODE, * PSCORENODE;

// node_setValues : Set noode values
//
//  @node : pointer to the node
//  @escore, @elines, @elevel : score informations
//
void node_setValues(PSCORENODE const node, uint32_t escore, uint16_t elines, uint16_t elevel);

// node_compare() : Comparison between current node and other node values
//
//  @left : pointer to the left node
//  @right : pointer to the right node
//
//  @return comparaison status :
//      -2 : error
//      -1 : "right" < "left"
//       0 : deep equal
//       1 : "right" > "left"
//
int8_t node_compare(PSCORENODE const left, PSCORENODE const right);

//
//   scores
//
//  A score list. Items are automatically ordered in descending order
//

typedef struct _scores{
    PSCORENODE   head;
    PSCORENODE   tail;
} SCORES, * PSCORES;


// scores_init() : Initializes a score list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully initialized
//
BOOL scores_init(PSCORES const slist);

// scores_clear() : Empty the list
//
//  @sList : pointer to the list
//
void scores_clear(PSCORES const sList);

// scores_load() : Load a stored scores'list
//
//  @filename : Name of the file containing the scores
//  @sList : pointer to the list
//
//  @return : count of items loaded, -1 if error
//
int scores_load(const char* filename, PSCORES const sList);

// scores_save() : Save scores in a file
//
//  @filename : Name of the file to save file in
//  @sList : pointer to the list
//  @max : Max. count of items to store
//
//  @return : count of items saved, -1 if error
//
int scores_save(const char* filename, PSCORES const sList, uint max);

// scores_add() : Add a score
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL scores_add(PSCORES const sList, uint32_t score, uint16_t lines, uint16_t level);

// scores_append() : Append a score to the list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL scores_append(PSCORES const sList, uint32_t score, uint16_t lines, uint16_t level);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_SCORES_h__

// EOF
