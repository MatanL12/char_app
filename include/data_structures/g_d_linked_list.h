#ifndef __G_D_LINKED_LIST_H__
#define __G_D_LINKED_LIST_H__

#include <stddef.h> /* size_t */

typedef struct List List;

typedef enum ListResult
{
    LIST_SUCCESS,
    LIST_UNINITIALIZED_ERROR, /**< Uninitialized list 					 	*/
    LIST_ALLOCATION_ERROR,    /**< Node allocation failed due to heap error   */
    LIST_NULL_ELEMENT_ERROR,
    LIST_UNDERFLOW_EROOR
    /* Add more as needed by your implementation */

} ListResult;

typedef void (*DestroyItem)(void *_element);

/**
 * @brief Create a list
 *
 * @returns a pointer to the created list.
 * @retval NULL on failure due to allocation failure
 */
List *ListCreate(void);

/**
 * @brief Destroy list
 * @details Destroys the list completely
 *          optionally destroys elements using user provided function
 *
 * @params[in] _pList : A pointer to previously created List returned via ListCreate
 *					   on completion *_pList will be null
 * @params[in] _elementDestroy : A function pointer to be used to destroy elements inserted into the list
 *             or a null if no such destroy is required
 */
void ListDestroy(List **_list, DestroyItem _itemDestroy);

/**
 * @brief Add element to head of list
 * @details time complexity: O(1).
 *
 * @params _list A previously created List ADT returned via ListCreate
 * @params _item An item to add to the list
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_ALLOCATION_ERROR on memory allocation failure
 */
ListResult ListPushHead(List *_list, void *_item);

/**
 * @brief Add element to the list's tail
 * @details time complexity O(1).
 *
 * @params _list A previously created List ADT returned via ListCreate
 * @params _item An item to add to the list
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized
 * @retval LIST_NULL_ELEMENT_ERROR  if _item is NULL
 * @retval LIST_ALLOCATION_ERROR on memory allocation failure
 */
ListResult ListPushTail(List *_list, void *_item);

/** @brief Remove element from list's head
 *  @details if successfull, stores a pointer to the removed item in _item
 *  time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store a pointer to the removed item in
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized or if _pItem is NULL
 * @retval LIST_UNDERFLOW_EROOR if the list is empty
 */
ListResult ListPopHead(List *_list, void **_pItem);

/** @brief Remove element from list's tail
 *  @details time complexity O(1).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @params _pItem: To store a pointer to the removed item in
 * @returns error code
 * @retval LIST_SUCCESS on success
 * @retval LIST_UNINITIALIZED_ERROR if the list is not initialized or if _pItem is NULL
 * @retval LIST_UNDERFLOW_EROOR if the list is empty
 */
ListResult ListPopTail(List *_list, void **_pItem);

/** @brief Get number of elements in the list
 * Average time complexity O(n).
 *
 * @params _list : A previously created List ADT returned via ListCreate
 * @returns - number of elements in the list
 * @retval - return 0 in case that _list is NULL
 */
size_t ListSize(const List *_list);

#endif /* __G_D_LINKED_LIST_H__ */