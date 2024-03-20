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
  for line in lines:
    bees = line.split()
    if len(bees) < 60:
      continue
    values = []
    for i in range(15):
      values