#include <SoftwareSerial.h>

#include <TinyGPS.h>

#define gpsRX 34 //16
#define gpsTX 4 //17


TinyGPS gps;
SoftwareSerial gpsSerial(gpsRX, gpsTX);

String getGps(float * latitude, float * longitude)
{
    unsigned long age;
    String string;
    while (gpsSerial.available())
    {
    // Serial.print(gpsSerial.read());
    string = string +  gps.encode(gpsSerial.read());
      // gps.encode(gpsSerial.read());
    }
    // Serial.println("GPS String " + string);
    gps.f_get_position(latitude, longitude, &age);

    if (*latitude != TinyGPS::GPS_INVALID_F_ANGLE && *longitude != TinyGPS::GPS_INVALID_F_ANGLE && age != TinyGPS::GPS_INVALID_AGE)
    {
        // string = String(latitude, 7) + "," + String(longitude, 7) + "," + String(age);
        string = String(*latitude, 7) + "," + String(*longitude, 7);
    }
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
