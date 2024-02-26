import json


with open("send.json", "r") as f:
  data = json.load(f)
  print(data["temp_bmp"])
