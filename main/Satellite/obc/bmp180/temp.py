#!/usr/bin/env python3

import bmpsensor
import time

temp, pressure, altitude = bmpsensor.readBmp180()
print(temp, pressure, altitude)
