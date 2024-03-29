//----------------------------------------------------------------------
//--
//--    sList.h
//--
//--            Definition of sList objet
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#ifndef __GEE_TETRIS_SIMPLE_LIST_h__
#define __GEE_TETRIS_SIMPLE_LIST_h__    1

#include <cstdint>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

// Scores
//

// Filename
#ifdef DEST_CASIO_CALC
#define SCORES_FILENAME u"\\\\fls0\\.geeTetris.scores"
#else
#define SCORES_FILENAME "/home/jhb/.geeTetris.scores"
#endif // #ifdef DEST_CASIO_CALC

// # of scores in file
#ifdef FX9860G
#define MAX_SCORES      5
#else
#define MAX_SCORES      10
#endif // #ifdef FX9860G

// Size in bytes of a score
#ifdef FX9860G
#define SIZE_SCORE      6   // (sizeof(int32_t) + sizeof(int16_t))
#else
#define SIZE_SCORE      8   // (sizeof(int32_t) + 2*sizeof(int16_t))
#endif // #ifdef FX9860G

// Size of the file
#define SIZE_SCORES_FILE    (SIZE_SCORE*MAX_SCORES)

// Size of the Window
#ifdef FX9860G
#define WIN_X       9
#define WIN_WIDTH   110
#define WIN_HEIGHT  60
#else
#define WIN_X       20
#define WIN_WIDTH   190
#define WIN_HEIGHT  140
#endif // #ifdef FX9860G

//----------------------------------------------------------------------
//--
//-- sList
//--
//--    A score list. Items are automatically ordered in descending order
//--
//----------------------------------------------------------------------
class sList{
    public:

    // a score
    typedef struct _record{

        // Score and other informations
        uint32_t score;
        uint16_t lines;
#ifndef FX9860G
        uint16_t level;     // Level aren't stored on FX9860G
#endif // #ifndef FX9860G
    }RECORD;

    // Internal node
    typedef struct _node{
        // Set values
        void setValues(uint32_t escore = 0, uint16_t elines = 0,
                uint16_t elevel = 0);

        // compare() : Comparison between current node and other node
        //
        //  @other : pointer to a _node object
        //
        //  @return comparaison status :
        //      -2 : error
        //      -1 : *other < *this
        //       0 : deep equal
        //       1 : *other > *this
        //
        int8_t compare(_node* other);

        // The score
        _record record;

        // Next node
        _node* next;
    }NODE,* PNODE;

    // Public methods
    //
    public:

        // Construction
        sList(){
            head_ = tail_ = NULL;    // The list is empty
        }

        // Destruction
        ~sList(){
            clear();
        }

        // Add a score
        bool add(uint32_t score, uint16_t lines = 0, uint16_t level = 0);
        bool append(uint32_t score, uint16_t lines = 0, uint16_t level = 0);

        // Access
        PNODE head(){
            return head_;
        }

        // Empty the list
        void clear();

    // Members
    //
    protected:
        PNODE   head_;      // Head of the list
        PNODE   tail_;      // tail of the list
};


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_SIMPLE_LIST_h__

// EOF
