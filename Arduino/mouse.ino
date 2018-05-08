
#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

LSM6DS3 myIMU(0x6A);


const char* ssid = "htl-IoT";
const char* password = "iot..2015";

WiFiUDP Udp;
unsigned int port = 8888;
char incomingPacket[255];

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMU
  myIMU.begin();


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

  float ay = myIMU.readFloatAccelY();
  float ax = myIMU.readFloatAccelX();
  float az = myIMU.readFloatAccelZ();
  float gx = myIMU.readFloatGyroX();
  float gy = myIMU.readFloatGyroY();
  float gz = myIMU.readFloatGyroZ();


//  sprintf(format,"a %.4f %.4f %.4f \ng %.4f %.4f %.4f ", ax, ay, az, gx, gy,);


  Udp.beginPacket("10.66.219.233", 8888);
  Udp.write("a " + new String(ax) + " " + new String(ay) + " " + new String(az) + "g " + new String(gx) + " " + new String(gy) + " " + new String(gz);
  Udp.endPacket();

  delay(100);
}