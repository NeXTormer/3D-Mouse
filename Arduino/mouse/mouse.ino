
#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

LSM6DS3 myIMU( I2C_MODE, 0x6A ); //Default constructor is I2C, addr 0x6B

const char* ssid = "htl-IoT";
const char* password = "iot..2015";

WiFiUDP Udp;
unsigned int port = 8888;
char incomingPacket[255];

float ax_offset = 0;
float ay_offset = 0;
float az_offset = 0;
float gx_offset = 0;
float gy_offset = 0;
float gz_offset = 0;


void setup() 
{
  myIMU.begin();
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("Processor came out of reset.\n");
  
  
  Serial.println("reading offset...");

  float ay = myIMU.readFloatAccelY();
  float ax = myIMU.readFloatAccelX();
  float az = myIMU.readFloatAccelZ();
  float gx = myIMU.readFloatGyroX();
  float gy = myIMU.readFloatGyroY();
  float gz = myIMU.readFloatGyroZ();

  float tries = 300.0f;

  for(float i = 0; i < tries; i++)
  {
      ay += myIMU.readFloatAccelY();
      ax += myIMU.readFloatAccelX();
      az += myIMU.readFloatAccelZ();
      gx += myIMU.readFloatGyroX();
      gy += myIMU.readFloatGyroY();
      gz += myIMU.readFloatGyroZ();
      delay(2);
  }

  
   ax_offset = ax/tries;
   ay_offset = ay/tries;
   az_offset = az/tries;
   gx_offset = gx/tries;
   gy_offset = gy/tries;
   gz_offset = gz/tries;

   Serial.println(ax_offset);
   Serial.println(ay_offset);
   Serial.println(az_offset);
   Serial.println(gx_offset);
   Serial.println(gy_offset);
   Serial.println(gz_offset);
   
  
  
  
  

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(8888);
  Serial.printf("UDP Server Started");


  

  
}


void loop()
{
  Serial.println(millis());
  

  float ax = myIMU.readFloatAccelX();
  float ay = myIMU.readFloatAccelY();
  float az = myIMU.readFloatAccelZ();
  float gx = myIMU.readFloatGyroX() - gx_offset;
  float gy = myIMU.readFloatGyroY() - gy_offset;
  float gz = myIMU.readFloatGyroZ() - gz_offset;


  Udp.beginPacket("10.66.219.233", 8888);
  String data = "t ";
  data += millis();
  data += " a \n";
  data += ax;
  data += "\n";
  data += ay;
  data += "\n";
  data += az;
  data += "\ng\n";
  data += gx;
  data += "\n";
  data += gy;
  data += "\n";
  data += gz;

  Serial.println(data);

  char data2[data.length()];
  data.toCharArray(data2, data.length());

  Udp.write(data2);
  Udp.endPacket();
  
  delay(100);
}


void printserial(LSM6DS3 myIMU)
{
   Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatAccelZ(), 4);

  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readFloatGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readFloatGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readFloatGyroZ(), 4);

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees C = ");
  Serial.println(myIMU.readTempC(), 4);
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 4);
}

