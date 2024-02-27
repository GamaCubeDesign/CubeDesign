#!/bin/bash

index=1
time=0
while true; do
    bmp180=$(./bmp180/temp.py)
    echo "$bmp180"
    read -r temp pressure altitude <<< "$bmp180"
    mpu6050=$(./mpu6050/mpu6050.out)
    echo "$mpu6050"
    read -r gx gy gz ax ay az <<< "$mpu6050"
    voltage=$(python ./voltage/voltage.py)
    echo "$voltage"
    read -r bat_voltage ntc volt_3v <<< "$voltage"
    diskp=$(./disk_usage.sh)
    > send.json
    printf '{ "time": %d, "index": %d, "temp_bmp": %.3f, "pressure_bmp": %.3f, "altitude_bmp": %.3f, "Gx_mpu": %.3f, "Gy_mpu": %.3f, "Gz_mpu": %.3f, "Ax_mpu": %.3f, "Ay_mpu": %.3f, "Az_mpu": %.3f, "battery_voltage": %.3f, "ext_temp_voltage": %.3f, "volt_3v": %.3f, "diskp": %s }' "$time" "$index" "$temp" "$pressure" "$altitude" "$gx" "$gy" "$gz" "$ax" "$ay" "$az" "$bat_voltage" "$ntc" "$volt_3v" "$diskp" > send.json
    python dataAquisition.py
    let index++
    let time+=500
    sleep 0.5
done

#printf '{ "temp_bmp": "%s ºC", "pressure_bmp": "%s Pa", "altitude_bmp": "%s m", "Gx_mpu": "%.3f °/s", "Gy_mpu": "%.3f °/s", "Gz_mpu": "%.3f °/s", "Ax_mpu": "%.3f g", "Ay_mpu": "%.3f g", "Az_mpu": "%.3f g", "voltage": "%.3f V", "diskp": "%s" }' "$temp" "$pressure" "$altitude" "$gx" "$gy" "$gz" "$ax" "$ay" "$az" "$voltage" "$diskp" > send.json
