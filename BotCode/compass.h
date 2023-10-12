#include <QMC5883LCompass.h>

QMC5883LCompass compass;

float compassOffset = 0;

void setCompass()
{
  compass.init();
  Serial.println("Compass Set");
}

void getCompass(float *heading)
{

  compass.read();

  int a = compass.getAzimuth();

  // converting negetive values to positive (-180, 180) -> (0, 360)
  if (a < 0){
    a = 360 + a;  
  }

  *heading = a - compassOffset;
    
}

void setCompassOffset()
{
    // getCompass(&compassOffset);
    compass.read();

  int a = compass.getAzimuth();

  if (a < 0){
    a = 360 + a;  
  }

  compassOffset = a;
}
