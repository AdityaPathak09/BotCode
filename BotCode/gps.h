// for GPS to work better, use use Higher Power Gain ceramic anteena.
// Currently we are using simple pcb antenna ro get the job done. Also, Raise the Anteena over the rover body.
// Same can be applied for GSM Module

#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define gpsRX 34  // Sensor's TX, microcontroller's RX
#define gpsTX 4   // Sensor's RX, microcontroller's TX

TinyGPSPlus gps;
SoftwareSerial gpsSerial(gpsRX, gpsTX);

String getGps(double *latitude, double *longitude) {
  unsigned long age;
  String string;
  while (gpsSerial.available())
    string = string + gps.encode(gpsSerial.read());

  if (gps.location.isValid()) {
    *latitude = gps.location.lat() * 10000000.0;
    *longitude = gps.location.lng() * 10000000.0;
    // Serial.print(*latitude, 6);
    // Serial.println(*longitude, 6);
  } else {
    string = "Invalid Data";
    *latitude = 0;
    *longitude = 0;
  }

  return string;
}

void setGPS(double *latitude, double *longitude) {
  gpsSerial.begin(9600);

  long fixstarttime = millis();

  while (*latitude == 0 || *longitude == 0) {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
    if (gps.location.isValid()) {
      *latitude = gps.location.lat() * 10000000.0;
      *longitude = gps.location.lng() * 10000000.0;
      // Serial.print(*latitude, 6);
      // Serial.println(*longitude, 6);
    }
    Serial.println("Finding Fix");
    if (millis() - fixstarttime >= 7000)
      break;
  }

  Serial.println("GPS Set");
}
