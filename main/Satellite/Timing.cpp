#include "Timing.h"

clock_t timer = clock();
auto start = std::chrono::system_clock::now();

float seconds(){
  return ((float)(clock() - timer))/CLOCKS_PER_SEC;
}

long my_millis(){
  auto end = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  // return seconds()*1000;
}