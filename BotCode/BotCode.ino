// Bluetooth Library ///////////////////////////////////////////////////////////////////
#include <BluetoothSerial.h>
BluetoothSerial bluetooth;

// Software Serial ////////////////////////////////////////////////////////////////
// for communication between camera and controller
#include <SoftwareSerial.h>

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


int ip_address_of_flask_server[4];
// pinMap ////////////////////////////////////////////////////////////////////////
#define camTrig 19

#define RX_PIN 12  //not used
#define TX_PIN 19  // used for communicating with camera (for flask server IP)

/*
  camera trigger and software serial tx (TX_PIN) shares same pin. 
  First the IP will  be sent to camera using Software Serial, 
  then the software serial port will be closed, and the pin will be used as camera trigger 

*/

// objects Initiation ///////////////////////////////////////////////////////////

SoftwareSerial mySerial(RX_PIN, TX_PIN);

// Main Coding /////////////////////////////////////////////////////////////////

// this function handles gps task like retriving lat, lon and compass heading data
// and sending it back to mobile phone
void sendGpsData() {

  getGps(&latitude, &longitude); // getting GPS Data

  getCompass(&compassHead);      // getting Compass

  // Serial.println("Latitude: " + String(latitude) + " Longitude: " + String(longitude)+ " Comass: " + String(compassHead));


  // converting Data to non-decimal form 

  long lat = long(latitude * 10000000.0);  
  long lon = long(longitude * 10000000.0);
  long compH = long(compassHead * 100.0);


  // Braking data to set of 2 digits, then to byte.
  // Since bluetooth can only send byte data 

  for (int i = 0; i < 5; i++) {
    latitudeArr[i] = byte(lat % 100);
    longitudeArr[i] = byte(lon % 100);

    lat = lat / 100;
    lon = lon / 100;
  }

  for (int i = 0; i < 3; i++) {
    compassHeadArr[i] = byte(compH % 100);
    compH = compH / 100;
  }


  // Writing data to bluetooth port

  for (int i = 4; i >= 0; i--) {
    bluetooth.write(latitudeArr[i]);
  }

  delay(100);

  for (int i = 4; i >= 0; i--) {
    bluetooth.write(longitudeArr[i]);
  }

  delay(100);
  for (int i = 2; i >= 0; i--) {
    bluetooth.write(compassHeadArr[i]);
  }

}

// this function reads instructions from bluetooth port sent by operater and acts upon them
byte getInstruction() {

  byte instruction = 0; // instruction
  int charector = 0;    // additional property with instruction

  while (bluetooth.available()) {
    instruction = bluetooth.read();
    charector = bluetooth.read();

    Serial.println("Instruction: " + String(instruction) + "Speed: " + String(charector));

    switch (instruction) {
      case 1:                       // Forward at perticular speed
        forward(charector);
        break;

      case 2:                       // Reverse at perticular speed
        backard(charector);
        break;

      case 3:                       // slight left at perticular speed
        leftTurn(charector);
        break;

      case 4:                       // slight Rght at perticular speed
        rightTurn(charector);
        break;

      case 5:                       // rotate towared left at perticular speed
        leftSpin(charector);
        break;

      case 6:                       // rotate towards right at perticular speed
        rightSpin(charector);
        break;

      case 7:                       // Stop vehicle
        stopMotors();
        break;

      case 8:                       // Move Camera at perticular angle
        setCam(charector);
        break;

      case 9:                       // Send GPS Data
        sendGpsData();
        break;

      case 10:                      // Turn On Off Pump1
        if (charector == 1)
          setPump(pump1, LOW);
        else
          setPump(pump1, HIGH);
        break;

      case 11:                      // Turn On Off Pump2
        if (charector == 1)
          setPump(pump2, LOW);
        else
          setPump(pump2, HIGH);
        break;

      case 12:                      // Set Sprayer Height/Angle (center)
        setSprayerCentreHeight(charector);
        break;

      case 13:                      // Set Sprayer Height/Angle (Edge)
        setSprayerEdgeHeight(charector);
        break;

      case 14:                      // Trigger Camera
        digitalWrite(camTrig, LOW);
        delay(1200);
        digitalWrite(camTrig, HIGH);
        break;

      case 15:                      // set IP Header 0
        ip_address_of_flask_server[0] = charector;
        break;

      case 16:                      // set IP Header 1
        ip_address_of_flask_server[1] = charector;
        break;

      case 17:                      // set IP Header 2
        ip_address_of_flask_server[2] = charector;
        break;

      case 18:                      // set IP Header 3, then send IP to camera using software serial
        ip_address_of_flask_server[3] = charector;

        mySerial.begin(9600);

        mySerial.println("Dummy");  // Send the integer value via SoftwareSerial
        delay(100);
        mySerial.println(ip_address_of_flask_server[0] + 255);  // Send the integer value via SoftwareSerial
        delay(10);
        mySerial.println(ip_address_of_flask_server[1] + 255);  // Send the integer value via SoftwareSerial
        delay(10);
        mySerial.println(ip_address_of_flask_server[2] + 255);  // Send the integer value via SoftwareSerial
        delay(10);
        mySerial.println(ip_address_of_flask_server[3] + 255);  // Send the integer value via SoftwareSerial

        mySerial.end();

        pinMode(camTrig, OUTPUT);

        digitalWrite(camTrig, HIGH);

        break;

      default:                       // for any other instruction, stop all motors
        instruction = 0;
        stopMotors();
        break;
    }
  }

  return instruction;
}

// This is bluetooth callback function. used to stop motors when device gets disconnected
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
  }

  if (event == ESP_SPP_CLOSE_EVT) {
    stopMotors();
    Serial.println("Client disconnected");
  }
}

// Setup function  runs one
void setup() {
  
  //Setting pinmodes 
  pinMode(camTrig, OUTPUT);
  digitalWrite(camTrig, HIGH);
  
  // Serial monitor setup
  Serial.begin(115200);

  // bluetooth setup 
  bluetooth.begin("pestRobo");
  bluetooth.register_callback(callback);

  // setting  peripherals (Sensors, Drivers)
  setMotors();
  setGPS();
  setCompass();
  setGSM();
}

// Loop function runs continously
void loop() {
  getInstruction();
  updateSerial();

  // sendGpsData(); //For sending GPS Signals
}
