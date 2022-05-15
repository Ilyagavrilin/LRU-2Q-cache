#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum {
    MIN_length = 5,
    MAX_length = 100,

    MIN_sizeCache = 4,
    MAX_sizeCache = 50,

    MIN_element_arr = 5,
    MAX_element_arr = 100,
};
//--- header

///Accepts 2 numbers and outputs a random number between them (used to generate the size of an array)
unsigned CreateN(unsigned min, unsigned max);
/*!
    unsigned min - minimum number
    unsigned max - maximum number
*/

///Accepts 2 numbers and outputs a random number between them (used to generate the cache size)
long CreatesizeCache(long min, long max);
/*!
   * unsigned min - minimum number
   * unsigned max - maximum number
*/

///Fills the array with random numbers
void  FillArr(long a[], unsigned long size, unsigned min, unsigned max);
/*!
   * unsigned min - minimum number in array
   * unsigned max - maximum number in array
*/

int PunPreload();

int RunGenerated(int a);

int RunTests(int a);
