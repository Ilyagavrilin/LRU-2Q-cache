#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "test.h"

int createN(int a){               // генератор N
    int n = 0;
    n = rand() % a + 1;
    return n;
}

int main() {
    srand(time(NULL));//будет в main
    printf("\n%d", RunTests(10));
    return 0;
}
