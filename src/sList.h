//---------------------------------------------------------------------------
//--
//--	File	: sList.h
//--
//--	Author	: Jérôme Henry-Barnaudière - GeeHB
//--
//--	Project	: geeTetris - cpp version
//--
//---------------------------------------------------------------------------
//--
//--	Description:
//--
//--			Definition of sList objet
//--                A (very) simple linked list for scores.
//--                Since this list will only have around ten elements,
//--                we don't need to use std lists
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TETRIS_SIMPLE_LIST_h__
#define __GEE_TETRIS_SIMPLE_LIST_h__    1

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif // #ifdef __cplusplus

//---------------------------------------------------------------------------
//--
//-- sList
//--
//--    A score list. Items are automatically ordered in descending order
//--
//---------------------------------------------------------------------------
class sList{
    public:

    // Internal node
    typedef struct _node{
        // Construction
        _node(uint16_t escore = 0, uint16_t elines = 0, uint16_t elevel = 0);

        // Comparison
        //  -2 : error
        //  -1 : *other < *this
        //   0 : deep equal
        //   1 : *other > *this
        //
        int8_t compare(_node* other);

        // Score and other informations
        uint16_t score;
        uint16_t lines;
        uint16_t level;

        // Nzxt node
        _node* next;
    }NODE,* PNODE;

    // Public methods
    //
    public:

        // Construction
        sList(){
            head_ = tail_ = nullptr;    // The list is empty
        }

        // Destruction
        ~sList(){
            clear();
        }

        // Add a score
        bool add(uint16_t score, uint16_t lines = 0, uint16_t level = 0);
        bool append(uint16_t score, uint16_t lines = 0, uint16_t level = 0);

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
        PNODE	tail_;		// tail of the list
};


#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // __GEE_TETRIS_SIMPLE_LIST_h__

// EOF
