#include <stdio.h>
#include <iostream>

using namespace std;

#include "../CommunicationProtocol.h"

void print_status_packet(HealthData statusPacket){
  cout << "Packet index: " << statusPacket.index << endl;
  cout << "Reading time: " << statusPacket.time << endl;
  // cout << "Battery charge: " << statusPacket.battery_charge << endl;
  // cout << "Battery current: " << statusPacket.battery_current << endl;
  cout << "Battery voltage: " << statusPacket.battery_voltage << endl;
  // cout << "Battery temperature: " << statusPacket.battery_temperature << endl;
  cout << "External temperature: " << statusPacket.external_temperature << endl;
  cout << "Internal temperature: " << statusPacket.internal_temperature << endl;
  cout << "Pressure: " << statusPacket.pressure << endl;
  cout << "Altitude: " << statusPacket.altitude << endl;
  cout << "Memory usage: " << statusPacket.sd_memory_usage << endl;
  cout << "Ax: " << statusPacket.accel_x << endl;
  cout << "Ay: " << statusPacket.accel_y << endl;
  cout << "Gx: " << statusPacket.giros_x << endl;
  cout << "Gy: " << statusPacket.giros_y << endl;
}

void print_status_packet_bytes(HealthData statusPacket){
  for(int i = 0; i < sizeof(HealthData); i++){
    cout << (int)(((uint8_t*)&statusPacket)[i]) << " ";
  }
}

void print_imaging_packet(ImagingData imagingPacket){
  // for(int i = 0; i < 5; i++){
    cout << "Index: " << imagingPacket.index << endl;
    cout << "Duration: " << imagingPacket.duration << endl;
    cout << "Size: " << imagingPacket.radius << endl;
    cout << "x: " << imagingPacket.x << endl;
    cout << "y: " << imagingPacket.y << endl << endl;
  // }
}

void print_imaging_packet_bytes(ImagingData imagingPacket){
  for(int i = 0; i < sizeof(ImagingData); i++){
    cout << (int)(((uint8_t*)&imagingPacket)[i]) << " ";
  }
}

void print_operation(GSPacket gsPacket){
  cout << "Active thermal control: " << gsPacket.data.operation.switch_active_thermal_control ? "ON" : "OFF";
  cout << "Attitude control: " << gsPacket.data.operation.switch_attitude_control ? "ON" : "OFF";
  cout << "Imaging: " << gsPacket.data.operation.switch_imaging ? "ON" : "OFF";
  cout << "Imaging mode: " << gsPacket.data.operation.switch_imaging_mode ? "Mode 2" : "Mode 1";
  cout << "Stand by: " << gsPacket.data.operation.switch_stand_by_mode ? "ON" : "OFF";
}
