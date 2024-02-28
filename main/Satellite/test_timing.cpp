#include <iostream>
#include "Timing.h"

using namespace std;

int main(){
  long now = my_millis();
  cout << "now " << now << endl;
  // long after = now + 1000;
  long next_print = now + 1000;
  while(my_millis() < 10000){
    if(my_millis() > next_print){
      next_print += 1000;
      cout << "waiting " << my_millis() << endl;
    }
  }
  cout << "after " << my_millis();
}