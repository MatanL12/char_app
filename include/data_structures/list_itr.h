/*#include "internal_list_itr.h"*/

#ifndef __LIST_ITR_H__
#define __LIST_ITR_H__

#include "g_d_linked_list.h"
#define LIST_ITR_NOT_INITIAILIZED -1

typedef void *ListItr;

/**
 * @brief Get itertator to the list's beginning
 *
 * @params _list : list to return begin iterator, pointing at first element
 *                 or at the end if list is empty
 * @return iterator pointing at the list's beginning
 */
ListItr ListItrBegin(const List *_list);

/**
 * @brief Get itertator to the list end
 *
 * @params _list : list to return end iterator
 * @return an iterator pointing at the list's end
 */
ListItr ListItrEnd(const List *_list);

/**
 * @brief retruns none zero if _a and _b iterators refer to same node
 * @warning might be implemented as a macro
 */
int ListItrEquals(const ListItr _a, const ListItr _b);

/**
 * @brief Get itertator to the next element from a given iterator
 * @warning if _itr is end iterator it will be returned
 */
ListItr ListItrNext(ListItr _itr);

/**
 * @brief Get itertator to the previous element
 * @warning if _itr is begin iterator it will be returned
 */
ListItr ListItrPrev(ListItr _itr);

/**
 * @brief Get data from the list node the iterator is pointing to
 *
 * @params _itr : A list iterator
 * @return the data the iterator is pointing at or NULL if pointing to the end
 */
void *ListItrGet(ListItr _itr);

/**
 * @brief Set data at the node where the iterator is pointing at
 * @return the data from the node to be changed
 */
void *ListItrSet(ListItr _itr, void *_element);

/**
 * @brief Inserts a new node before node the iterator is pointing at
 * @return an iterator pointing at the element inserted or NULL on error
 */
ListItr ListItrInsertBefore(ListItr _itr, void *_element);

/**
 * @brief Removes the node the iterator is pointing at
 *
 * @params _itr : A list iterator
 * @return the removed data
 */
void *ListItrRemove(ListItr _itr);

#endif /*__LIST_ITR_H__ */