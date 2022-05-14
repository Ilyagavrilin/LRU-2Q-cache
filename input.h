#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//this define help to allocate place where error was raised
#define POSITION __FILE__, __LINE__, __PRETTY_FUNCTION__
//define SECURITY_LVL for enabling in_function warning
//if you want graphic dump set SECURITY_LVL>1
#define SECURITY_LVL 0
//define type which you want to put into cache
typedef long int caching_t;

typedef enum {
  EXIT,
  NOT_EXIT,
} RESULT;
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