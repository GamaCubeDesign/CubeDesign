all: mpu6050.out

#wiringPi.o: wiringPi.c
#	gcc -o wiringPi.o -c wiringPi.c

#wiringPiI2C.o: wiringPiI2C.c
#	gcc -o wiringPiI2C.o -c wiringPiI2C.c

#gpio.o: gpio.c
#	gcc -o gpio.o -c gpio.c

mpu6050.out: gpio.c
	gcc -o mpu6050.out gpio.c -lwiringPi

clean:
	rm *.o *.out
