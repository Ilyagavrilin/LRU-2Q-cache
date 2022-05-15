#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


//--- header

///Generating a random number in the range from 0 to a
int createN(int a);
/*!
    a
*/


///the function for starting the program (hereinafter, instead of it, the function for starting the algorithm)
int bebra(unsigned long cache_sz, unsigned long arr_sz, unsigned long *arr);
/*!
    cache_sz - cache size
    arr_sz - array size
    arr - array of requests
*/

///random number list generation function
void CreateArr(unsigned long a[], unsigned long size, unsigned long n_requests);


//изменить на int 1 - success, 0 -
int RunTests(int n_tests);