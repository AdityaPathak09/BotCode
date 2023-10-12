
#define motorlf 0 // channels
#define motorrf 1
#define motorlb 2
#define motorrb 3
#define servobe 4
#define servobc 5
#define servoCam 6

#define motorPinlf 13 // pins 2, 4
#define motorPinlb 14
#define motorPinrf 27
#define motorPinrb 26
#define servoPinBackE 25
#define servoPinBackC 33
#define servoPincam 15
#define pump1 5
#define pump2 18

#define servoMinDS 6  //102 //6
#define servoMaxDS 32  //512 //32
#define freq 50
#define resMotor 8
#define resServo 8   //12 //8
#define offset 100 // only for actuator
#define offsetspeed 175

void forward(int speed) // speed between [0, 255]
{
  ledcWrite(motorlf, speed);
  ledcWrite(motorrf, speed);
  ledcWrite(motorlb, 0);
  ledcWrite(motorrb, 0);
}

void backard(int speed)
{
  ledcWrite(motorlf, 0);
  ledcWrite(motorrf, 0);
  ledcWrite(motorlb, speed);
  ledcWrite(motorrb, speed);
}

void leftTurn(int speed)
{
  ledcWrite(motorlf, 0);
  ledcWrite(motorrf, speed);
  ledcWrite(motorlb, 0);
  ledcWrite(motorrb, 0);
}

void rightTurn(int speed)
{
  ledcWrite(motorlf, speed);
  ledcWrite(motorrf, 0);
  ledcWrite(motorlb, 0);
  ledcWrite(motorrb, 0);
}

void leftSpin(int speed)
{
  ledcWrite(motorlf, 0);
  ledcWrite(motorrf, speed);
  ledcWrite(motorlb, speed);
  ledcWrite(motorrb, 0);
}

void rightSpin(int speed)
{
  ledcWrite(motorlf, speed);
  ledcWrite(motorrf, 0);
  ledcWrite(motorlb, 0);
  ledcWrite(motorrb, speed);
}

void stopMotors()
{
  ledcWrite(motorlf, 0);
  ledcWrite(motorrf, 0);
  ledcWrite(motorlb, 0);
  ledcWrite(motorrb, 0);
}

void setCam(int angle)
{
  angle = map(angle, 0, 180, servoMinDS, servoMaxDS);
  ledcWrite(servoCam, angle);
  delay(15);
}

void setSprayerCentreHeight(int height)
{
  height = map(height, 0, 180, servoMinDS, servoMaxDS);
  ledcWrite(servobc, height); 
  delay(15);
}

void setSprayerEdgeHeight(int height)
{
  height = map(height, 0, 180, servoMinDS, servoMaxDS);
  ledcWrite(servobe, height);
  delay(15);
}

void setPump(int pump, boolean state)
{
  digitalWrite(pump, state);
}

void setMotors()
{
  pinMode(motorPinlf, OUTPUT);
  pinMode(motorPinrf, OUTPUT);
  pinMode(motorPinlb, OUTPUT);
  pinMode(motorPinrb, OUTPUT);
  pinMode(servoPinBackE, OUTPUT);
  pinMode(servoPinBackC, OUTPUT);
  pinMode(servoPincam, OUTPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);

  ledcSetup(motorlf, freq, resMotor);
  ledcSetup(motorrf, freq, resMotor);
  ledcSetup(motorlb, freq, resMotor);
  ledcSetup(motorrb, freq, resMotor);
  ledcSetup(servobc, freq, resMotor);
  ledcSetup(servobe, freq, resMotor);
  ledcSetup(servoCam, freq, resServo);

  ledcAttachPin(motorPinlf, motorlf);
  ledcAttachPin(motorPinrf, motorrf);
  ledcAttachPin(motorPinlb, motorlb);
  ledcAttachPin(motorPinrb, motorrb);
  ledcAttachPin(servoPinBackC, servobc);
  ledcAttachPin(servoPinBackE, servobe);
  ledcAttachPin(servoPincam, servoCam);

// setSprayerCentreHeight(64);
// setSprayerEdgeHeight(64);
  setCam(90);
  setSprayerCentreHeight(180);
  setSprayerEdgeHeight(180);

  setPump(pump1, HIGH);
  setPump(pump2, HIGH);

  Serial.println("Motors Set");
}
