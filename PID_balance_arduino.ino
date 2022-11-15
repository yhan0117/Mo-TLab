#include <Wire.h>
#include <Servo.h>
#include <JY901.h>
#define high 2000
#define low 1000

Servo right_prop;
Servo left_prop;

//System states
float angle;
float speed;

long time, timePrev;
float elapsedTime;
float PID, pwmLeft, pwmRight, error, previous_error;
float encoderPos = 0, previous_encoderPos = 0;
float dpos;

int i;

float pid_p = 0;
float pid_i = 0;
float pid_d = 0;

//Gain Coefficients
float kp = 0.5;//0.5;
float ki = 0.0034;//0.01;
float kd = 0.15;//0.2;

//Initial value of throttle to the motors
float throttle = 1300;

//Desired angle
float desired_angle = 0;


void setup() {
  Serial.begin(9600);
  right_prop.attach(10);  //attatch the right motor to pin 3
  left_prop.attach(11);   //attatch the left motor to pin 5

  time = millis();  //Start counting time in milliseconds
  //Calibrate();
  Start();
  Wire.begin();  //Begins I2C communication at pin (A4,A5)
}

void loop() {

  /*///////////////////////////Feedback///////////////////////////////////*/

  //time variables
  timePrev = time;
  time = millis();
  elapsedTime = (time - timePrev) / 1000.0;

  //IMU
  angle = (float)JY901.stcAngle.Angle[1] / 32768 * 180;
  Serial.print(angle);
  Serial.print(',');

  //Encoder
  Wire.requestFrom(8, 1);            // request 1 byte from slave arduino (8)
  byte MasterReceive = Wire.read();  // receive a byte from the slave arduino and store in MasterReceive

  //negating negative angles
  if (MasterReceive >= 128) {
    encoderPos = MasterReceive - 256;
  } else {
    encoderPos = MasterReceive;
  }

  //600 pulses per revolution
  encoderPos = encoderPos / 600 * 360;

  dpos = encoderPos - previous_encoderPos;

  //Filter, in case if encoder changes by 1 speed spikes
  if (dpos < 2 && dpos > -2) {
    dpos = 0;
  } else if (dpos >= 2) {
    dpos = (dpos - 2)*1.05;
  } else {
    dpos = (dpos + 2)*1.05;
  }
  
  speed = (encoderPos - previous_encoderPos) / elapsedTime;
  Serial.println(speed);

  previous_encoderPos = encoderPos;

  /*///////////////////////////P I D///////////////////////////////////*/

  //Error
  error = desired_angle - angle;

  //Proportional Gain
  pid_p = kp * error;

  //Integral Gain
  if (-5 < error && error < 5) {
    pid_i = pid_i + (ki * error);
  }

  //Derivative Gain
  pid_d = kd * speed;

  //Total Gain
  PID = pid_p + pid_i + pid_d;

  //Saturation
  if (PID < -200) {
    PID = -200;
  }
  if (PID > 200) {
    PID = 200;
  }

  //Calculated total actuation value
  pwmLeft = throttle + PID;
  pwmRight = throttle - PID;

  //Saturation
  //Right
  if (pwmRight < 1100) {
    pwmRight = 1100;
  }
  if (pwmRight > 1500) {
    pwmRight = 1500;
  }
  //Left
  if (pwmLeft < 1100) {
    pwmLeft = 1100;
  }
  if (pwmLeft > 1500) {
    pwmLeft = 1500;
  }

  //Actuation
  left_prop.writeMicroseconds(pwmLeft);
  right_prop.writeMicroseconds(pwmRight);
}

void serialEvent() {
  while (Serial.available()) {
    JY901.CopeSerialData(Serial.read());  //Call JY901 data cope function
  }
}

void Calibrate() {
  Serial.println("ESC calibration begin...");
  Serial.print("Writing maximum output: ");
  Serial.println(high);
  Serial.println("Plug battery in, then wait 2 seconds and press Ctrl + Enter");

  left_prop.writeMicroseconds(high);
  right_prop.writeMicroseconds(high);

  // Wait for input
  while (!Serial.available())
    ;
  Serial.read();

  // Send min output
  Serial.print("Sending minimum output: ");
  Serial.println(low);
  left_prop.writeMicroseconds(low);
  right_prop.writeMicroseconds(low);
  delay(5000);
  Serial.println("ESCs are calibrated.");
  Serial.println("Unplug and replug battery then press Ctrl + Enter");

  // Wait for input
  while (!Serial.available())
    ;
  Serial.read();

  Serial.println("Program begin...");
}

void Start() {
  Serial.print("Sending minimum output: ");
  Serial.println(low);
  left_prop.writeMicroseconds(low);
  right_prop.writeMicroseconds(low);
  Serial.println("Plug in battery then press plug in TX line");

  // Wait for input
  while (!Serial.available())
    ;
  Serial.read();

  Serial.println("Program begin...");
}