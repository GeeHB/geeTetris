//----------------------------------------------------------------------
//--
//--    scores.c
//--
//--            Scores management
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#include "scores.h"

#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

//
//   scores_
//
//      A node with a score
//

// Set value// node_setValues : Set noode values
//
//  @node : pointer to the node
//  @escore, @elines, @elevel : score informations
//
void node_setValues(PSCORENODE const node, uint32_t escore, uint16_t elines, uint16_t elevel){
    if (node){
        node->score.score = escore;
        node->score.lines = elines;
        node->score.level = elevel;
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
int8_t node_compare(PSCORENODE const left, PSCORENODE const right){
    if (!left || !right){
        return -2;
    }

    if (right->score.score < left->score.score
        || (right->score.score == left->score.score &&
            right->score.lines < left->score.lines)
        || (right->score.score == left->score.score &&
            right->score.lines == left->score.lines &&
            right->score.level < left->score.level )
        ){
        return -1;
    }

    if (right->score.score > left->score.score
        || (right->score.score == left->score.score &&
            right->score.lines > left->score.lines)
        || (right->score.score == left->score.score &&
            right->score.lines == left->score.lines &&
            right->score.level > left->score.level )
        ){
        return 1;
    }

    // Deep equal
    return 0;
}

//
//   scores
//
//  A score list. Items are automatically ordered in descending order
//

// scores_init() : Initializes a score list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully initialized
//
BOOL scores_init(PSCORES const sList){
    if (sList){
        sList->head = sList->tail = NULL;
    }

    return FALSE;
}

// scores_clear() : Empty the list
//
//  @sList : pointer to the list
//
void scores_clear(PSCORES const sList){
    if (sList){
        PSCORENODE pnode, next;

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

// scores_load() : Load a stored scores'list
//
//  @filename : Name of the file containing the scores
//  @sList : pointer to the list
//
//  @return : count of items loaded, -1 if error
//
int scores_load(const char* filename, PSCORES const sList){
    int count = -1, total = 0;
    if (filename && sList){
        size_t fSize, bufferSize = SIZE_SCORES_FILE;
        PSCORE score, buffer = (PSCORE)malloc(bufferSize);
        if (buffer){
            int fd = open(filename, O_RDONLY | O_CREAT);
            if (fd != -1){
                memset(buffer, 0, bufferSize);
                if ((fSize = read(fd, (void*)buffer, bufferSize)) > 0){
                    scores_clear(sList);    // Empty the list

                    count = (int)fSize / SIZE_SCORE;
                    for (int id =0; id < count; id++){
                        score = &buffer[id];
                        if (scores_add(sList, score->score, score->lines, score->level)){
                            total++;
                        }
                    }

                    count = total;
                }
                close(fd);
            } // if (fd != -1)

            free(buffer);
        } // if buffer
    }

    return count;
}

// scores_save() : Save scores in a file
//
//  @filename : Name of the file to save file in
//  @sList : pointer to the list
//  @max : Max. count of items to store
//
//  @return : count of items saved, -1 if error
//
int scores_save(const char* filename, PSCORES const sList, uint max){
    int count = -1;
    if (filename && sList && sList->head && max){
        int fd = open(filename, O_WRONLY | O_CREAT);
        if (fd != -1){
            PSCORENODE node = sList->head;
            count = 0;
            while (node && count < (int)max){
                write(fd, &node->score, SIZE_SCORE);
                count++;
                node = node->next;
            }

            close(fd);
        } // if (fd != -1)
    }

    return count;
}

// scores_add() : Add a score
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL scores_add(PSCORES const sList, uint32_t score, uint16_t lines, uint16_t level){
    if (sList){
        PSCORENODE current, prev, node;

        current = sList->head;
        prev = NULL;

        // Create free node with the values
        node = (PSCORENODE)malloc(sizeof(SCORENODE));
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

/*
// scores_append() : Append a score to the list
//
//  @sList : pointer to the list
//
//  @return : TRUE if successfully added to the list
//
BOOL scores_append(PSCORES const sList, uint32_t score, uint16_t lines, uint16_t level){
    if (sList){
        PSCORENODE node = (PSCORENODE)malloc(sizeof(SCORENODE));
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
*/

// EOF
