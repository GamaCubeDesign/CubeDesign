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
	  printf("ATTITUDE CONTROL DAMMIT\n");
    printf("ATTITUDE CONTROL ");
    printf("%d", mode);
    printf(" Gyro print ");
    serialPutchar(serial_port, (mode+48));
    serialPutchar(serial_port, ';');
    u_int8_t b;
    while (gz>0){
      b = gz%10;
      serialPutchar(serial_port, b);
      printf("%d", (int)b);
      gz/=10;
    }
    printf("\n");
    serialPutchar(serial_port,'\n');
    if(serialDataAvail (serial_port) ){
		//fflush (stdout) ; /* transmit character serially on port */
	    printf("ATTITUDE CONTROL RECEIVED: %d\n", (int)serialGetchar(serial_port));
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