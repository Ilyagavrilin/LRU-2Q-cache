#include <stdio.h>
#include "hashmap.h"


int getRandomNumber(int min, int max) {
    assert(min <= max);

    int x;

    do {
        x = rand();
    } while (x >= RAND_MAX - (RAND_MAX % (max - min + 1)));

    return x % (max - min + 1);
} 

#if TEST_CODE
void showHashTableSettings(hashmap_t *table) {
    printf("a = %du; b = %du; size = %lu\n", table->a, table->b, table->size);
}

void showHashTable(hashmap_t *table) {
    printf("-----------------\n");
    for (int i = 0; i < table->size; ++i) {
        if (table->data[i] != NULL) {
            printf("hashIndex: %d ", i);
            bucket_t *link = table->data[i];
            bucket_t *nextLink = link;
            while (link != NULL) {
                printf("--(key: %ld)", link->key);
                link = link->next;
            }
            printf("--NULL;\n");
        }
    }
    printf("-----------------\n");
}
#endif

hashmap_t createHashTable(unsigned long size) {
    hashmap_t table;

    table.a = getRandomNumber(1, INT_MAX - 1);
    table.b = getRandomNumber(0, INT_MAX - 1);
    table.size = size;
    table.data = (bucket_t**) calloc(size, sizeof(bucket_t*));

    if (table.data == NULL) {
        table.size = 0;
        table.data = NULL;
        return table;
    }

    for (int i = 0; i < size; ++i) {
        table.data[i] = NULL;
    }
    
    return table;
}

void deleteHashTableData(hashmap_t *table) {
    for (int i = 0; i < table->size; ++i) {
        bucket_t *link = table->data[i];
        bucket_t *nextLink = link;
        while (link != NULL) {
            nextLink = link->next;
            free(link);
            link = nextLink;
        }
    }
    free(table->data);

    table->size = 0;

    #if TEST_CODE
    showHashTable(table);
    #endif
}

int getIntLog2(unsigned long size) {
    assert(size != 0);

    int powerRes = 0, powerNow = 0;
    size -= 1;
    while (size != 0) {
        powerNow += 1;
        if (size & 1)
            powerRes = powerNow;
        size = size >> 1;
    }

    return powerRes;
}

unsigned long hash(long key, unsigned long a, unsigned long b, unsigned long size) {
    assert(a != 0);

    return ((a * (unsigned long int)key + b) >> (32 - getIntLog2(size))) % size;
}

void *getElement(hashmap_t *table, long key) {
    assert(table != NULL);
    assert(table->data != NULL);

    unsigned long hashValue = hash(key, table->a, table->b, table->size);
    bucket_t *link = table->data[hashValue];

    while(link != NULL) {
        if (link->key == key) {
            #if TEST_CODE
            showHashTable(table);
            #endif

            return link->value;
        }
        link = link->next;
    }

    #if TEST_CODE
    showHashTable(table);
    #endif

    return NULL;
}

int addElement(hashmap_t *table, long key, void *value) {
    assert(table != NULL);
    assert(table->data != NULL);

    if(getElement(table, key) != NULL) {
        #if TEST_CODE
        showHashTable(table);
        #endif

        return 0;
    }

    unsigned long hashValue = hash(key, table->a, table->b, table->size);
    bucket_t *newBucket = (bucket_t*) calloc(1, sizeof(bucket_t));

    if (newBucket == NULL) {
        #if TEST_CODE
        showHashTable(table);
        #endif

        return -1;
    }

    newBucket->key = key;
    newBucket->value = value;
    newBucket->next = table->data[hashValue];
    table->data[hashValue] = newBucket;

    #if TEST_CODE
    showHashTable(table);
    #endif

    return 1;
}

void *deleteElement(hashmap_t *table, long key) {
    assert(table != NULL);
    assert(table->data != NULL);

    unsigned long hashValue = hash(key, table->a, table->b, table->size);
    bucket_t **link = (table->data) + hashValue;

    if ((*link) == NULL) {
        #if TEST_CODE
        printf("return #1\n");
        showHashTable(table);
        #endif

        return NULL;
    }

    do {
        #if TEST_CODE
        showHashTable(table);
        #endif
        if ((*link)->key == key) {
            void *value = (*link)->value;
            *link = (*link)->next;

            #if TEST_CODE
            printf("return #2\n");
            showHashTable(table);
            #endif

            return value;
        }
        printf("%ld\n", (*link)->key);
        link = &((*link)->next);
    } while (*link != NULL);

    #if TEST_CODE
    printf("return #3\n");
    showHashTable(table);
    #endif

    return NULL;
}
 