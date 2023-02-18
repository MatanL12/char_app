#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stddef.h> /* size_t */

/**
 *  @file HashMap.h
 *  @brief Generic Hash map of key-value pairs implemented with separate chaining using linked lists.
 *
 *  @details  The hash map (sometimes called dictionary or associative array)
 *  is a set of distinct keys (or indexes) mapped (or associated) to values.
 *  size of allocated table will be the nearest prime number greater than requestedcapacity
 *  Lists used for chaining will be allocated lazely.
 *
 */

typedef struct HashMap HashMap;

typedef enum Map_Result
{
    MAP_SUCCESS = 0,
    MAP_UNINITIALIZED_ERROR, /**< Uninitialized map error 	*/
    MAP_KEY_NULL_ERROR,      /**< Key was null 				*/
    MAP_KEY_DUPLICATE_ERROR, /**< Duplicate key error 		*/
    MAP_KEY_NOT_FOUND_ERROR, /**< Key not found 		    */
    MAP_ALLOCATION_ERROR     /**< Allocation error 	 		*/
} Map_Result;

/**
 * @brief hashing function for keys
 */
typedef size_t (*HashFunction)(void *_key);

/**
 * @brief equality check function for keys - return 0 if both keys are equal, and 1 if not.
 */
typedef int (*EqualityFunction)(void *_firstKey, void *_secondKey);

/**
 * @brief function pointer to be invoked for each element - return 0 to stop the iteration, and 1 to continue.
 */
typedef int (*KeyValueActionFunction)(const void *_key, void *_value, void *_context);

/**
 * @brief Create a new hash map with given capcity and key characteristics.
 * @param[in] _capacity - Expected max capacity. Must be greater than 4.
 * 						  shall be rounded to nearest larger prime number.
 * @param[in] _hashFunc - hashing function for keys
 * @param[in] _keysEqualFunc - equality check function for keys.
 * @return newly created map or null on failure
 */
HashMap *HashMapCreate(size_t _size, HashFunction _hashFunc, EqualityFunction _keysEqualFunc);

/**
 * @brief destroy hash map and set *_map to null
 * @param[in] _map : map to be destroyed
 * @param[optional] _keyDestroy : pointer to function to destroy keys. can be NULL
 * @param[optional] _valDestroy : pointer to function to destroy values. can be NULL
 * @details optionally destroy all keys and values using user provided functions
 */
void HashMapDestroy(HashMap **_map, void (*_keyDestroy)(void *_key), void (*_valDestroy)(void *_value));

/**
 * @brief Adjust map capacity and rehash all key/value pairs
 * @param[in] _map - exisiting map
 * @param[in] _newCapacity - new capacity shall be rounded to nearest larger prime number.
 * @return MAP_SUCCESS or MAP_ALLOCATION_ERROR
 */
Map_Result HashMapRehash(HashMap *_map, size_t newCapacity);

/**
 * @brief Insert a key-value pair into the hash map (using ListPushHead()).
 * @param[in] _map - Hash map adt to insert to, must be initialized
 * @param[in] _key - key to serve as index
 * @param[in] _value - The value to associate with the key
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_DUPLICATE_ERROR	if key alread present in the map
 * @retval  MAP_KEY_NULL_ERROR if _key is NULL
 * @retval  MAP_ALLOCATION_ERROR on failer to allocate key-value pair
 * @retval  MAP_UNINITIALIZED_ERROR if _map is NULL
 *
 * @warning key must be unique and destinct
 */
Map_Result HashMapInsert(HashMap *_map, const void *_key, const void *_value);

/**
 * @brief Remove a key-value pair from the hash map.
 * @param[in] _map - Hash map to remove pair from, must be initialized
 * @param[in] _key - key to serve as index to locate _value.
 * @param[in] _pKey  - pointer to variable to recieve key from map if found. Cann't be NULL.
 * @param[in] _pValue - pointer to variable that will get the value if found. Can be NULL.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR if _searchKey or _pKey is NULL
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR if _map is NULL
 *
 * @warning key must be unique and destinct
 */
Map_Result HashMapRemove(HashMap *_map, const void *_searchKey, void **_pKey, void **_pValue);

/**
 * @brief Find a value by key
 * @param[in] _map - Hash map to find pair in, must be initialized
 * @param[in] _key - key to serve as index
 * @param[in] _pValue - pointer to variable that will get the value if found. can be NULL.
 * @return Success indicator
 * @retval  MAP_SUCCESS	on success
 * @retval  MAP_KEY_NULL_ERROR if _key is NULL
 * @retval  MAP_KEY_NOT_FOUND_ERROR if key not found
 * @retval  MAP_UNINITIALIZED_ERROR if _map is NULL
 *
 * @warning key must be unique and destinct
 */
Map_Result HashMapFind(const HashMap *_map, const void *_key, void **_pValue);

/**
 * @brief Get number of key-value pairs inserted into the hash map
 * @param[in] _map - Hash map to get size of, must be initialized
 * @return return the number of elements in _map
 * @return return 0 if _map isn't initialized
 * @warning complexity can be O(?)
 */
size_t HashMapSize(const HashMap *_map);

/**
 * This method is optional in the homewor
 * @brief Iterate over all key-value pairs in the map.
 *                   Iteration will stop if _act returns a zero for a given pair
 * @details The user provided KeyValueActionFunction _action will be called for each element.
 * @param[in] _map - Hash map to iterate over.
 * @param[in] _action - User provided function pointer to be invoked for each element
 * @param[in] _context - can be NULL
 * @returns number of times the user functions was invoked or 0 if _map or _action is NULL
 */
size_t HashMapForEach(const HashMap *_map, KeyValueActionFunction _action, void *_context);

#ifndef NDEBUG

typedef struct Map_Stats
{
    size_t numberOfBuckets;    /* num of lists we created */
    size_t numberOfChains;     /* is equal to number of elements */
    size_t maxChainLength;     /* */
    size_t averageChainLength; /* number of elem / numberOfBuckets */
} Map_Stats;

Map_Stats HashMap_GetStatistics(const HashMap *_map);

#endif /* NDEBUG */

#endif /* __HASH_MAP_H__ */
