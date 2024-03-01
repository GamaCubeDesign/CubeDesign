from gpiozero import LED
from time import sleep

led = LED(27)

led.on()
sleep(2)
led.off()
print("antenna open")
