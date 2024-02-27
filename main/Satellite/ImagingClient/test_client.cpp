#include <iostream>

#include "ImagingDataClient.cpp"

ImagingData testingData;
ImagingDataClient imagingSocket(8080);

void do_stuff(){
  sleep(2);
}

int main(){
  
  for(int i = 0; i < 5; i++){
    testingData.lightnings[i].duration = i*10.0;
    testingData.lightnings[i].index = i;
    testingData.lightnings[i].radius = 5.0;
    testingData.lightnings[i].x = i + 25;
    testingData.lightnings[i].y = 20 + i * 1.5;
  }

  if(imagingSocket.connect_to_socket() < 0){
    cout << "Error connecting to socket" << endl;
    cout << "Goodbye" << endl;
    return 0;
  }

  while(imagingSocket.is_connected){
    do_stuff();
    imagingSocket.update();
    cout << "All nominal" << endl;
    imagingSocket.send_packet(testingData);
  };

  cout << "Finished" << endl;

  return 0;
}