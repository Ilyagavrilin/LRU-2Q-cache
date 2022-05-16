#include "cache2q.h"
#include "input.h"



int main(){
  srand(time(0));
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
  return 0;
}
