#!/usr/bin/env python3

import smbus
import time
from ctypes import c_short

DEVICE = 0x48

#bus = smbus.SMBus(0)  # Rev 1 Pi uses 0
bus = smbus.SMBus(1) # Rev 2 Pi uses 1 
 
def convertToString(data):
	# Simple function to convert binary data into a string
	return str((data[1] + (256 * data[0])) / 1.2)

def getShort(data, index):
	# return two bytes from data as a signed 16-bit value
	return c_short((data[index] << 8) + data[index + 1]).value

def getUshort(data, index):
	# return two bytes from data as an unsigned 16-bit value
	return (data[index] << 8) + data[index + 1]

def readten(addr=0x48):
	# Register Addresses
	REG_CALIB = 0xAA
	REG_MEAS = 0xF4
	REG_MSB = 0xF6
	REG_LSB = 0xF7
	# Control Register Address
	CRV_TEMP = 0x2E
	CRV_PRES = 0x34 
	# Oversample setting
	OVERSAMPLE = 3    # 0 - 3

	# Read calibration data from EEPROM
	cal = bus.read_i2c_block_data(addr, REG_CALIB, 22)

	# Convert byte data to word values
	AC1 = getShort(cal, 0)
	AC2 = getShort(cal, 2)
	AC3 = getShort(cal, 4)
	AC4 = getUshort(cal, 6)
	AC5 = getUshort(cal, 8)
	AC6 = getUshort(cal, 10)
	B1 = getShort(cal, 12)
	B2 = getShort(cal, 14)
	MB = getShort(cal, 16)
	MC = getShort(cal, 18)
	MD = getShort(cal, 20)

	# Read temperature
	bus.write_byte_data(addr, REG_MEAS, CRV_TEMP)
	time.sleep(0.005)
	(msb, lsb) = bus.read_i2c_block_data(addr, REG_MSB, 2)
	UT = (msb << 8) + lsb
	return (UT,msb,lsb)

voltage, msb, lsb = readten()

print(voltage,msb,lsb)