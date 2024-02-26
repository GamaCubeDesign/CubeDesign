import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import json

from enum import Enum

import Protocol
protocol = Protocol.get_protocol()

# class Comm:
class ControlFrame(ttk.LabelFrame):
  def __init__(self, root, serial):
    tk.LabelFrame.__init__(self, master=root, text="Radio Control")
    self.root = root

    self.configuration = {
      "Head": None,
      "ADDH": None,
      "ADDL": None,
      "CHAN": None,
      "UART parity bit": None,
      "TTL UART baud rate (bps)": None,
      "Air data rate (bps)": None,
      "Transmission mode": None,
      "Transmission power": None
    }

    self.ser = serial
    
    self.ser.add_listener(self, 'CONTROL')

    self.init_variables()
    self.init_layout()
  
  def init_variables(self):
    self.rx_addh_variable = tk.StringVar()
    self.rx_addl_variable = tk.StringVar()
    self.tx_addh_variable = tk.StringVar()
    self.tx_addl_variable = tk.StringVar()
    
    self.frequency_variable = tk.StringVar()
    self.spi_frequency_variable = tk.StringVar()
    
    self.idle_variable = tk.StringVar()
    self.sleep_variable = tk.StringVar()
    
    self.spreading_factor_variable = tk.StringVar()
    self.bandwidth_variable = tk.StringVar()
    self.coding_rate_variable = tk.StringVar()
    self.preamble_length_variable = tk.StringVar()
    self.sync_word_variable = tk.StringVar()
    self.crc_variable = tk.StringVar()
    self.tx_power_variable = tk.StringVar()

  def init_layout(self):
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Rx ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.rx_addh_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_rx_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_rx_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text=" Rx ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.rx_addl_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_rx_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_rx_addl).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Tx ADDH")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.tx_addh_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_addh).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_addh).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text=" Tx ADDL")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.tx_addl_variable,width=4,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_addl).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_addl).pack(side='left',fill='both')

    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Frequency")
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.frequency_variable,width=16,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_frequency).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_frequency).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame,text="Bandwidth")
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['bandwidth_dict'].keys()), textvariable=self.bandwidth_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_bandwidth).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_bandwidth).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='SPI frequency')
    top_frame.pack(side='top',fill='both')
    ttk.Entry(master=top_frame,textvariable=self.spi_frequency_variable,width=16,justify='center').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_spi_frequency).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_spi_frequency).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Spreading factor')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['spreading_factor_dict'].keys()), textvariable=self.spreading_factor_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_spreading_factor).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_spreading_factor).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Transmission power')
    top_frame.pack(side='top',fill='both')
    ttk.Combobox(master=top_frame, values=list(protocol['transmission_power_dict'].keys()), textvariable=self.tx_power_variable, state='readonly').pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="read",command=self.read_tx_power).pack(side='left',fill='both')
    ttk.Button(master=top_frame,text="set",command=self.set_tx_power).pack(side='left',fill='both')
  
  def begin(self, serial):
      self.ser = serial
      self.pack(fill='both')
  
  def close(self):
    self.pack_forget()
  
  def key_from_value(self, d, v):
    for k in d.keys():
      if d[k] == v:
        return k
    
    return None
  
  def parse_message(self, cmd:str, args:list):
    print("Cmd: " + cmd)
    print("Args")
    print(args[0:])
    if cmd=="0":#Device initiated successfully
      self.on_device_ready()
    elif cmd == "1":#RX_ADDH
      self.rx_addh_variable.set(hex(int(args[0], 16))[2:].upper())
    elif cmd == "2":#RX_ADDL
      self.rx_addl_variable.set(hex(int(args[0], 16))[2:].upper())
    elif cmd == "3":#TX_ADDH
      self.tx_addh_variable.set(hex(int(args[0], 16))[2:].upper())
    elif cmd == "4":#TX_ADDL
      self.tx_addl_variable.set(hex(int(args[0], 16))[2:].upper())
    elif cmd == "5":#BANDWIDTH
      sbw = list(protocol["bandwidth_dict"].keys())[int(args[0])]
      self.bandwidth_variable.set(sbw)
    elif cmd == "6":#FREQUENCY
      self.frequency_variable.set(args[0][:-3])
    elif cmd == "7":#SPI_FREQUENCY
      self.spi_frequency_variable.set(args[0][:-3])
    elif cmd == "8":#SPREADING_FACTOR
      self.spreading_factor_variable.set(args[0])
    elif cmd == "9":#TXPW
      self.tx_power_variable.set(self.key_from_value(protocol["transmission_power_dict"], int(args[0])))
    elif cmd == "STATUS PACKET":
      data = dict(time = args[0],
                  index = args[1],
                  sd_memory_usage = args[2],
                  internal_temperature = args[3],
                  external_temperature = args[4],
                  battery_charge = args[5],
                  battery_voltage = args[6],
                  battery_current = args[7],
                  battery_temperature = args[8],
                  accel_x = args[9],
                  accel_y = args[10],
                  giro_x = args[11],
                  giro_y = args[12])
      with open("ADCS_DATA.txt", "a") as archive:
        json.dump(archive, data)
        
    else:
      print(cmd + " " + args[0:])

  def on_device_ready(self):
    self.read_rx_addh()
    self.read_rx_addl()
    self.read_tx_addh()
    self.read_tx_addl()
    self.read_bandwidth()
    self.read_frequency()
    self.read_spi_frequency()
    self.read_spreading_factor()
    self.read_tx_power()
  
  def set_rx_addh(self):
    self.ser.write([protocol['control_commands'].index("SET_RX_ADDH"), int(self.rx_addh_variable.get(), 16)])
  def read_rx_addh(self):
    self.ser.write([protocol['control_commands'].index("READ_RX_ADDH")])
  
  def set_rx_addl(self):
    self.ser.write([protocol['control_commands'].index("SET_RX_ADDL"), int(self.rx_addl_variable.get(), 16)])
  def read_rx_addl(self):
    self.ser.write([protocol['control_commands'].index("READ_RX_ADDL")])
  
  def set_tx_addh(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_ADDH"), int(self.tx_addh_variable.get(), 16)])
  def read_tx_addh(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_ADDH")])
  
  def set_tx_addl(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_ADDL"), int(self.tx_addl_variable.get(), 16)])
  def read_tx_addl(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_ADDL")])
  
  def set_bandwidth(self):
    self.ser.write([protocol['control_commands'].index("SET_BANDWIDTH"), protocol['bandwidth_dict'][self.bandwidth_variable.get()]])
  def read_bandwidth(self):
    self.ser.write([protocol['control_commands'].index("READ_BANDWIDTH")])
  
  def set_frequency(self):
    self.ser.write([protocol['control_commands'].index("SET_FREQUENCY")] + [int(i)+48 for i in self.frequency_variable.get()] + [ord(':')])
  def read_frequency(self):
    self.ser.write([protocol['control_commands'].index("READ_FREQUENCY")])
  
  def set_spi_frequency(self):
    self.ser.write([protocol['control_commands'].index("SET_SPI_FREQUENCY")] + [int(i)+48 for i in self.spi_frequency_variable.get()] + [ord(':')])
  def read_spi_frequency(self):
    self.ser.write([protocol['control_commands'].index("READ_SPI_FREQUENCY")])
  
  def set_spreading_factor(self):
    self.ser.write([protocol['control_commands'].index("SET_SPREADING_FACTOR"), protocol['spreading_factor_dict'][self.spreading_factor_variable.get()]])
  def read_spreading_factor(self):
    self.ser.write([protocol['control_commands'].index("READ_SPREADING_FACTOR")])
  
  def set_tx_power(self):
    self.ser.write([protocol['control_commands'].index("SET_TX_POWER"), protocol['transmission_power_dict'][self.tx_power_variable.get()]])
  def read_tx_power(self):
    self.ser.write([protocol['control_commands'].index("READ_TX_POWER")])