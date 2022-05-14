#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//define type which you want to put into cache
typedef long int caching_t;

//Mode of program running
typedef enum {
  TEST,
  RUN_CONSOLE,
  RUN_SIMPLE,
  RUN_FILE,
  END_PROC,
} MODE;
//structure of input data
typedef struct {
  long cache_size;
  long requests_sz;
  caching_t* requests;
} input_t;

input_t* InputScan();//main function
MODE InputInit();
input_t *InputConsole();
input_t *InputFile();
input_t *InputSimple();
caching_t InputScanCache(FILE* stream);//function which writes for each caching type
int RunTest();
FILE* InputScanFname();
void InputFree(input_t* data);
