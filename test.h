#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


//--- header
int createN(int a);
//----.c file

int bebra(unsigned long cache_sz, unsigned long arr_sz, unsigned long *arr) {
    return 1;
}

//[0, n_requests]
void CreateArr(unsigned long a[], unsigned long size, unsigned long n_requests) {                  // генератор Массивов
    for (unsigned long i = 0; i < size; i++) {
        a[i] = rand();
    }
}


//изменить на int 1 - success, 0 -
int RunTests(int n_tests) {
    unsigned long cache_sz = 0;
    unsigned long arr_sz = 0;
    unsigned long mass = 0;
    unsigned long *arr;
    int exArr_sz[5] = {5,
                  5,
                  5,
                  5,
                  5};

    int exCache_sz[5] = {0,
                  -1,
                  5,
                  2,
                  5};

    int exArr[5][5] = {{1, 2, 3, 4, 5},
                       {-1, -2, -3, -4, -5},
                       {1, 2, 3, 4, 5},
                       {1, 2, 3, 4, 5},
                       {1, 2, 3, 4, 5}};


    for (int i = 0; i < n_tests; i++) {
        if (i < 5){

            arr_sz = exArr_sz[i];
            cache_sz = exCache_sz[i];

            mass = bebra(exArr_sz[i], exCache_sz[i], exArr[i]);

            if (0 > mass || mass > arr_sz) {
                return 0;
            } else {
                for (int j = 0; j < arr_sz; j++) {
                    printf("%d ", exArr[i][j]);
                }
                printf("\n");
            }

        } else {
            arr_sz = createN(10);
            cache_sz = createN(10);

            arr = (unsigned long *) calloc(arr_sz, sizeof(unsigned long)); //no malloc chacking
            assert(arr != NULL);

            CreateArr(arr, arr_sz, 100);
            mass = bebra(cache_sz, arr_sz, arr);

            if (0 > mass || mass > arr_sz) {
                free(arr);
                return 0;
            } else {
                for (int j = 0; j < arr_sz; j++) {
                    printf("%lu ", arr[j]);
                }
                printf("\n");
                free(arr);
            }
        }
    }
    return 1;
}

