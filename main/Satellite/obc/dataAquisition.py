import socket
import sys

HOST = "127.0.0.1"
PORT = 8081

class Client:
  def __init__(self):
    self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    self.socket.connect((HOST, PORT))
    
    data = self.socket.recv(1024)
    r = str(data, encoding='utf-8')
    print(r)
  
  def update(self):
    print("Updating")
    self.socket.sendall(b'RequestUpdate\n\0')
    data = self.socket.recv(1024)
    
    r = str(data, encoding='utf-8')
    print("Update response: " + r)
    if r == "Close\n":
      pass
  
  def send_packet(self, index, time, battery_voltage,
      battery_current, battery_charge, battery_temperature,
      internal_temperature, external_temperature,
      sd_memory_usage):
    self.socket.sendall(b'SendPacket\n\0')
    data = self.socket.recv(1024)
    r = str(data, encoding='utf-8')
    if r=='Ok\n':
      print("Sending data")
      p = [index.to_bytes(4,'little'), time.to_bytes(4,'little'), battery_voltage.to_bytes(4,'little'),
      battery_current.to_bytes(4,'little'), battery_charge.to_bytes(4,'little'), battery_temperature.to_bytes(4,'little'),
      internal_temperature.to_bytes(4,'little'), external_temperature.to_bytes(4,'little'),
      sd_memory_usage.to_bytes(4,'little')]
      packet = bytearray(0)
      for e in p:
        packet += e
      self.socket.sendall(packet)
    else:
      print("Unknown response: " + r)

if __name__=='__main__':
  client = Client()
  client.update()
  client.send_packet(10,10,10,10,10,10,10,10,10)
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