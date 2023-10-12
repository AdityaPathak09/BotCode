#include <SoftwareSerial.h>


#define gsmRX 16
#define gsmTX 17

SoftwareSerial gsmSerial(gsmRX, gsmTX);

// just prints whatever available on software serial to serial monitor and vise-versa
void updateSerial() 
{
    
  while (Serial.available()) 
  {
    gsmSerial.write(Serial.read());
    //Forward what Serial received to Software Serial Port
  }
  while(gsmSerial.available()) 
  {
    Serial.write(gsmSerial.read());
    //Forward what Software Serial received to Serial Port
  }
  delay(100);
}

// String SIM800_send(String incoming) //Function to communicate with SIM800 module
// {
//     gsmSerial.println(incoming); delay(100); //Print what is being sent to GSM module
//     String result = "";
//     while (gsmSerial.available()) //Wait for result
//     {
//     char letter = gsmSerial.read();
//     result = result + String(letter); //combine char to string to get result
//     }
// return result; //return the result
// }

void setGSM()
{
    gsmSerial.begin(9600);
    Serial.println("Initializing...");
    // delay(5000);

      gsmSerial.println("AT"); //Once the handshake test is successful, it will back to OK
      updateSerial();
      
      gsmSerial.println("AT+CBC"); //Check whether it has registered in the network
      updateSerial();

      gsmSerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      gsmSerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
      updateSerial();
      gsmSerial.println("AT+CREG?"); //Check whether it has registered in the network
      updateSerial();

      // gsmSerial.println("AT+CGATT=1"); //Signal quality test, value range is 0-31 , 31 is the best
      // updateSerial();
      // gsmSerial.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); //Read SIM information to confirm whether the SIM is plugged
      // updateSerial();
      // gsmSerial.println("AT+SAPBR=3,1,\"APN\",\"RCMNET\" "); //Check whether it has registered in the network
      // updateSerial();
      // gsmSerial.println("AT+SAPBR=1,1"); //Signal quality test, value range is 0-31 , 31 is the best
      // updateSerial();
      // gsmSerial.println("AT+SAPBR=2,1"); //Read SIM information to confirm whether the SIM is plugged
      // updateSerial();
      // gsmSerial.println("AT+CLBS=1,1"); //Signal quality test, value range is 0-31 , 31 is the best
      // updateSerial();
 
      gsmSerial.println("AT+CMGF=1"); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      gsmSerial.print("AT+CMGS=\"+917507927015\""); //Read SIM information to confirm whether the SIM is plugged
      updateSerial();
      gsmSerial.print("PestBot Turned On."); //Check whether it has registered in the network
      updateSerial();
      gsmSerial.println((char)26); //Signal quality test, value range is 0-31 , 31 is the best
      updateSerial();
      
      gsmSerial.println("AT+DDET=1"); //Check whether it has registered in the network
      updateSerial();
      
      gsmSerial.println("ATS0=1"); //Check whether it has registered in the network
      updateSerial();

      
      Serial.println("GSM Set");
}

