from machine import I2C, Pin, SoftI2C
from machine import ADC
from machine import Pin
import time
import utime
from machine import I2C, Pin
#from mpu9250 import MPU9250
import network
import urequests
from bmp280 import *
import json
import imu
import machine
from micropython import const
from mpu6500 import MPU6500
from ak8963 import AK8963
from machine import RTC

# Used for enabling and disabling the I2C bypass access
_INT_PIN_CFG = const(0x37)
_I2C_BYPASS_MASK = const(0b00000010)
_I2C_BYPASS_EN = const(0b00000010)
_I2C_BYPASS_DIS = const(0b00000000)

class MPU9250:
    """Class which provides interface to MPU9250 9-axis motion tracking device."""
    def __init__(self, i2c, mpu6500 = None, ak8963 = None):
        if mpu6500 is None:
            self.mpu6500 = MPU6500(i2c)
        else:
            self.mpu6500 = mpu6500

        # Enable I2C bypass to access AK8963 directly.
        char = self.mpu6500._register_char(_INT_PIN_CFG)
        char &= ~_I2C_BYPASS_MASK # clear I2C bits
        char |= _I2C_BYPASS_EN
        self.mpu6500._register_char(_INT_PIN_CFG, char)

        if ak8963 is None:
            self.ak8963 = AK8963(i2c)
        else:
            self.ak8963 = ak8963

    @property
    def acceleration(self):
        """
        Acceleration measured by the sensor. By default will return a
        3-tuple of X, Y, Z axis values in m/s^2 as floats. To get values in g
        pass `accel_fs=SF_G` parameter to the MPU6500 constructor.
        """
        return self.mpu6500.acceleration

    @property
    def gyro(self):
        """
        Gyro measured by the sensor. By default will return a 3-tuple of
        X, Y, Z axis values in rad/s as floats. To get values in deg/s pass
        `gyro_sf=SF_DEG_S` parameter to the MPU6500 constructor.
        """
        return self.mpu6500.gyro

    @property
    def temperature(self):
        """
        Die temperature in celcius as a float.
        """
        return self.mpu6500.temperature

    @property
    def magnetic(self):
        """
        X, Y, Z axis micro-Tesla (uT) as floats.
        """
        return self.ak8963.magnetic

    @property
    def whoami(self):
        return self.mpu6500.whoami

    def __enter__(self):
        return self

    def __exit__(self, exception_type, exception_value, traceback):
        pass


sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
i2c=I2C(scl=Pin(22), sda=Pin(21))
sensor = MPU9250(i2c)  
bus = I2C(scl=Pin(22),sda=Pin(21))
bmp280 = BMP280 (bus)
adc35=ADC(Pin(35))


def internet ():
  sta_if.disconnect()
  sta_if.scan()
  sta_if.connect('PP2LA','px9k1247')
  print("Waiting for Wifi connection")
  while not sta_if.isconnected(): time.sleep(1)
  print("Connected")

def sht20_temperature():
  i2c.writeto(0x40,b'\xf3')
  time.sleep_ms(70)
  t=i2c.readfrom(0x40, 2)
  return -46.86+175.72*(t[0]*256+t[1])/65535

def sht20_humidity():
  i2c=I2C(scl=Pin(22), sda=Pin(21))
  i2c.writeto(0x40,b'\xf5')
  time.sleep_ms(70)
  t=i2c.readfrom(0x40, 2)
  return -6+125*(t[0]*256+t[1])/65535
    
def acelerometro():
  #print("MPU9250 id: " + hex(sensor.whoami))
  #print(sensor.acceleration)
  #print(sensor.gyro)
  #print(sensor.magnetic)
  retorno = str(sensor.acceleration + sensor.gyro + sensor.magnetic)
  return retorno
         
def pressao():
  bmp280.use_case(BMP280_CASE_WEATHER)
  bmp280.oversample(BMP280_OS_HIGH)
  bmp280.normal_measure()
  pressao = str(bmp280.pressure)
  bmp280.sleep()
  
  #for count in range(10):
   # bmp280.normal_measure()
   # print(str(bmp280.pressure))
   # bmp280.sleep()
    #time.sleep_ms(1)
  return pressao
  
  
def bateria():
  adc35.atten(ADC.ATTN_11DB)
  adc35.width(ADC.WIDTH_12BIT)
  bateria_nivel = adc35.read()*100 / 2600
  return bateria_nivel
  

def vibracao():
  imu.init_MPU()
  imu.calibrate_sensors()
  return (''.join([str(x) for x in [imu.read_mpu6050v(1), ',', imu.read_mpu6050v(2), ', ', imu.read_mpu6050v(3)]]))

def hora():
    hora = None
    rtc = RTC()
    rtc.datetime((2023,1,500,0,9,21,0,0))
    hora = (rtc.datetime())[2]
    return hora
#internet()

time.sleep(3)

sdcard=machine.SDCard(slot=2, width=1, cd=None, wp=None, sck=Pin(18), miso=Pin(19), mosi=Pin(23), cs=Pin(15), freq=20000000)
os.mount(sdcard, '/sd')

while True:
  humidade = sht20_humidity()
  temperatura = sht20_temperature()
  aceleracao = acelerometro()
  valor_pressao = pressao()
  nivel_bat = bateria()
  horario = hora()    
  vibr = vibracao()

  dados_dic = {
    "equipe": 41,
    "bateria": nivel_bat,
    "temperatura": temperatura,
    "pressao": valor_pressao,
    "acelerometro e giroscopio": aceleracao,
    "payload": {
     horario,vibr
    }
  }

  print (dados_dic)  

  arquivos = open('/sd/file.txt', 'a') #se quiser remover o binrio, basta trocar 'ba' por 'a'

  dados_salvos = str(dados_dic)
  arquivos.write(dados_salvos)
  arquivos.write('\n')
  arquivos.close()

  time.sleep(3)