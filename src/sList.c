//----------------------------------------------------------------------
//--
//--    sList.c
//--
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#include "sList.h"

//
//   tetrisNode
//
//      A node with a score
//

// Set value// node_setValues : Set noode values
//
//  @node : pointer to the node
//  @escore, @elines, @elevel : score informations
//
void node_setValues(PTETRISNODE const node, uint32_t escore, uint16_t elines, uint16_t elevel){
    if (node){
        node->score = escore;
        node->lines = elines;
        node->level = elevel;
        node->next = NULL;
    }
}

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
int8_t node_compare(PTETRISNODE const left, PTETRISNODE const right){
    if (!left || !right){
        return -2;
    }

    if (right->score < left->score
        || (right->score == left->score &&
            right->lines < left->lines)
        || (right->score == left->score &&
            right->lines == left->lines &&
            right->level < left->level )
        ){
        return -1;
    }

    if (right->score > left->score
        || (right->score == left->score &&
            right->lines > left->lines)
        || (right->score == left->score &&
            right->lines == left->lines &&
            right->level > left->level )
        ){
        return 1;
    }

    // Deep equal
    return 0;
}

//
//   sList
//
//  A score list. Items are automatically ordered in descending order
//

// sList_init() : Initialize the list
//
//  @sList : pointer to the list
//
//  @return : TRUE if done
//
BOOL sList_init(PSLIST const sList){
    if (sList){
        sList->head = sList->tail = NULL;
    }

    return FALSE;
}

// sList_clear() : Empty the list
//
//  @sList : pointer to the list
//
void sList_clear(PSLIST const sList){
    if (sList){
        PTETRISNODE pnode, next;

        pnode = sList->head;
        next = NULL;

        while (pnode){
            next = pnode->next;
            free(pnode);
            pnode = next;
        }

        sList->head = sList->tail = NULL;
    }
}

// sList_add() : Add a score
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL sList_add(PSLIST const sList, uint32_t score, uint16_t lines, uint16_t level){
    if (sList){
        PTETRISNODE current, prev, node;

        current = sList->head;
        prev = NULL;

        // Create free node with the values
        node = (PTETRISNODE)malloc(sizeof(TETRISNODE));
        if (!node){
            return FALSE;
        }
        node_setValues(node, score, lines, level);

        // Search for its position
        int8_t c;
        while (current && (c = node_compare(node, current)) > 0 ){
            prev = current;
            current = current->next;
        }

        // tail ?
        if (!current){
            if (!prev){
                // ... and head
                sList->head = node;
            }
            else{
                prev->next = node;
            }

            sList->tail = node;
        }
        else{
            // Deep equal ???
            if (!c){
                free(node);
                return FALSE;
            }

            // Insert
            if (prev){
                current = prev->next;
                prev->next = node;
                node->next = current;
            }
            else{
                // new head
                node->next = sList->head;
                sList->head = node;
            }
        }

        return TRUE;
    }

    return FALSE;
}

// sList_append() : Append a score to the list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL sList_append(PSLIST const sList, uint32_t score, uint16_t lines, uint16_t level){
    if (sList){
        PTETRISNODE node = (PTETRISNODE)malloc(sizeof(TETRISNODE));
        if (!node){
            return FALSE;
        }

        node_setValues(node, score, lines, level);

        // Append to the tail
        if (sList->tail){
            sList->tail->next = node;
        }
        else{
            // First node => tail = head = new node
            sList->head = node;
        }

        // Done
        sList->tail = node;       // new tail
        return TRUE;
    }

    return FALSE;
}

// EOF
