all: rx

LoRa.o: LoRa.c
	gcc -c LoRa.c -o LoRa.o -lpigpio -lrt -pthread -lm

rx.o: rx_test.cpp
	g++ -c rx_test.cpp -o rx.o -lpigpio -lrt -pthread -lm

rx: LoRa.o rx.o
	g++ -o rx rx.o LoRa.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o tx
