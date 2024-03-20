import numpy as np
import matplotlib.pyplot as plt
import json

index = []
bat_voltage = []
int_temperature = []
ext_temperature = []

counter = 0
with open("ADCS_DATA_ciclagem_termica.txt", "r") as f:
  reading = True
  lines = f.readlines()
  # while reading and f.readable():
  #   s = f.readline()
  for s in lines:
    # print(s)
    # if s=="end file":
    #   reading = False
    if s.startswith("{"):
      # print(s)
      jss = s.split("}{")
      # jss = "}\n{"/
      # reading = False
      for ss in jss:
        c = ss
        # print("123")
        c = c.rstrip()
        # print(c)
        if c.endswith("}"):
          c = c[:-1]
        if c.startswith("{"):
          c = c[1:]
        # print("{" + c + "}")
        data = json.loads("{" + c + "}")
        if data['index'] != "0":
          bat_voltage += [float(data['battery_voltage'])]
          int_temperature += [float(data['internal_temperature'])]
          ext_temperature += [float(data['external_temperature'])]
          index += [counter]
          counter+=1

index = np.array(index)
bat_voltage = np.array(bat_voltage)
int_temperature = np.array(int_temperature)
ext_temperature = np.array(ext_temperature)


fig, ax = plt.subplots()
ax.plot(index, int_temperature)
ax.grid()
plt.show()