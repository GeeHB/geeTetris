//----------------------------------------------------------------------
//--
//--    sList.cpp
//--
//--            Definition of sList objet
//--                A (very) simple linked list for scores.
//--                Since this list will only have about ten elements,
//--                we don't need to use std lists
//--
//----------------------------------------------------------------------

#include "sList.h"

// Set values
//
void sList::_node::setValues(uint32_t escore, uint16_t elines,
                [[maybe_unused]] uint16_t elevel){
    record.score = escore;
    record.lines = elines;
#ifndef FX9860G
    record.level = elevel;
#endif // #ifndef FX9860G
    next = NULL;
}

// Node comparison
//  -2 : error
//  -1 : *other < *this
//   0 : deep equal
//   1 : *other > *this
//
int8_t sList::_node::compare(_node* other){
    if (NULL == other){
        return -2;
    }

    if (other->record.score < record.score
        || (other->record.score == record.score &&
            other->record.lines < record.lines)
#ifndef FX9860G
        || (other->record.score == record.score &&
            other->record.lines == record.lines &&
            other->record.level < record.level )
#endif // #ifndef FX9860G
        ){
        return -1;
    }

    if (other->record.score > record.score
        || (other->record.score == record.score &&
            other->record.lines > record.lines)
#ifndef FX9860G
        || (other->record.score == record.score &&
            other->record.lines == record.lines &&
            other->record.level > record.level )
#endif // #ifndef FX9860G
        ){
        return 1;
    }

    // Deep equal
    return 0;
}

//----------------------------------------------------------------------
//--
//-- sList
//--
//--    A score list. Items are automatically ordered in descending order
//--
//----------------------------------------------------------------------

// Add a score
//
bool sList::add(uint32_t score, uint16_t lines, uint16_t level){
    sList::PNODE current(head_), prev(NULL), node(NULL);

    node = (sList::PNODE)malloc(sizeof(sList::NODE));
    if (NULL == node){
        return false;   // Memory error
    }

    node->setValues(score, lines, level);

    // Search position
    int8_t c;
    while (current && (c = node->compare(current)) > 0 ){
        prev = current;
        current = current->next;
    }

    // tail ?
    if (!current){
        if (!prev){
            // ... and head
            head_ = node;
        }
        else{
            prev->next = node;
        }

        tail_ = node;
    }
    else{

        // Deep equal ???
        if (!c){
            free(node);
            return false;   // already stored
        }

        // Insert
        if (prev){
            current = prev->next;
            prev->next = node;
            node->next = current;
        }
        else{
            // new head
            node->next = head_;
            head_ = node;
        }
    }

    // Successfully added (or inserted)
    return true;
}

// Append a score
//
bool sList::append(uint32_t score, uint16_t lines, uint16_t level){
    sList::PNODE node = (sList::PNODE)malloc(sizeof(sList::NODE));
    if (NULL == node){
        return false;   // Memory error
    }

    node->setValues(score, lines, level);

    // Append to the tail
    if (tail_){
        tail_->next = node;
    }
    else{
        // First node => tail = head = new node
        head_ = node;
    }

    // Done
    tail_ = node;       // new tail
    return true;
}

// Empty the list
//
void sList::clear(){
    sList::PNODE pnode(head_), next(NULL);

    // browse and delete ...
    while (pnode){
        next = pnode->next;
        free(pnode);
        pnode = next;   // forward
    }

    head_ = tail_ = NULL;    // The list is now empty
}

// EOF
