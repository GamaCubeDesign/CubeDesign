#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
// #include <iostream>

void send_to_control(int mode, int gx, int gy){
  int serial_port ;
  u_int8_t dat;
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
    while (gx>0){
      b = gx%10;
      serialPutchar(serial_port, b);
      gx/=10;
    }
    serialPutchar(serial_port, ';');
    	/* receive character serially*/
      while (gy>0){
      b = gy%10;
      serialPutchar(serial_port, b);
      gy/=10;
    }
    serialPutchar(serial_port,'\n');
		//fflush (stdout) ; /* transmit character serially on port */
	                     
	}
	
}