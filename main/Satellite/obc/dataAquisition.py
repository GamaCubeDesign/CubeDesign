import socket
import sys

HOST = ".local"
PORT = 8081

class Client:
  def __init__(self):
    self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    self.socket.connect((HOST, PORT))
  
  def update(self):
    self.socket.sendall(b'RequestUpdate\n')
    data = self.socket.recv(1024)
    r = str(data, encoding='utf-8')
    print("Update response: " + r)
    if r == "Close\n":
      pass
  
  def send_packet(self, index, time, battery_voltage,
      battery_current, battery_charge, battery_temperature,
      internal_temperature, external_temperature,
      sd_memory_usage):
    self.socket.sendall(b'SendPacket\n')
    data = self.socket.recv(1024)
    r = str(data, encoding='utf-8')
    if r=='Ok\n':
      print("Sending data")
    else:
      print("Unknown response: " + r)

if __name__=='__main__':
  pass