#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <unistd.h>
// #include <iostream>

void send_to_control(int mode, int gz){
  int serial_port ;
  if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)	/* open serial port */
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return;
  }

  if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return;
  }
	  
	if(serialDataAvail (serial_port) )
	{ 
    serialPutchar(serial_port, (mode+48));
    serialPutchar(serial_port, ';');
    u_int8_t b;
    while (gz>0){
      b = gz%10;
      serialPutchar(serial_port, b);
      gz/=10;
    }
    serialPutchar(serial_port,'\n');
		//fflush (stdout) ; /* transmit character serially on port */
	                     
	}
	
}

// int main(){
//   send_to_control(0, 400, 600);
//   sleep(4);
//   send_to_control(1, 400, 600);
//   sleep(4);
//   send_to_control(2, 400, 600);
//   sleep(4);
//   return 0;
// }