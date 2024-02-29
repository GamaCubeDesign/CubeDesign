// Open loop motor control example
#include <SimpleFOC.h>
#include <time.h>

#define TIME_ROT 4000
#define FINAL_VEL 500
// BLDC motor & driver instanceu
// BLDCMotor motor = BLDCMotor(pole pair number);
BLDCMotor motor = BLDCMotor(4);
// BLDCDriver3PWM driuver = BLDCDriver3PWM(pwmA, pwmB, pwmC, Enable(optional));
BLDCDriver3PWM driver = BLDCDriver3PWM(9, 10, 11, 8);

// Stepper motor & driver instance
//StepperMotor motor = StepperMotor(50);
//StepperDriver4PWM driver = StepperDriver4PWM(9, 5, 10, 6,  8);


//target variable
float target_velocity = 25; //rad/s
float target_int_velocity = 5;
int tempo = 10000;
int duracao = 5000;
long int ti = millis();
long int tf = millis()+tempo;
// instantiate the commander
//Commander command = Commander(Serial);
//void doTarget(char* cmd) { command.scalar(&target_velocity, cmd); }
//void doLimit(char* cmd) { command.scalar(&motor.voltage_limit, cmd); }

void setup() {

  // driver config
  // power supply voltage [V]
  driver.voltage_power_supply = 5;
  // limit the maximal dc voltage the driver can set
  // as a protection measure for the low-resistance motors
  // this value is fixed on startup
  driver.voltage_limit = 5;
  driver.init();
  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor movements
  // limit the voltage to be set to the motor
  // start very low for high resistance motors
  // current = voltage / resistance, so try to be well under 1Amp
  motor.voltage_limit = 5;   // [V]
 
  // open loop control config
  motor.controller = MotionControlType::velocity_openloop;

  // init motor hardware
  motor.init();

  // add target command T
//  command.add('T', doTarget, "target velocity");
//  command.add('L', doLimit, "voltage limit");

  Serial.begin(115200);
  Serial.println("Motor ready!");
  //Serial.println("Set target velocity [rad/s]");
  _delay(1000);

 
}

void velo(){
  ti = millis();
  int duracao = 5000;
  int tempo = 100000;
  if (ti>tf){
      tf +=tempo;
  }
  long next_iter = millis()+TIME_ROT;

//  for (int i=0; i<50;i++){
//    for(int j = 0;j==vel;j++){
//       while (millis() < tempo+duracao*i){
//        motor.move(5+10*j);
//       }
//    }
//  }
  int v = 5;
  motor.move(v);
  while(v <= FINAL_VEL){
    motor.move(v);
    if(millis() > next_iter){
      v += 5;
      next_iter = millis() + TIME_ROT;
    }
  }
  while(millis()>1){
    motor.move(FINAL_VEL);
  }
 
}

void loop() {
//  ti = millis();
//  if (ti>tf){
////    ...
//      tf +=tempo;
//  }
//  // open loop velocity movement
//  // using motor.voltage_limit and motor.velocity_limit
//  // to turn the motor "backwards", just set a negative target_velocity
//  while (millis() < tempo){
//    motor.move(5);
//  }

  
  //target_velocity = target_velocity;
  //motor.move(100);
  
  velo();
}
