#include "cache2q.h"
#include "input.h"

int main(){
  srand(time(0));
  while ((data = InputScan()) != NULL){
    if (InputChkOccur(data)){
      printf("%ld\n", appendArrayTo2Q(data->requests, data->requests_sz, data->cache_size));
    }
    InputFree(data);
  }
  return 0;
}