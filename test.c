#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "cache2q.h"

unsigned CreateN(unsigned min, unsigned max){
    int n = 0;
    n = min + rand() % (max - min + 1);
    return n;
}

long CreatesizeCache(long min, long max){
    int n = 0;
    n = min + rand() % (max - min + 1);
    return n;
}

void  FillArr(long a[], unsigned long size, unsigned min, unsigned max) {                  // генератор Массивов
    for (unsigned long i = 0; i < size; i++) {
        a[i] = min + rand() % (max - min + 1);
    }
}


int PunPreload() {
    //////////////////////////Test Block//////////////////////////////////
    //          length;  sizeCache;   ans;       arr
    long arr0[] = {5,        4,        0,    1, 2, 3, 4, 5};
    long arr1[] = {5,        4,        4,    0, 0, 0, 0, 0};
    long arr2[] = {3,        3,        2,    0, 0, 0};
    long arr3[] = {3,        4,        0,    1, 2, 3};
    ///////////////////////////////////////////////////////////////

    long *SUPER_ARR[] = {arr0, arr1, arr2, arr3};


    for (int i = 0; i < 4; i ++) {
        long *h = 0;
        unsigned long ans = 0;
        h = SUPER_ARR[i] + 3;

        ans = appendArrayTo2Q(h, SUPER_ARR[i][0], SUPER_ARR[i][1]);

        if (ans != SUPER_ARR[i][2]){
            printf("\nError in PunPreload!!!\n");
            printf("arr: ");
            for (int j = 0; j < SUPER_ARR[i][0]; j ++) {
                printf("%lu ", h[j]);
            }

            printf("program answer = %lu ", ans);

            printf("true ans = %lu ", SUPER_ARR[i][2]);
            printf("length = %lu ", SUPER_ARR[i][0]);
            printf("sizeCache = %lu ", SUPER_ARR[i][1]);
            return 0;
        }
    }
    return 1;
}

int RunGenerated(int a) {
    long *arr;

    unsigned long length = 0;
    long sizeCache = 0;
    unsigned long ans = 0;

    for (int i = 0; i < a; i ++) {
        length = CreateN(MIN_length, MAX_length);
        sizeCache = CreatesizeCache(MIN_sizeCache, MAX_sizeCache);

        arr = (long *) calloc(length, sizeof(long));
        FillArr(arr, length, MIN_element_arr, MAX_element_arr);

        ans = appendArrayTo2Q(arr, length, sizeCache);

        printf("program answer = %lu ", ans);
        printf("length = %lu ", length);
        printf("sizeCache = %lu ", sizeCache);
        printf("\tarr: ");

        for (int j = 0; j < length; j ++) {
            printf("%lu ", arr[j]);
        }

        printf("\n");
        free(arr);
    }
    return 1;
}


int RunTests(int a) {
    int status = 0;

    status = PunPreload() && RunGenerated(a);

    if (status) {
        printf("All right!!!\n");
    }

    return status;
}

