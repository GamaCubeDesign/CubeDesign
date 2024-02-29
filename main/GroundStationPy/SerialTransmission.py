import time
import serial

class SerialTransmission:
  def __init__(self, app):
    self.app = app

    self.channel = None
    self.transmission_flag = False
    self.receiving_buffer = bytes()
    self.receiving_flag = False
    self.reading_binary = False
    self.listeners = dict()
    
    self.cmd = ''
    self.args = []
    
  def begin(self, port, baudrate):
    if self.channel is not None:
      if self.channel.is_open:
        self.channel.close()
    
    try:
      self.channel = serial.Serial(port=port.device,baudrate=baudrate,timeout=0.5)
    except Exception as e:
      print(e)
      return False
    return True

  def close(self):
    if self.channel is not None:
      if self.channel.is_open:
        self.channel.close()
  
  def add_listener(self, listener, key):
    self.listeners[key] = listener
  
  def update(self):
    self.read()
  
  def encode(self, message):
    out = []
    for c in message:
      # if c==10:
      #   out += [13]
      # if c==13:
      #   out += [13]
      out += [str(c)]
    return ','.join(out)
  
  def write(self, message):
    # print("Sending:")
    # self.channel.write([50, 48, 44, 49, 10])
    print(bytearray(self.encode(message)+'\n', encoding='utf8'))
    self.channel.write(bytearray(self.encode(message)+'\n', encoding='utf8'))
    # self.channel.write((self.encode(message)+'\n').encode())
    # for c in self.encode(message):#+'\n'
    #   print(ord(c))
    #   self.channel.write([ord(c)])
    # print("Done sending")
    time.sleep(0.1)
  
  def read(self):
    while self.channel.in_waiting > 0:
      b = self.channel.read()[0]
      # print(b)
      if self.receiving_flag:
        if b==10:
          self.parse_message()
          self.receiving_buffer = bytes()
        else:
          self.receiving_buffer += b.to_bytes(1,'little')
      elif b==13:
        self.receiving_flag = True
      else:
        self.receiving_buffer += b.to_bytes(1,'little')
  
  def parse_message(self):
    # print(self.receiving_buffer)
    # try:
      # print("received buffer")
      # print(self.receiving_buffer)
      msg = str(self.receiving_buffer,encoding='utf8').rstrip()
      print(msg)
      if len(msg)>2:
        v = msg.split(':')
        if v[0] in self.listeners.keys():
          cmd = v[1]
          args = v[2:]
          self.listeners[v[0]].parse_message(cmd,args)
        # else:
          # print(msg)
    # except Exception as e:
    #   print("Exception")
    #   print(e)