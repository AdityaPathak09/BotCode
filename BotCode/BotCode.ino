// Bluetooth Library ///////////////////////////////////////////////////////////////////
#include <BluetoothSerial.h>
BluetoothSerial bluetooth;

// Motor Library///////////////////////////////////////////////////////////////////
#include "Motor.h"

// GPS library //////////////////////////////////////////////////////////////////
#include "gps.h"

// Compass Library ////////////////////////////////////////////////////////////////
#include "compass.h"

// GSM Library ////////////////////////////////////////////////////////////////
#include "gsm.h"

// All variables //////////////////////////////////////////////////////////////////
float latitude;
float longitude;
String gpsData;
float compassHead;

byte latitudeArr[5];
byte longitudeArr[5];
byte compassHeadArr[3];

// pinMap ////////////////////////////////////////////////////////////////////////
#define camTrig 19

// interrupt ///////////////////////////////////////////////////////////////////
// bool Battery1 = true;

// void IRAM_ATTR ISR()
// {
//   delay(1000);
//   if (digitalRead(34))
//   {
//     Battery1 = !Battery1;

//     if (Battery1)
//     {
//       digitalWrite(2, HIGH);
//     }
//     else
//     {
//       digitalWrite(2, LOW);
//     }
//   }
// }
// Main Coding /////////////////////////////////////////////////////////////////

void sendGpsData()
{
  getGps(&latitude, &longitude);
  getCompass(&compassHead);
  // Serial.println("Comass:" +String(compassHead));

  long lat = long(latitude * 10000000.0);
  long lon = long(longitude * 10000000.0);
  long compH = long(compassHead * 100.0);

  // Serial.print("    ");
  // Serial.print(lat);
  // Serial.print(" ");
  // Serial.print(lon);
  // Serial.print(" ");
  // Serial.println(compH);

  for (int i = 0; i < 5; i++)
  {
    latitudeArr[i] = byte(lat % 100);
    longitudeArr[i] = byte(lon % 100);

    // Serial.print(latitudeArr[i]);
    // Serial.print(" ");
    // Serial.println(longitudeArr[i]);

    lat = lat / 100;
    lon = lon / 100;
  }

  for (int i = 0; i < 3; i++)
  {
    compassHeadArr[i] = byte(compH % 100);
    // // Serial.println(compassHeadArr[i]);
    compH = compH / 100;
  }

  for (int i = 4; i >= 0; i--)
  {
    // // Serial.print(latitudeArr[i]);
    bluetooth.write(latitudeArr[i]);
  }
  // // Serial.println();

  delay(100);
  for (int i = 4; i >= 0; i--)
  {
    // // Serial.print(longitudeArr[i]);
    bluetooth.write(longitudeArr[i]);
  }
  // Serial.println();

  delay(100);
  for (int i = 2; i >= 0; i--)
  {
    // Serial.print(compassHeadArr[i]);
    bluetooth.write(compassHeadArr[i]);
  }

  // // Serial.println("data sent");
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
      // Serial.println(String(compassHead));
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
    case 12:
      setSprayerCentreHeight(charector);
      break;
    case 13:
      setSprayerEdgeHeight(charector);
      break;
    case 14:
      digitalWrite(camTrig, LOW);
      delay(250);
      digitalWrite(camTrig, HIGH);
      break;
    default:
      instruction = 0;
      stopMotors();
      break;
    }
  }
  return instruction;
}

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
   if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    stopMotors();
    Serial.println("Client disconnected");
  }
}

void setup()
{
  pinMode(camTrig, OUTPUT);
  digitalWrite(camTrig, HIGH);
  Serial.begin(115200);
  bluetooth.begin("pestRobo");

  bluetooth.register_callback(callback);


  // pinMode(34, INPUT);
  // pinMode(2, OUTPUT);
  // attachInterrupt(34, ISR, HIGH);

  
  setMotors();
  setGPS();
  setCompass();
  setGSM();
}

void loop()
{
  getInstruction();
  updateSerial();
  //  getGps();
  //  sendGpsData();
  //  getCompass(&compassHead);
  //  Serial.println(String(compassHead));
}
