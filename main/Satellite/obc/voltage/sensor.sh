#!/bin/bash

# Set the GPIO pin numbers for the SPI interface
SPICLK=11
SPIMISO=9
SPIMOSI=10
SPICS=8

# Set the MCP3008 channel (0-7)
CHANNEL=0

# Configure SPI pins
gpio mode $SPICLK out
gpio mode $SPIMISO in
gpio mode $SPIMOSI out
gpio mode $SPICS out

# Function to read from MCP3008
read_mcp3008() {
    gpio write $SPICS 0
    gpio write $SPIMOSI 1
    gpio write $SPIMOSI 0
    gpio write $SPICS 1

    gpio write $SPICS 0
    gpio write $SPIMOSI 1
    gpio write $SPIMOSI 0

    value=0
    for i in {11..0}; do
        gpio write $SPICLK 1
        value=$((value << 1 | $(gpio read $SPIMISO)))
        gpio write $SPICLK 0
    done

    gpio write $SPICS 1

    echo $value
}

while true; do
    sensor_value=$(read_mcp3008 $CHANNEL)
    voltage=$(echo "scale=2; $sensor_value / 1023.0 * 3.3" | bc)
    echo "Sensor Value: $sensor_value, Voltage: $voltage V"
    sleep 1
done

