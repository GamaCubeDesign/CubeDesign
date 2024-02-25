#!/usr/bin/env python3

import spidev
import time

# Setup SPI
spi = spidev.SpiDev()
spi.open(0, 0)  # Bus 0, Device 0

def read_adc(channel):
    adc = spi.xfer2([1, (8 + channel) << 4, 0])
    data = ((adc[1] & 3) << 8) + adc[2]
    return data

# Analog input channel on MCP3008 (e.g., CH0)
sensor_channel = 0

try:
	sensor_value = read_adc(sensor_channel)
	voltage = (sensor_value / 1023.0) * 3.3  # Assuming 3.3V ADC reference voltage
	print(f"{voltage}")
except KeyboardInterrupt:
    spi.close()
