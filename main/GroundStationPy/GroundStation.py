import tkinter as tk
import tkinter.filedialog
from tkinter import ttk

import serial

import Protocol
protocol = Protocol.get_protocol()

class GroundStation(ttk.LabelFrame):
  def __init__(self, root, serial):
    tk.LabelFrame.__init__(self, master=root, text="Ground Station Control")
    self.root = root

    self.ser = serial
    
    self.ser.add_listener(self, 'GS')

    self.init_variables()
    self.init_layout()

  def init_variables(self):
    self.toggle_status_variable = tk.StringVar(value='OFF')
    self.toggle_imaging_data_variable = tk.StringVar(value='OFF')
    self.toggle_set_operation_variable = tk.StringVar(value='OFF')
    
    self.toggle_active_thermal_control_variable = tk.StringVar(value='OFF')
    self.toggle_attitude_control_variable = tk.StringVar(value='OFF')
    self.toggle_imaging_variable = tk.StringVar(value='OFF')
    self.toggle_imaging_mode_variable = tk.StringVar(value='1')
    self.toggle_stand_by_mode_variable = tk.StringVar(value='OFF')
    
    self.control_option = tk.StringVar(value="1")
  
  def init_layout(self):
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Status')
    top_frame.pack(side='top',fill='both')
    top_left_frame = ttk.LabelFrame(master=top_frame, text='Toggle')
    top_left_frame.pack(side='left',fill='both')
    tk.Button(master=top_left_frame, textvariable=self.toggle_status_variable, relief='raised', command=self.toggle_status).pack(side='left',fill='both')
    tk.Button(master=top_frame, text='SEND', relief='raised', command=self.send_status).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Imaging Data')
    top_frame.pack(side='top',fill='both')
    top_left_frame = ttk.LabelFrame(master=top_frame, text='Toggle')
    top_left_frame.pack(side='left',fill='both')
    tk.Button(master=top_left_frame, textvariable=self.toggle_imaging_data_variable, relief='raised', command=self.toggle_imaging_data).pack(side='left',fill='both')
    tk.Button(master=top_frame, text='SEND', relief='raised', command=self.send_imaging_data).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Set Operation')
    top_frame.pack(side='top',fill='both')
    top_left_frame = ttk.LabelFrame(master=top_frame, text='Toggle')
    top_left_frame.pack(side='left',fill='both')
    tk.Button(master=top_left_frame, textvariable=self.toggle_set_operation_variable, relief='raised', command=self.toggle_set_operation).pack(side='left',fill='both')
    tk.Button(master=top_frame, text='SEND', relief='raised', command=self.send_set_operation).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Ping')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, text='Send ping', relief='raised', command=self.send_ping).pack(side='left',fill='both')
    
    left_frame = ttk.Frame(master=self)
    left_frame.pack(side='left', fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Active thermal control')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.toggle_active_thermal_control_variable, relief='raised', command=self.toggle_active_thermal_control).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Attitude control')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.toggle_attitude_control_variable, relief='raised', command=self.toggle_attitude_control).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Attitude control option')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.control_option, relief='raised', command=self.toggle_attitude_control_option).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Imaging')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.toggle_imaging_variable, relief='raised', command=self.toggle_imaging).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Imaging mode')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.toggle_imaging_mode_variable, relief='raised', command=self.toggle_imaging_mode).pack(side='left',fill='both')

    top_frame = ttk.LabelFrame(master=left_frame, text='Stand by mode')
    top_frame.pack(side='top',fill='both')
    tk.Button(master=top_frame, textvariable=self.toggle_stand_by_mode_variable, relief='raised', command=self.toggle_stand_by_mode).pack(side='left',fill='both')
  
  def begin(self, ser):
    self.ser = ser
    self.pack(fill='both')

  def close(self):
    self.pack_forget()
  
  def parse_message(self, cmd, args):
    pass

  def toggle_status(self):
    if self.toggle_status_variable.get() == 'ON':
      self.status_flag = 0
      self.toggle_status_variable.set('OFF')
    elif self.toggle_status_variable.get() == 'OFF':
      self.status_flag = 1
      self.toggle_status_variable.set('ON')
    self.ser.write([protocol['control_commands'].index("TOGGLE_QUERY_STATUS"), self.status_flag])
  
  def send_status(self):
    self.ser.write([protocol['control_commands'].index("SEND_QUERY_STATUS")])

  def toggle_imaging_data(self):
    if self.toggle_imaging_data_variable.get() == 'ON':
      self.imaging_data_flag = 0
      self.toggle_imaging_data_variable.set('OFF')
    elif self.toggle_imaging_data_variable.get() == 'OFF':
      self.imaging_data_flag = 1
      self.toggle_imaging_data_variable.set('ON')
    self.ser.write([protocol['control_commands'].index("TOGGLE_QUERY_IMAGING"), self.imaging_data_flag])
  
  def send_imaging_data(self):
    self.ser.write([protocol['control_commands'].index("SEND_QUERY_IMAGING")])

  def toggle_set_operation(self):
    if self.toggle_set_operation_variable.get() == 'ON':
      self.set_operation_flag = 0
      self.toggle_set_operation_variable.set('OFF')
    elif self.toggle_set_operation_variable.get() == 'OFF':
      self.set_operation_flag = 1
      self.toggle_set_operation_variable.set('ON')
    self.ser.write([protocol['control_commands'].index("TOGGLE_COMMAND"), self.set_operation_flag])
  
  def send_set_operation(self):
    self.ser.write([protocol['control_commands'].index("SEND_COMMAND")])
  
  def send_ping(self):
    self.ser.write([protocol['control_commands'].index("SEND_PING")])
  
  def toggle_active_thermal_control(self):
    if self.toggle_active_thermal_control_variable.get() == 'OFF':
      print("Toggling active thermal control OFF")
      self.toggle_active_thermal_control_variable.set('ON')
      self.ser.write([protocol['control_commands'].index("SET_ACTIVE_THERMAL_CONTROL"), 0])
    elif self.toggle_active_thermal_control_variable.get() == 'ON':
      print("Toggling active thermal control ON")
      self.toggle_active_thermal_control_variable.set('OFF')
      self.ser.write([protocol['control_commands'].index("SET_ACTIVE_THERMAL_CONTROL"), 1])
  
  def toggle_attitude_control(self):
    if self.toggle_attitude_control_variable.get() == 'OFF':
      self.toggle_attitude_control_variable.set('ON')
      if self.toggle_attitude_control_option.get()=="1":
        f = 1
      elif self.toggle_attitude_control_option.get()=="2":
        f = 2
    elif self.toggle_attitude_control_variable.get() == 'ON':
      self.toggle_attitude_control_variable.set('OFF')
      f = 0
    self.ser.write([protocol['control_commands'].index("SET_ATTITUDE_CONTROL"), f])
  
  def toggle_attitude_control_option(self):
    if self.toggle_attitude_control_option.get() == "1":
      self.toggle_attitude_control_option.set("2")
    elif self.toggle_attitude_control_option.get() == "2":
      self.toggle_attitude_control_option.set("1")
  
  def toggle_imaging(self):
    if self.toggle_imaging_variable.get() == 'OFF':
      self.toggle_imaging_variable.set('ON')
      f = 1
    elif self.toggle_imaging_variable.get() == 'ON':
      self.toggle_imaging_variable.set('OFF')
      f = 0
    self.ser.write([protocol['control_commands'].index("SET_IMAGING"), f])
  
  def toggle_imaging_mode(self):
    if self.toggle_imaging_mode_variable.get() == '1':
      self.toggle_imaging_mode_variable.set('2')
      f = 1
    elif self.toggle_imaging_mode_variable.get() == '2':
      self.toggle_imaging_mode_variable.set('1')
      f = 0
    self.ser.write([protocol['control_commands'].index("SET_IMAGING_MODE"), f])
  
  def toggle_stand_by_mode(self):
    if self.toggle_stand_by_mode_variable.get() == 'OFF':
      self.toggle_stand_by_mode_variable.set('ON')
      f = 1
    elif self.toggle_stand_by_mode_variable.get() == 'ON':
      self.toggle_stand_by_mode_variable.set('OFF')
      f = 0
    self.ser.write([protocol['control_commands'].index("SET_STAND_BY_MODE"), f])