#pragma once

#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define TEST_CODE 0

///< Singly linked list structure for elements with the same hash value
typedef struct bucket_t {
    long key;
    void *value;
    struct bucket_t *next;
} bucket_t;

///< A hashtable structure that takes a long int as a key and a void* pointer to the element as its value
typedef struct hashmap_t {
    unsigned a;     // random number from 1 to INT_MAX - 1 for the hash function
    unsigned b;     // random number from 0 to INT_MAX - 1 for the hash function
    unsigned long size;

    bucket_t **data;
} hashmap_t;

/*!
    \brief Gets the hash-value
    \param [in] key  Value to hash
    \param [in] a  Random number from 1 to INT_MAX - 1
    \param [in] b  Random number from 0 to INT_MAX - 1
    \param [in] size  Hashtable size
    \return Hashed value
*/
unsigned long hash(long key, unsigned long a, unsigned long b, unsigned long size);

/*!
    \brief Сreates an empty hashtable
    \param [in] size  Hashtable array size
    \return Hashtable structure
    \note Also, in the structure of the hashtable, the values a and b are generated for the hash function
*/
hashmap_t createHashTable(unsigned long size);

/*!
    \brief Adds a new key-value pair to the hashtable
    \param [in] table  Pointer to hashtable
    \param [in] key  Long int key
    \param [in] value  Pointer of type *void to a value
    \return 1 - if value added; 0 - if the value already exists; -1 - if an error occurred while adding the value
*/
int addElement(hashmap_t *table, long key, void *value);

/*!
    \brief Gets an existing pointer to a value given a key in a hash table
    \param [in] table  Pointer to hashtable
    \param [in] key  Long int key
    \return Pointer to the value if it exists in the hashtable, or NULL if the desired value does not exist
*/
void *getElement(hashmap_t *table, long key);

/*!
    \brief Removes an existing pointer to a value with the given key from the hash table
    \param [in] table  Pointer to hashtable
    \param [in] key  Long int key
    \return A pointer to the value if it could be removed from the hash table, or NULL if the desired value does not exist
*/
void *deleteElement(hashmap_t *table, long key);

/*!
    \brief Completely removes all data in the hashtable
    \param [in] table  Pointer to hashtable
    \note The hash table size is reset to zero
*/
void deleteHashTableData(hashmap_t *table);
