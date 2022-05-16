#include "cache2q.h"
#include "input.h"
#include "test.h"


int main(){
  srand(time(0));
#ifdef TST_RUN
  if (RunTests(TST_RUN) == 0) {
    return 0;
  }
#else
  input_t* data;
#ifdef HW8_INPUT
  data = InputScan();
  if (InputChkOccur(data)){
      printf("%ld\n", appendArrayTo2Q(data->requests, data->requests_sz, data->cache_size));
  }
  InputFree(data);
#else  
  while ((data = InputScan()) != NULL){
    if (InputChkOccur(data)){
      printf("%ld\n", appendArrayTo2Q(data->requests, data->requests_sz, data->cache_size));
    }
    InputFree(data);
  }
#endif
#endif  
  return 0;
}
