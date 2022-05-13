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
    if (table == NULL || table->data == NULL) {
        printf("Table does not exist\n");
        return;
    }

    printf("a = %d; b = %d; size = %lu\n", table->a, table->b, table->size);

    return;
}

void showHashTable(hashmap_t *table, const char *message) {
    printf("%s: \n", message);
    if (table == NULL || table->data == NULL) {
        printf("Table does not exist\n");
        return;
    }

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
    printf("-----\n");

    return;
}
#endif

hashmap_t *createHashTable(unsigned long size) {
    assert(size != 0);

    hashmap_t *table = (hashmap_t*) calloc(1, sizeof(hashmap_t));

    if (table == NULL) {
        return NULL;
    }

    table->a = getRandomNumber(1, INT_MAX - 1);
    table->b = getRandomNumber(0, INT_MAX - 1);
    table->size = size;
    table->data = (bucket_t**) calloc(size, sizeof(bucket_t*));

    if (table->data == NULL) {
        free(table);
        return NULL;
    }

    for (int i = 0; i < size; ++i) {
        table->data[i] = NULL;
    }
    
    #if TEST_CODE
    showHashTableSettings(table);
    #endif

    return table;
}

void deleteHashTable(hashmap_t *table) {
    if (table == NULL) {
        #if TEST_CODE
        printf("deleteHashTable function completed\n");
        #endif

        return;
    }

    if (table->data != NULL) {
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
    }

    free(table);

    #if TEST_CODE
    printf("deleteHashTable function completed\n");
    #endif

    return;
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
    assert(size != 0);

    int m = getIntLog2(size);
    int w = (m > 32) ? 64 : 32;

    assert(m <= 64);

    return ((a * (unsigned long)key + b) >> (w - m)) % size;
}

void *getElement(hashmap_t *table, long key) {
    assert(table != NULL);
    assert(table->data != NULL);

    unsigned long hashValue = hash(key, table->a, table->b, table->size);
    bucket_t *link = table->data[hashValue];

    while(link != NULL) {
        if (link->key == key) {
            #if TEST_CODE
            showHashTable(table, "getElement function completed");
            #endif

            return link->value;
        }
        link = link->next;
    }

    #if TEST_CODE
    showHashTable(table, "getElement function completed");
    #endif

    return NULL;
}

int addElement(hashmap_t *table, long key, void *value) {
    assert(table != NULL);
    assert(table->data != NULL);

    if(getElement(table, key) != NULL) {
        #if TEST_CODE
        showHashTable(table, "addElement function completed");
        #endif

        return 0;
    }

    unsigned long hashValue = hash(key, table->a, table->b, table->size);
    bucket_t *newBucket = (bucket_t*) calloc(1, sizeof(bucket_t));

    if (newBucket == NULL) {
        #if TEST_CODE
        showHashTable(table, "addElement function completed");
        #endif

        return -1;
    }

    newBucket->key = key;
    newBucket->value = value;
    newBucket->next = table->data[hashValue];
    table->data[hashValue] = newBucket;

    #if TEST_CODE
    showHashTable(table, "addElement function completed");
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
        printf("deleteElement func return #1\n");
        showHashTable(table, "deleteElement function completed");
        #endif

        return NULL;
    }

    do {
        if ((*link)->key == key) {
            void *value = (*link)->value;
            *link = (*link)->next;

            #if TEST_CODE
            printf("deleteElement func return #2\n");
            showHashTable(table, "deleteElement function completed");
            #endif

            return value;
        }
        link = &((*link)->next);
    } while (*link != NULL);

    #if TEST_CODE
    printf("deleteElement func return #3\n");
    showHashTable(table, "deleteElement function completed");
    #endif

    return NULL;
}
 