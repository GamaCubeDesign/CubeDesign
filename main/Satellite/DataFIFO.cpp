#include "DataFIFO.h"

void ImagingFIFO::write(ImagingData b){
  unsigned int n = this->serial_buffer_writing_pointer++;
  cout << "Adding Imaging Packet to FIFO:";
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
  cout << "Packet index: " << statusPacket.index << endl;
  cout << "Reading time: " << statusPacket.time << endl;
  cout << "External temperature: " << statusPacket.external_temperature << endl;
  cout << "Internal temperature: " << statusPacket.internal_temperature << endl;
  cout << "Battery voltage: " << statusPacket.battery_voltage << endl;
  // cout << "Battery charge: " << statusPacket.battery_charge << endl;
  // cout << "Battery current: " << statusPacket.battery_current << endl;
  // cout << "Battery temperature: " << statusPacket.battery_temperature << endl;
  cout << "Pressure: " << statusPacket.pressure << endl;
  cout << "Altitude: " << statusPacket.altitude << endl;
  cout << "Memory usage: " << statusPacket.sd_memory_usage << endl;
  cout << "Ax: " << statusPacket.accel_x << endl;
  cout << "Ay: " << statusPacket.accel_y << endl;
  cout << "Gx: " << statusPacket.giros_x << endl;
  cout << "Gy: " << statusPacket.giros_y << endl;
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