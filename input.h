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
  int cache_size;
  unsigned long requests_sz;
  caching_t* requests;
} input_t;

//runs main process of returning data
input_t* InputScan();
//change programm mode
MODE InputInit();
//scan input values from console with wriitten hints
input_t *InputConsole();
//scan data from file
input_t *InputFile();
//like InputConsole, but without hints
input_t *InputSimple();
//function to scan that type of data which will be in cache
caching_t InputScanCache(FILE* stream);
//read from console name of file with request
FILE* InputScanFname();
//delets structure with input data
void InputFree(input_t* data);
//scans withiut multiple requests
caching_t InputFScanCache(FILE* stream);
//checks data before starting cache algorithm
int InputChkOccur(input_t* data);
