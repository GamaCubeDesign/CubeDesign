// Open loop motor control example
#include <SimpleFOC.h>
#include <Wire.h>
#include <time.h>

#define TIME_ROT 4000
#define FINAL_VEL 50

BLDCMotor motor = BLDCMotor(4);
BLDCDriver3PWM driver = BLDCDriver3PWM(9, 10, 11, 8);

float STEP = (6.144/1024);
const int painel1 = A0;
const int painel2 = A1;
const int painel3 = A2;
const int painel4 = A3;
float v0max = 740; 
float v1max = 900;
float v2max = 900;
float v3max = 900;
float target_velocity = 25; //rad/s
int tempo = 10000;
long int ti = millis();
long int tf = millis()+tempo;

//variáveis do PID
float Kp = 1.0, Ki = 0.05, Kd = 0.02;

float setpoint = 0.0; // <- Angulo desejado
float measuredYaw = 0.0; // <- Medição

float previousError = 0.0;
float integral = 0.0;
float derivative = 0.0;

int target_velo = 0;
int current_velo = 0;
long next_iter = millis()+TIME_ROT;

void setup() {
  
  driver.voltage_power_supply = 5;
  driver.voltage_limit = 5;
  driver.init();
  motor.linkDriver(&driver);

  motor.voltage_limit = 5;   // [V]
 
  motor.controller = MotionControlType::velocity_openloop;

  motor.init();
  
  Serial.begin(9600);
  
  
    _delay(1000);

}

void velo(){
  // ti = millis();
  // int duracao = 5000;
  // unsigned long tempo = 70000;
  // if (ti>tf){
  //     tf +=tempo;
  // }

  // int v = 5;
  // motor.move(v);
  // while(v <= vel){
    // motor.move(v);
    if(millis() > next_iter){
      if(current_velo < target_velo){
        current_velo += 5;
      } else{
        current_velo -= 5;
      }
      next_iter = millis() + TIME_ROT;
    }
  // if(millis() <= (next_iter+tempo)){ //limitar esse tempo
  //   motor.move(vel);
  // }
 
}

void sun_pointing(float angulo){
  
  measuredYaw = angulo; // <- Medição, mudar

  
  float error = setpoint - measuredYaw;
  integral += error;
  derivative = error - previousError;

  float output = Kp * error + Ki * integral + Kd * derivative;

  //analogWrite(output) // <- velocidade para o motor

  int value = output*FINAL_VEL;
  if(value > 500){
    value = 500;
  } else if(value < -500){
    value = -500;
  }
  velo();
  previousError = error;
  Serial.write((uint8_t)((int)(value / 4)));
}

void estabilization(float vel_angular){
  vel_angular /= 1000;
  measuredYaw = vel_angular; // <- Medição---> nesse caso devemos calcular a partir do giroscópio

  float error = setpoint - measuredYaw;
  integral += error;
  derivative = error - previousError;

  float output = Kp * error + Ki * integral + Kd * derivative;

  //analogWrite(output) // <- velocidade para o motor

  int value = output*FINAL_VEL;
  if(value > 500){
    value = 500;
  } else if(value < -500){
    value = -500;
  }
  velo();
  Serial.write((uint8_t)((int)(value / 4)));
  previousError = error;
  
}

int revert(int n)
{
   int ret=0;
   int i=1;
   
   while(i<=n)
   {
       ret*=10;
       ret+=(n%(i*10)-n%i)/i;
       i*=10;
   }
   return ret;
}

int mode = 0;
int giros = 0;

