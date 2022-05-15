#include "input.h"

//this define help to allocate place where error was raised
#define POSITION __FILE__, __LINE__, __PRETTY_FUNCTION__
//define SECURITY_LVL for enabling in_function warning
//if you want graphic dump set SECURITY_LVL>1
#define SECURITY_LVL 0


typedef enum {
  EXIT,
  NOT_EXIT,
} RESULT;

static int ShowErr(int expr, const char *msg, RESULT res, const char *file, int line, const char *func);
static void CleanBuffer();
static unsigned long ScanNumber(const char* msg, FILE* stream);
//---------------------

int RunTest() {
  return 1;
}

static int ShowErr(int expr, const char *msg, RESULT res, const char *file, int line, const char *func) {
#if SECURITY_LVL > 0
  if (expr > 0) { return 1; }
  fprintf(stderr, "Error in %s in line (%d), function %s: %s\n", file, line, func, msg);
  if (res == EXIT) {
    exit(1);
  }
  return 0;
#else
  assert(expr);
  return 1;
#endif
}

static void CleanBuffer() { //cleaning for case when user for mistake typed more than one number
  int buff_elem = 0;

  while (buff_elem != '\n' && buff_elem != EOF) {
    buff_elem = getchar();
  }
}

MODE InputInit() {
  printf("Choose mode:\n"
         "0 - quit program\n"
         "1 - run built-in tests\n"
         "2 - run with reading from the file\n"
         "3 - problem_lc like input\n"
         "4-9 - run with reading from the console\n");
  int usr_ans = 3, scanf_ret = 0;
  scanf_ret = scanf("%d", &usr_ans);
  while (scanf_ret != 1 || usr_ans < 0 || usr_ans > 9) {
    printf("You entered incorrect number for mode, please, try again.\n");
    CleanBuffer();
    scanf_ret = scanf("%d", &usr_ans);
  }

  switch (usr_ans) {
    case 0:
      return END_PROC;
    case 1:
      return TEST;
    case 2:
      return RUN_FILE;
    case 3:
      return RUN_SIMPLE;
    default:
      return RUN_CONSOLE;
  }
}

static unsigned long ScanNumber(const char* msg, FILE* stream) {
  long res;
  int scanf_ret = 0;
  ShowErr(stream != NULL, "invalid stream pointer", EXIT, POSITION);
  if (stream == stdin) {
    printf("%s", msg);
  }
  scanf_ret = scanf("%lu", &res);
  while (scanf_ret != 1 || res < 0) {
    if (stream == stdin) {
      printf("Incorrect value, please try again:");
    }
    CleanBuffer();
    scanf_ret = scanf("%ld", &res);
  }
  return res;
}

input_t* InputScan() {
  int tst_res;
  switch (InputInit()) {
    case END_PROC:
      return NULL;
    case TEST:
      tst_res = RunTest();
      if (tst_res == 1) {
        printf("test successfully passed.\n");
      } else {
        printf("Error occurred in tests, exiting.\n");
      }
      return NULL;
    case RUN_CONSOLE:
      return InputConsole();
    case RUN_FILE:
      return InputFile();
    case RUN_SIMPLE:
      return InputSimple();
    default:
      return NULL;
  }

}

caching_t InputScanCache(FILE* stream) {
  long res;
  int scanf_ret = 0;
  ShowErr(stream != NULL, "invalid stream pointer", EXIT, POSITION);
  scanf_ret = fscanf(stream, "%ld", &res);
  while (scanf_ret != 1) {
    printf("Incorrect value, please try again:");
    CleanBuffer();
    scanf_ret = fscanf(stream, "%ld", &res);
  }
  return res;
}

caching_t InputFScanCache(FILE* stream) {
  long res;
  ShowErr(stream != NULL, "invalid stream pointer", EXIT, POSITION);
  if (fscanf(stream, "%ld", &res) != 1) {
    return 0;
  }
  return res;
}

input_t *InputSimple() {
  input_t* res = (input_t*) malloc(sizeof(input_t));
  ShowErr(res != NULL, "memory allocation error", EXIT, POSITION);
  res->cache_size = ScanNumber("", stdin);
  res->requests_sz = ScanNumber("", stdin);
  ShowErr(res->requests_sz >= 0, "invalid number of requests", EXIT, POSITION);
  res->requests = (caching_t*) calloc(res->requests_sz, sizeof(caching_t));
  ShowErr(res->requests != NULL, "memory allocation error", EXIT, POSITION);
  for (int i = 0; i < res->requests_sz; i++) {
    res->requests[i] = InputScanCache(stdin);
  }

  return res;
}

input_t *InputConsole() {
  input_t* res = (input_t*) malloc(sizeof(input_t));
  ShowErr(res != NULL, "memory allocation error", EXIT, POSITION);
  res->cache_size = ScanNumber("Enter cache size(>= 0): ", stdin);
  res->requests_sz = ScanNumber("Enter number of requests(>=): ", stdin);
  ShowErr(res->requests_sz >= 0, "invalid number of requests", EXIT, POSITION);
  res->requests = (caching_t*) calloc(res->requests_sz, sizeof(caching_t));
  ShowErr(res->requests != NULL, "memory allocation error", EXIT, POSITION);
  printf("Enter requests:\n");
  for (int i = 0; i < res->requests_sz; i++) {
    res->requests[i] = InputScanCache(stdin);
  }

  return res;
}

input_t *InputFile() {
  static FILE* file = NULL;
  input_t* res = (input_t*) malloc(sizeof(input_t));
  ShowErr(res != NULL, "memory allocation error", EXIT, POSITION);
  if (file == NULL){
    file = InputScanFname();//no file pointer checking because it enabled in InputScanFname()
  }
  if (fscanf(file, "%ld %ld", &(res->cache_size), &(res->requests_sz)) != 2) {
    printf("Nothing to read in file.\n");
    return NULL;
  }
  ShowErr(res->cache_size >= 0, "invalid cache size", EXIT, POSITION);
  ShowErr(res->requests_sz >= 0, "invalid count of requests", EXIT, POSITION);
  res->requests = (caching_t*) calloc(res->requests_sz, sizeof(caching_t));
  ShowErr(res->requests != NULL, "invalid count of requests", EXIT, POSITION);
  for (int i = 0; i < res->requests_sz; i++) {
    res->requests[i] = InputFScanCache(file);
  }

  return res;
}

FILE* InputScanFname() {
  FILE* res;
  int scanf_res = 0;
  char filename[51] = {0};
  printf("Enter file name: ");
  while (scanf_res == 0) {
    scanf_res = scanf("%50s", filename);

    if (scanf_res != 0) {
      res = fopen(filename, "r");
      if (res == NULL) {
        printf("You entered non existing file.\n");
        scanf_res = 0;
      }
    }

  }
  return res;
}

void InputFree(input_t* data) {
  ShowErr(data!= NULL, "invalid pointer for free.", EXIT, POSITION);
  free(data->requests);
  data->cache_size = 0xBADDED; data->requests_sz = 0xBAD;
  free(data);
}
