#!/bin/bash

while true; do
    bmp180=$(./bmp180/temp.py)
    read -r temp pressure altitude <<< "$bmp180"
    mpu6050=$(./mpu6050/mpu6050.out)
    read -r gx gy gz ax ay az <<< "$mpu6050"
    voltage=$(./voltage/sensor.py)
    diskp=$(./disk_usage.sh)
    > send.json
    printf '{ "temp_bmp": "%s ºC", "pressure_bmp": "%s Pa", "altitude_bmp": "%s m", "Gx_mpu": "%.3f °/s", "Gy_mpu": "%.3f °/s", "Gz_mpu": "%.3f °/s", "Ax_mpu": "%.3f g", "Ay_mpu": "%.3f g", "Az_mpu": "%.3f g", "voltage": "%.3f V", "diskp": "%s" }' "$temp" "$pressure" "$altitude" "$gx" "$gy" "$gz" "$ax" "$ay" "$az" "$voltage" "$diskp" > send.json
    python dataAquisition.py
    sleep 0.5
done
