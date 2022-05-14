#include "cache2q.h"
#include "input.h"

int main() {
  input_t* data;
  while ((data = InputScan()) != NULL) {
    printf("%ld\n", appendArrayTo2Q(data->requests, data->requests_sz, data->cache_size));
    InputFree(data);
  }
  return 0;
}
