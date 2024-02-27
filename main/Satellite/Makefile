all: SatTTC.out

HomeFolder := $(shell pwd)/..
# HomeFolder := ~/gamasat/TTEC-Gamacubedesign
# HomeFolder := /mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign
LibFolder := $(HomeFolder)

# LoraLib := $(LibFolder)/sx1278-LoRa-RaspberryPi

# MAIN

# CommunicationProtocol.o: CommunicationProtocol.h
# 	g++ -c CommunicationProtocol.h -o CommunicationProtocol.o

Timing.o: Timing.cpp
	g++ -c Timing.cpp -o Timing.o

Logger.o: Logger.cpp
	g++ -c Logger.cpp -o Logger.o -lrt -pthread -lm

# RFModemOffline.o: RFModemOffline.cpp
# 	g++ -c RFModemOffline.cpp -o RFModemOffline.o -lrt -pthread -lm

RFModem.o: RFModem.cpp
	g++ -c RFModem.cpp -o RFModem.o -lrt -pthread -lm

RFModule.o: RFModule.cpp
	g++ -c RFModule.cpp -o RFModule.o -lrt -pthread -lm

DataFIFO.o: DataFIFO.cpp
	g++ -c DataFIFO.cpp -o DataFIFO.o

AttitudeControl.o: AttitudeControl.c
	gcc -c AttitudeControl.c -o AttitudeControl.o -lpigpio -lrt -pthread -lm

LoRa.o: LoRa.c
	gcc -c LoRa.c -o LoRa.o -lpigpio -lrt -pthread -lm

SatTTC.o: SatTTC.cpp
	g++ -c SatTTC.cpp -o SatTTC.o -lpigpio -lrt -pthread -lm

SatTTC.out: Timing.o Logger.o RFModule.o RFModem.o SatTTC.o DataFIFO.o LoRa.o AttitudeControl.o
	g++ -o SatTTC.out SatTTC.o Timing.o Logger.o RFModule.o RFModem.o DataFIFO.o LoRa.o AttitudeControl.o -lpigpio -lrt -pthread -lm

clean:
	rm *.o *.out

test: test_struct_formatting.cpp
	g++ -o test.out test_struct_formatting.cpp

run: SatTTC.out
	sudo ./SatTTC.out

run_test_timing: Timing.o
	g++ -o test_timing.out test_timing.cpp Timing.o
	./test_timing.out