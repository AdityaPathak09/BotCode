// for GPS to work better, use use Higher Power Gain ceramic anteena.
// Currently we are using simple pcb antenna ro get the job done. Also, Raise the Anteena over the rover body.
// Same can be applied for GSM Module

#include <SoftwareSerial.h>

#include <TinyGPS.h>

#define gpsRX 34 // Sensor's TX, microcontroller's RX
#define gpsTX 4  // Sensor's RX, microcontroller's TX

TinyGPS gps;
SoftwareSerial gpsSerial(gpsRX, gpsTX);

String getGps(float *latitude, float *longitude)
{
  unsigned long age;
  String string;
  while (gpsSerial.available())
    string = string + gps.encode(gpsSerial.read());

  gps.f_get_position(latitude, longitude, &age);

  if (*latitude != TinyGPS::GPS_INVALID_F_ANGLE && *longitude != TinyGPS::GPS_INVALID_F_ANGLE && age != TinyGPS::GPS_INVALID_AGE)
    string = String(*latitude, 7) + "," + String(*longitude, 7);

  else
  {
    string = "Invalid Data";
    *latitude = 0;
    *longitude = 0;
  }

  return string;
}

void setGPS()
{
  gpsSerial.begin(9600);
  Serial.println("GPS Set");
}
