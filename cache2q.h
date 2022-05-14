#pragma once

#include "queues.h"
#include "hashmap.h"

#define TEST_CODE_2Q 0


///< 2q cache structure with built-in hashtable
typedef struct Cache2Q_t {
    chain_t *fifoInRoot;
    chain_t *fifoOutRoot;
    chain_t *lru2Root;

    hashmap_t *table;
} Cache2Q_t;

/*!
    \brief Сreates an empty 2q cache and hashtable
    \param [in] size  Сache and hashtable size
    \return Pointer to 2q cache structure with built-in hashtable
*/
Cache2Q_t *createCache2Q(unsigned long size);

/*!
    \brief Delete all contents of 2q cache and hashtable
    \param [out] cache  Pointer to 2q cache
*/
void deleteCache2Q(Cache2Q_t *cache);

/*!
    \brief Appends value to 2q cache
    \param [in] value  The value to add to 2q cache
    \param [out] cache  Pointer to 2q cache
    \return Presence of a cache hit
*/
int appendTo2Q(long value, Cache2Q_t *cache);

/*!
    \brief Appends values from an array to the 2q cache
    \param [out] arr  Pointer to the first element of the query array 
    \param [in] length  Array length
    \param [in] sizeCache  Сache and hashtable size
    \return Number of 2q cache hits
*/
unsigned long appendArrayTo2Q(long *arr, unsigned long length, unsigned long sizeCache);
