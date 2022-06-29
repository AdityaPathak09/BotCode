// Bluetooth Library ///////////////////////////////////////////////////////////////////
#include <BluetoothSerial.h>
BluetoothSerial bluetooth;

// Motor Library///////////////////////////////////////////////////////////////////
#include "Motor.h"

// GPS library //////////////////////////////////////////////////////////////////
#include "gps.h"

// Compass Library ////////////////////////////////////////////////////////////////
#include "compass.h"

// All variables //////////////////////////////////////////////////////////////////
float latitude;
float longitude;
String gpsData;
float compassHead;

byte latitudeArr[5];
byte longitudeArr[5];
byte compassHeadArr[3];

// Main Coding /////////////////////////////////////////////////////////////////

void sendGpsData()
{

  long lat = long(latitude * 10000000.0);
  long lon = long(longitude * 10000000.0);
  long compH = long(compassHead * 100.0);

  Serial.print("    ");
  Serial.print(lat);
  Serial.print(" ");
  Serial.print(lon);
  Serial.print(" ");
  Serial.println(compH);

  for (int i = 0; i < 5; i++)
  {
    latitudeArr[i] = byte(lat % 100);
    longitudeArr[i] = byte(lon % 100);

    Serial.print(latitudeArr[i]);
    Serial.print(" ");
    Serial.println(longitudeArr[i]);

    lat = lat / 100;
    lon = lon / 100;
  }

  for (int i = 0; i < 3; i++)
  {
    compassHeadArr[i] = byte(compH % 100);
    // Serial.println(compassHeadArr[i]);
    compH = compH / 100;
  }

  for (int i = 4; i >= 0; i--)
  {
    Serial.print(latitudeArr[i]);
    bluetooth.write(latitudeArr[i]);
  }
  Serial.println();

  delay(100);
  for (int i = 4; i >= 0; i--)
  {
    Serial.print(longitudeArr[i]);
    bluetooth.write(longitudeArr[i]);
  }
  Serial.println();

  delay(100);
  for (int i = 2; i >= 0; i--)
  {
    Serial.print(compassHeadArr[i]);
    bluetooth.write(compassHeadArr[i]);
  }

  Serial.println("data sent");
}

byte getInstruction()
{
  byte instruction = 0;
  int charector = 0;

  while (bluetooth.available())
  {
    instruction = bluetooth.read();
    charector = bluetooth.read();

    Serial.println("Instruction: " + String(instruction) + "Speed: " + String(charector));

    switch (instruction)
    {
    case 1:
      forward(charector);
      break;
    case 2:
      backard(charector);
      break;
    case 3:
      leftTurn(charector);
      break;
    case 4:
      rightTurn(charector);
      break;
    case 5:
      leftSpin(charector);
      break;
    case 6:
      rightSpin(charector);
      break;
    case 7:
      stopMotors();
      break;
    case 8:
      setCam(charector);
      Serial.println(charector);
      break;
    case 9:
      Serial.println("Sending data");
      sendGpsData();
      break;
    case 10:
      if (charector == 1)
        setPump(pump1, LOW);
      else
        setPump(pump1, HIGH);
      break;
    case 11:
      if (charector == 1)
        setPump(pump2, LOW);
      else
        setPump(pump2, HIGH);
      break;
    // case 12:
    //   setSprayerCentreHeight(charector);
    //   break;
    // case 13:
    //   setSprayerEdgeHeight(charector);
    //   break;
    default:
      instruction = 0;
      stopMotors();
      break;
    }
  }
  return instruction;
}

void setup()
{
  Serial.begin(115200);
  bluetooth.begin("pestRobo");

  setMotors();
  setGPS();
  setCompass();
}

void loop()
{
  getInstruction();
  getGps(&latitude, &longitude);
  getCompass(&compassHead);
  Serial.println(String(latitude) + " " +String(longitude) + " " +String(compassHead));
}
