
#define motorlf 0 // channels
#define motorrf 1
#define motorlb 2
#define motorrb 3
#define motorbeup 4
#define motorbedown 5
#define motorbcup 6
#define motorbcdown 7
#define servoCam 8

#define motorPinlf 13 // pins 2, 4
#define motorPinlb 14
#define motorPinrf 27
#define motorPinrb 26
#define motorPinbeup 25
#define motorPinbedown 33
#define motorPinbcup 32
#define motorPinbcdown 23
#define servoPincam 15
#define actpote 35
#define actpotc 34
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
  int potval = map(height, 0, 127, 0, 255); // convert height to 12 bit value

  if (analogRead(actpotc) < potval)
  {
    while (analogRead(actpotc) < potval)
    {
      ledcWrite(motorbcup, offsetspeed);
      ledcWrite(motorbcdown, 0);
    }
  }
  else if (analogRead(actpotc) > potval)
  {
    while (analogRead(actpotc) > potval)
    {
      ledcWrite(motorbcup, 0);
      ledcWrite(motorbcdown, offsetspeed);
    }
  }
}

void setSprayerEdgeHeight(int height)
{
  int potval = map(height, 0, 127, 0, 255); // convert height to 12 bit value

  if (analogRead(actpote) < potval)
  {
    while (analogRead(actpote) < potval)
    {
      ledcWrite(motorbeup, offsetspeed);
      ledcWrite(motorbedown, 0);
    }
  }
  else if (analogRead(actpote) > potval)
  {
    while (analogRead(actpote) > potval)
    {
      ledcWrite(motorbeup, 0);
      ledcWrite(motorbedown, offsetspeed);
    }
  }
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
  pinMode(motorPinbeup, OUTPUT);
  pinMode(motorPinbedown, OUTPUT);
  pinMode(motorPinbcup, OUTPUT);
  pinMode(motorPinbcdown, OUTPUT);
  pinMode(servoPincam, OUTPUT);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);

  ledcSetup(motorlf, freq, resMotor);
  ledcSetup(motorrf, freq, resMotor);
  ledcSetup(motorlb, freq, resMotor);
  ledcSetup(motorrb, freq, resMotor);
  ledcSetup(motorPinbeup, freq, resMotor);
  ledcSetup(motorPinbedown, freq, resMotor);
  ledcSetup(motorbcup, freq, resMotor);
  ledcSetup(motorbcdown, freq, resMotor);
  ledcSetup(servoCam, freq, resServo);

  ledcAttachPin(motorPinlf, motorlf);
  ledcAttachPin(motorPinrf, motorrf);
  ledcAttachPin(motorPinlb, motorlb);
  ledcAttachPin(motorPinrb, motorrb);
  ledcAttachPin(motorPinbeup, motorbeup);
  ledcAttachPin(motorPinbedown, motorbedown);
  ledcAttachPin(motorPinbcup, motorbcup);
  ledcAttachPin(motorPinbcdown, motorbcdown);
  ledcAttachPin(servoPincam, servoCam);

// setSprayerCentreHeight(64);
// setSprayerEdgeHeight(64);
  setCam(90);

  setPump(pump1, HIGH);
  setPump(pump2, HIGH);
}
