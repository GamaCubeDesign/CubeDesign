#ifndef COMMUNICATION_PROTOCOL_H
#define COMMUNICATION_PROTOCOL_H

enum PROTOCOL{
  PROTOCOL_STATUS,
  PROTOCOL_IMAGING_DATA,
  PROTOCOL_SET_OPERATION,
  PROTOCOL_PING
};

enum PROTOCOL_STATUS{
  GS_STATUS_REQUEST,
  SATELLITE_STATUS_PACKETS_AVAILABLE,
  GS_STATUS_START_TRANSMISSION,
  SATELLITE_STATUS_PACKET,
  SATELLITE_STATUS_PACKETS_DONE,
  GS_STATUS_RESEND_PACKET,
  GS_STATUS_DONE,
  SATELLITE_STATUS_DONE
};

enum PROTOCOL_IMAGING_DATA{
  GS_IMAGING_REQUEST,
  SATELLITE_IMAGING_PACKETS_AVAILABLE,
  GS_IMAGING_START_TRANSMISSION,
  SATELLITE_IMAGING_PACKET,
  SATELLITE_IMAGING_PACKETS_DONE,
  GS_IMAGING_RESEND_STATUS,
  GS_IMAGING_DONE,
  SATELLITE_IMAGING_DONE
};

enum PROTOCOL_SET_OPERATION{
    GS_SET_OPERATION,
    SATELLITE_SET_OPERATION_ECHO,
    GS_SET_OPERATION_DONE,
    SATELLITE_SET_OPERATION_DONE
};

enum PROTOCOL_PING{
  GS_PING_REQUEST,
  SATELLITE_PING_RESPONSE
};

enum CONTROL_COMMAND{
  READ_RX_ADDH,
  READ_RX_ADDL,
  READ_TX_ADDH,
  READ_TX_ADDL,
  READ_BANDWIDTH,
  READ_FREQUENCY,
  READ_SPI_FREQUENCY,
  READ_SPREADING_FACTOR,
  READ_TRANSMISSION_POWER,
  READ_ALL,

  SET_RX_ADDH,
  SET_RX_ADDL,
  SET_TX_ADDH,
  SET_TX_ADDL,
  SET_BANDWIDTH,
  SET_FREQUENCY,
  SET_SPI_FREQUENCY,
  SET_SPREADING_FACTOR,
  SET_TRANSMISSION_POWER,
  SET_ALL,

  TOGGLE_QUERY_STATUS,
  SEND_QUERY_STATUS,
  TOGGLE_QUERY_IMAGING,
  SEND_QUERY_IMAGING,
  TOGGLE_COMMAND,
  SEND_COMMAND,

  FLUSH,
  SET_MESSAGE_DATA,

  SET_ACTIVE_THERMAL_CONTROL,
  SET_ATTITUDE_CONTROL,
  SET_IMAGING,
  SET_IMAGING_MODE,
  SET_STAND_BY_MODE,

  PING
};

struct ProtocolOperation{
    uint8_t protocol : 2;
    uint8_t operation : 6;
};

struct GSOperation{
  uint8_t switch_active_thermal_control : 1;
  uint8_t switch_attitude_control : 1;
  uint8_t switch_imaging : 1;
  uint8_t switch_imaging_mode : 1;
  uint8_t switch_stand_by_mode : 1;
};

// struct GSCommand{
//   uint8_t length;
//   GSCommand_b1 b1;
// };

struct GSResend{
  bool isDone;
  uint8_t packets[32];
};

union GSData{
  GSOperation operation;
  GSResend resend;
};

struct GSPacket{
  uint8_t length;
  ProtocolOperation operation;
  GSData data;
};

#define N 10 // Size of the added array
struct HealthData{
//   uint8_t length;
//   uint8_t type;
  uint32_t index;
  uint32_t time;
  float battery_voltage;
  float battery_current;
  float battery_charge;
  float battery_temperature;
  float internal_temperature;
  float external_temperature;
  uint32_t sd_memory_usage; // MB
  // uint8_t rasp_data[N];
};

struct LightningData{
  uint32_t type;
  uint64_t index;
  float x;
  float y;
  float radius;
  float duration;
};

struct ImagingData{
//   uint8_t length;
//   uint8_t type;
  LightningData lightnings[5];
};

// struct TelemetryPacket{
//   uint8_t index;
//   uint8_t data[56];
// };

union SatData{
  HealthData healthData;
  ImagingData imagingData;
  GSOperation operation_echo;
};

union ByteData{
  uint8_t index;
  uint8_t number_of_packets;
  uint8_t byte;
};

struct SatPacket{
  uint8_t length;
  ProtocolOperation operation;
  ByteData byte_data;
  uint8_t padding_0[5];
  SatData data;
};

struct Operation{
  uint8_t switch_active_thermal_control : 1;
  uint8_t switch_attitude_control : 1;
  uint8_t switch_imaging : 1;
  uint8_t switch_imaging_mode : 1;
  uint8_t switch_stand_by_mode : 1;
};

// extern Message message;
// extern TelemetryData telemetryData;
extern SatPacket satPacket;//int defsatPacketSize = sizeof(SatPacket);
extern GSPacket gsPacket;//int defgsPacketSize = sizeof(GSPacket);
extern Operation operation;//int defoperationSize = sizeof(Operation);

// int temp_var0 = sizeof(SatPacket);
// int temp_var1 = sizeof(uint8_t);
// int temp_var2 = sizeof(ProtocolOperation);

#endif
