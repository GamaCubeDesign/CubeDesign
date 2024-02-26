#include <iostream>
#include "Timing.h"

using namespace std;

int main(){
  long now = millis();
  cout << "now " << now << endl;
  // long after = now + 1000;
  long next_print = now + 1000;
  while(millis() < 10000){
    if(millis() > next_print){
      next_print += 1000;
      cout << "waiting " << millis() << endl;
    }
  }
  cout << "after " << millis();
}