void loop() {
  int count = 0;
  float adc0, adc1, adc2, adc3;
  adc0 = analogRead(painel1)*STEP;
  adc1 = analogRead(painel2)*STEP;
  adc2 = analogRead(painel3)*STEP;
  adc3 = analogRead(painel4)*STEP;
  Serial.print("AIN0: "); Serial.println(adc0);
  Serial.print("AIN1: "); Serial.println(adc1);
  Serial.print("AIN2: "); Serial.println(adc2);
  Serial.print("AIN3: "); Serial.println(adc3);
  Serial.println(" ");
  //identificar maior incidência:
  //Para determinar o vetor solar em duas dimensões em 2D utiliza-se essas duas eq.
  //𝑖+1 = 𝑖1𝑚𝑎𝑥cos a
  //𝑖+1 = 𝑖1𝑚𝑎𝑥sin a

  //Estamos usando tensão:
  //v+1 = v1𝑚𝑎𝑥cos a
  //v+2 = v2𝑚𝑎𝑥sin a
  //Inicialmente, usando a simplificação de 90 graus (sem fazer a conta dos ângulos)

  //O vetor solar nesse caso é dado por:
  //Sb = |cos a|
  //     |sin a|
  // ordenar os maiores valores de tensão medidas.

  float vetortensao[] = {adc0,adc1,adc2,adc3};
  int tamanho  = sizeof(vetortensao) / sizeof(float);
  //ordenar o os valores dentro do vetor
  for (int i = 0; i <= tamanho - 1; i++)
    {
        for (int j = i; j < tamanho - 1; j++)
        {
            if (vetortensao[i] < vetortensao[j])
            {
                int temp = vetortensao[i];
                vetortensao[i] = vetortensao[j];
                vetortensao[j] = temp;
            }
        }
    }

  //descobrir em quais faces ocorreram as maiores incidências
  String primeiro = "";
  String segundo = "";
  String maiores [] = {primeiro, segundo};
  for (int i = 0; i==1; i++){
    if (vetortensao[i] == adc0){
      maiores [i] = "adc0";
    }else if (vetortensao[i] == adc1){
      maiores [i] = "adc1";
    }else if (vetortensao[i] == adc2){
      maiores [i] = "adc2";
    }else if (vetortensao[i] == adc3){
      maiores [i] = "adc3";
    }
  }
  //Finalmente, definição do vetor solar em relação à face 0.
  float cosa;
  float sina;
  if ((maiores[0]=="adc0" && maiores[1]=="adc1")||(maiores[0]=="adc1" && maiores[1]=="adc0")){
    cosa = vetortensao[0]/v0max;
    sina = vetortensao[1]/v1max;
    //vetorsolar[] = {cosa, sina};þ
  }else if((maiores[0]=="adc1" && maiores[1]=="adc2")||(maiores[0]=="adc2" && maiores[1]=="adc1")){
    cosa = -vetortensao[0]/v1max;
    sina = vetortensao[1]/v2max;
    //vetorsolar[] = {-cosa, sina};
  }else if((maiores[0]=="adc2" && maiores[1]=="adc3")||(maiores[0]=="adc3" && maiores[1]=="adc1")){
    cosa = -vetortensao[0]/v2max;
    sina = -vetortensao[1]/v3max;
    //vetorsolar[] = {-cosa, -sina};
  }else if((maiores[0]=="adc0" && maiores[1]=="adc3")||(maiores[0]=="adc3" && maiores[1]=="adc0")){
    cosa = vetortensao[0]/v0max;
    sina = -vetortensao[1]/v3max;
   // vetorsolar[] = {cosa, -sina};
  }
  float vetorsolar[] = {cosa, sina};
  float a = acos(cosa);
  

  // delay(500);
  // open loop velocity movement
  // using motor.voltage_limit and motor.velocity_limit
  // to turn the motor "backwards", just set a negative target_velocity
//  Serial.flush();
  while(Serial.available()){
//    Serial.print("0");
    mode = Serial.read()-48;
    int g = 0;
    uint8_t b = Serial.read();
    Serial.read();
    Serial.print(b);Serial.print(" ");
    if(b==10 | b==13){
      giros = revert(g);
      Serial.println(giros);
    
      //conta para ver a velocidade angular;
    
    }else if(b=='-'){
      g *= -1;
    }else{
      g = g*10+b-48;
    }
    Serial.println(g);
  }
  
  mode = 3;
  // target_velo = 300;
  if (mode == 0){
   Serial.println("Controle desativado");
  }else if (mode == 1){//mudar para 1 ou 2 de acordo com a telemetria
    sun_pointing(a);
    // velo();
  }else if(mode == 2){
    estabilization(giros);//mudar para velocidade angular de acordo com o girsocópio
    // velo();
  } else if(mode == 3){
    target_velo = 300;
    velo();
  }
  
  velo();
  motor.move(current_velo);
}
