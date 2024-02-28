#include "DataFIFO.h"
#include <iostream>
using namespace std;

void ImagingFIFO::write(ImagingData b){
  unsigned int n = this->serial_buffer_writing_pointer++;
  cout << "Adding Imaging Packet to FIFO:" << endl;
  for(unsigned int i = 0; i < sizeof(ImagingData); i++){
    ((uint8_t*)&(serial_buffer[n]))[i] = ((uint8_t*)&b)[i];
  }
  if(this->serial_buffer_writing_pointer == this->buffer_size){
    this->serial_buffer_writing_pointer = 0;
  }
  this->serial_buffer_size++;
}

ImagingData ImagingFIFO::read(){
  unsigned int n = this->serial_buffer_reading_pointer++;
  ImagingData b;
  for(unsigned int i = 0; i < sizeof(ImagingData); i++){
    ((uint8_t*)&b)[i] = ((uint8_t*)&(serial_buffer[n]))[i];
  }
  if(this->serial_buffer_reading_pointer >= this->buffer_size){
    this->serial_buffer_reading_pointer = 0;
  }
  this->serial_buffer_size--;
  return b;
}

void StatusFIFO::write(HealthData b){
  unsigned int n = this->serial_buffer_writing_pointer++;
  cout << "Adding Status packet to FIFO" << endl;
  cout << "Packet index: " << b.index << endl;
  cout << "Reading time: " << b.time << endl;
  cout << "External temperature: " << b.external_temperature << endl;
  cout << "Internal temperature: " << b.internal_temperature << endl;
  cout << "Battery voltage: " << b.battery_voltage << endl;
  // cout << "Battery charge: " << b.battery_charge << endl;
  // cout << "Battery current: " << b.battery_current << endl;
  // cout << "Battery temperature: " << b.battery_temperature << endl;
  cout << "Pressure: " << b.pressure << endl;
  cout << "Altitude: " << b.altitude << endl;
  cout << "Memory usage: " << b.sd_memory_usage << endl;
  cout << "Ax: " << b.accel_x << endl;
  cout << "Ay: " << b.accel_y << endl;
  cout << "Gx: " << b.giros_x << endl;
  cout << "Gy: " << b.giros_y << endl;
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    ((uint8_t*)&(serial_buffer[n]))[i] = ((uint8_t*)&b)[i];
  }
  if(this->serial_buffer_writing_pointer == this->buffer_size){
    this->serial_buffer_writing_pointer = 0;
  }
  this->serial_buffer_size++;
}

HealthData StatusFIFO::read(){
  unsigned int n = this->serial_buffer_reading_pointer++;
  HealthData b;
  for(unsigned int i = 0; i < sizeof(HealthData); i++){
    ((uint8_t*)&b)[i] = ((uint8_t*)&(serial_buffer[n]))[i];
  }
  if(this->serial_buffer_reading_pointer >= this->buffer_size){
    this->serial_buffer_reading_pointer = 0;
  }
  this->serial_buffer_size--;
  return b;
}

bool FIFO::available(){
  return this->serial_buffer_size > 0;
}