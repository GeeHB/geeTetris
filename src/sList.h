//----------------------------------------------------------------------
//--
//--    sList.h
//--
//--            Definition of sList
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_SIMPLE_LIST_h__
#define __GEE_TETRIS_SIMPLE_LIST_h__    1

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

//
//   tetrisNode
//
//      A node with a score
//

typedef struct _tetrisNode{
    // Score inf.
    uint32_t score;
    uint16_t lines;
    uint16_t level;
    struct _tetrisNode* next;
}TETRISNODE, * PTETRISNODE;

// node_setValues : Set noode values
//
//  @node : pointer to the node
//  @escore, @elines, @elevel : score informations
//
void node_setValues(PTETRISNODE const node, uint32_t escore, uint16_t elines, uint16_t elevel);

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
int8_t node_compare(PTETRISNODE const left, PTETRISNODE const right);

//
//   sList
//
//  A score list. Items are automatically ordered in descending order
//

typedef struct _sList{
    PTETRISNODE   head;
    PTETRISNODE   tail;
} SLIST, * PSLIST;


BOOL sList_init(PSLIST const slist);

// sList_clear() : Empty the list
//
//  @sList : pointer to the list
//
void sList_clear(PSLIST const sList);

// sList_add() : Add a score
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL sList_add(PSLIST const sList, uint32_t score, uint16_t lines, uint16_t level);

// sList_append() : Append a score to the list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL sList_append(PSLIST const sList, uint32_t score, uint16_t lines, uint16_t level);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_SIMPLE_LIST_h__

// EOF
