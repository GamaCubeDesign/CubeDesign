import socket
import sys
import json

HOST = "127.0.0.1"
PORT = 8081

#class Client:
#  def __init__(self):
#  
#  def update(self):
#  
#  def send_packet(self):

if __name__=='__main__':
  socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
  socket.connect((HOST, PORT))
  
  data = socket.recv(1024)
  r = str(data, encoding='utf-8')
  print(r)
  
  
  # print("Updating")
  # socket.sendall(b'RequestUpdate\n\0')
  # data = socket.recv(1024)
  
  r = str(data, encoding='utf-8')
  print("Update response: " + r)
  # if r == "Close\n":
  #   pass
  
  with open("send.json", "r") as f:
    data = json.load(f)
    print(data)
    
    # socket.sendall(b'SendPacket\n\0')
    # resp = socket.recv(1024)
    # r = str(resp, encoding='utf-8')
    # if r=='Ok\n':
    print("Sending data")
    p = [
      data['index'].to_bytes(4,'little'),
      data['time'].to_bytes(4,'little'),
      data['battery_voltage'].to_bytes(4,'little'),
      data['battery_current'].to_bytes(4,'little'),
      data['battery_charge'].to_bytes(4,'little'),
      data['battery_temperature'].to_bytes(4,'little'),
      data['internal_temperature'].to_bytes(4,'little'),
      data['external_temperature'].to_bytes(4,'little'),
      data['sd_memory_usage'].to_bytes(4,'little'),
      data['altitude_bmp'].to_bytes(4,'little'),
      data['pressure_bmp'].to_bytes(4,'little'),
      data['Ax_mpu'].to_bytes(4,'little'),
      data['Ay_mpu'].to_bytes(4,'little'),
      data['Az_mpu'].to_bytes(4,'little'),
      data['Gx_mpu'].to_bytes(4,'little'),
      data['Gy_mpu'].to_bytes(4,'little'),
      data['Gz_mpu'].to_bytes(4,'little')
      ]
    packet = bytearray(0)
    for e in p:
      packet += e
    socket.sendall(packet)
    # else:
    #   print("Unknown response: " + r)
  # index = 10
  # time = 10
  # battery_voltage = 10
  # battery_current = 10
  # battery_charge = 10
  # battery_temperature = 10
  # internal_temperature = 10
  # external_temperature = 10
  # sd_memory_usage = 10
  # p = [index.to_bytes(4,'little'), time.to_bytes(4,'little'), battery_voltage.to_bytes(4,'little'),
  #     battery_current.to_bytes(4,'little'), battery_charge.to_bytes(4,'little'), battery_temperature.to_bytes(4,'little'),
  #     internal_temperature.to_bytes(4,'little'), external_temperature.to_bytes(4,'little'),
  #     sd_memory_usage.to_bytes(4,'little')]
  # packet = bytearray(0)
  # for e in p:
  #   packet += e
  # print(p)
  # print(packet)
