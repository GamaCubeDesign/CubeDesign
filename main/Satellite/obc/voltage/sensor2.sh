#!/bin/bash

# I2C address of the ADS1115
ADS1115_ADDR=0x48

# ADS1115 configuration register address
CONFIG_REG=0x01

# Function to read sensor value from ADS1115
read_ads1115() {
    local value=$(i2cget -y 1 $ADS1115_ADDR $CONFIG_REG w | awk '{printf "%d", "0x" $3 $2}')
    echo $value
}

while true; do
    sensor_value=$(read_ads1115)
    voltage=$(echo "scale=2; $sensor_value * 4.096 / 32767" | bc)
    echo "Sensor Value: $sensor_value, Voltage: $voltage V"
    sleep 1
done

