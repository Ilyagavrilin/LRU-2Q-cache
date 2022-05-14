#include <stdio.h>
#include "cache2q.h"


#if TEST_CODE_2Q 
void showCache2QSettings(Cache2Q_t *cache) {
    if (cache == NULL || cache->fifoInRoot == NULL || cache->fifoOutRoot == NULL || cache->lru2Root == NULL || cache->table == NULL) {
        printf("2Q cache does not exist\n");
        return;
    }

    printf("size FIFO in = %d; size FIFO out = %d; size LRU2 = %d; size hashtable = %lu\n", \
    cache->fifoInRoot->max_cnt, cache->fifoOutRoot->max_cnt, cache->lru2Root->max_cnt, cache->table->size);

    return;
}

void showCache2Q(Cache2Q_t *cache) {
    if (cache == NULL || cache->fifoInRoot == NULL || cache->fifoOutRoot == NULL || cache->lru2Root == NULL || cache->table == NULL) {
        printf("2Q cache does not exist\n");
        return;
    }

    chain_t *node = cache->fifoInRoot->next;

    printf("FIFO In (size = %d): \n", cache->fifoInRoot->max_cnt);
    while (node != cache->fifoInRoot && node != NULL) {
        printf("--(value: %ld)", node->value);
        node = node->next;
    }
    printf("--\n");

    node = cache->fifoOutRoot->next;

    printf("FIFO Out (size = %d): \n", cache->fifoOutRoot->max_cnt);
    while (node != cache->fifoOutRoot && node != NULL) {
        printf("--(value: %ld)", node->value);
        node = node->next;
    }
    printf("--\n");

    node = cache->lru2Root->next;

    printf("LRU2 (size = %d): \n", cache->lru2Root->max_cnt);
    while (node != cache->lru2Root && node != NULL) {
        printf("--(value: %ld)", node->value);
        node = node->next;
    }
    printf("--\n");

    return;
}
#endif

Cache2Q_t *createCache2Q(unsigned long size) {
    assert(size != 0);

    Cache2Q_t *cache = (Cache2Q_t*) calloc(1, sizeof(Cache2Q_t));

    if (cache == NULL) {
        return NULL;
    }

    cache->fifoInRoot = ChainCtr(FIFO_IN, size / 5);

    if (cache->fifoInRoot == NULL) {
        free(cache);
        return NULL;
    }

    cache->fifoOutRoot = ChainCtr(FIFO_OUT, (size / 5) * 3);

    if (cache->fifoOutRoot == NULL) {
        ChainDtr(cache->fifoInRoot);
        free(cache);
        return NULL;
    }

    cache->lru2Root = ChainCtr(LRU, size / 5);

    if (cache->lru2Root == NULL) {
        ChainDtr(cache->fifoInRoot);
        ChainDtr(cache->fifoOutRoot);
        free(cache);
        return NULL;
    }

    cache->table = createHashTable(size);

    if (cache->table == NULL) {
        ChainDtr(cache->fifoInRoot);
        ChainDtr(cache->fifoOutRoot);
        ChainDtr(cache->lru2Root);
        free(cache);
        return NULL;
    }

    #if TEST_CODE_2Q 
    showCache2QSettings(cache);
    #endif

    return cache;
}

void deleteCache2Q(Cache2Q_t *cache) {
    if (cache == NULL) {
        #if TEST_CODE_2Q 
        printf("2Q cache does not exist\n");
        #endif

        return;
    }

    if (cache->fifoInRoot != NULL) {
        ChainDtr(cache->fifoInRoot);
    }

    if (cache->fifoOutRoot != NULL) {
        ChainDtr(cache->fifoOutRoot);
    }

    if (cache->lru2Root != NULL) {
        ChainDtr(cache->lru2Root);
    }

    if (cache->table != NULL) {
        deleteHashTable(cache->table);
    }

    free(cache);

    #if TEST_CODE_2Q 
    printf("2Q cache does not exist\n");
    #endif

    return;
}

// adds a new unique element to the beginning and removes the last element in the queue on overflow
chain_t *displacingElement(chain_t *root, chain_t *toInsert) {
    assert(root != NULL);
    assert(toInsert != NULL);
    assert(root->type == ROOT && root->name == toInsert->name);

    chain_t *removedChain;

    if (root->cur_cnt + 1 <= root->max_cnt) {
        assert(ChainInsert(root, toInsert, HEAD) == root);
        return NULL;
    }

    removedChain = ChainRemoveFP(root, TAIL);

    assert(removedChain != NULL);
    assert(ChainInsert(root, toInsert, HEAD) == root);

    return removedChain;
}

int appendTo2Q(long value, Cache2Q_t *cache) {
    assert(cache != NULL);
    assert(cache->fifoInRoot != NULL && cache->fifoOutRoot != NULL && cache->lru2Root != NULL && cache->table != NULL);

    chain_t *node = (chain_t*) getElement(cache->table, value);
    chain_t *removedChain;
    
    if (node == NULL) {
        node = ChainMakeNode(value, FIFO_IN);
        removedChain = displacingElement(cache->fifoInRoot, node);

        assert(addElement(cache->table, node->value, node));

        if (removedChain == NULL) {
            return 0;
        }
        removedChain->name = FIFO_OUT;
        removedChain = displacingElement(cache->fifoOutRoot, removedChain);
        if (removedChain != NULL) {
            assert(deleteElement(cache->table, removedChain->value) == removedChain);
            free(removedChain);
        }
        return 0;
    }
    
    if (node->name != FIFO_IN) {
        removedChain = ChainRemove((node->name == FIFO_OUT) ? (cache->fifoOutRoot) : (cache->lru2Root), node);

        assert(removedChain != NULL);

        removedChain->name = LRU;
        removedChain = displacingElement(cache->lru2Root, removedChain);

        if (removedChain != NULL) {
            assert(deleteElement(cache->table, removedChain->value) == removedChain);
            free(removedChain);
        }
    } 

    return 1;
}

unsigned long appendArrayTo2Q(long *arr, unsigned long length, unsigned long sizeCache) {
    assert(arr != NULL);
    assert(length != 0 && sizeCache != 0);

    Cache2Q_t *cache = createCache2Q(sizeCache);
    unsigned long res = 0;

    if (cache == NULL) {
        return 0;
    }

    for (unsigned long i = 0; i < length; ++i) {
        res += appendTo2Q(arr[i], cache);

        #if TEST_CODE_2Q 
        showCache2Q(cache);
        printf("Count of 2q cache hits: %lu\n\n", res);
        #endif
    }

    deleteCache2Q(cache);
    return res;
}
