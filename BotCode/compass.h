#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

float compassOffset = 0.0;

Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(108);

void setCompass()
{
    compass.begin();
}

void getCompass(float *heading)
{
    sensors_event_t event;
    compass.getEvent(&event);

    *heading = atan2(event.magnetic.y, event.magnetic.x);
    *heading = *heading - compassOffset;

    if (*heading < 0)
        *heading += 2 * PI;

    if (*heading > 2 * PI)
        *heading -= 2 * PI;

    float headingDegrees = *heading * 180 / M_PI;

    *heading = headingDegrees;
}

void setCompassOffset()
{
    getCompass(&compassOffset);
}