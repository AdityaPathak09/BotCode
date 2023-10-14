#include <QMC5883LCompass.h>
#include <EEPROM.h>

#define EEPROM_SIZE 2

QMC5883LCompass compass;

int compassOffset = 0;

void setCompass()
{
  compass.init();
  EEPROM.begin(EEPROM_SIZE);
  compassOffset = EEPROM.read(0);
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

  *heading = 360 - (a - compassOffset);
    
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
  EEPROM.write(0, compassOffset);
  EEPROM.commit();
  Serial.println("New Offset: " +String(compassOffset));
}
