#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DEBUG

#ifdef DEBUG
#define PRINT(x); Serial.println(x);
#else
#define PRINT(x); ;
#endif

#if 0
const char* ssid = "htl-IoT";
const char* password = "iot..2015";
const char* ip = "10.66.219.233";
#elif 0
const char* ssid = "OnePlus 6";
const char* password = "petapeta";
const char* ip = "192.168.43.35";
#else
const char* ssid = "Network";
const char* password = "voneverec";
const char* ip = "10.0.0.69";
#endif

const int RGB_GND = 15;
const int RGB_RED = 13;
const int RGB_GREEN = 14;
const int RGB_BLUE = 12;
const int BUTTON = 15;

const float OFFSET_MEASUREMENTS = 100.0f;
const int OFFSET_DELAY = 10;

unsigned const int port = 8888;

/* === */

LSM6DS3 myIMU( I2C_MODE, 0x6A );
WiFiUDP udp;

char packetBuffer[3];
bool button_state = false;

float ax_offset = 0;
float ay_offset = 0;
float az_offset = 0;
float gx_offset = 0;
float gy_offset = 0;
float gz_offset = 0;

/* === */

void printserial();
void RGB(unsigned int value);
bool readButton();
void buttonInterrupt();

/* === */

void setup()
{
  RGB(0xFF0000);
  PRINT("Starting 3D-Mouse...\n");


  //Interrupt
  attachInterrupt(BUTTON, buttonInterrupt, RISING);

  //Sensor / Serial setup
  myIMU.begin();

  Serial.begin(115200);
  delay(1000);

  //PWM
  analogWriteRange(255);

  //Pins

  //LED_BUILTIN
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);

  //RGB_LED
  pinMode(RGB_GND, OUTPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  
  digitalWrite(RGB_GND, LOW);

  //BUTTON
  pinMode(BUTTON, INPUT);
  

  //Calculate offset
  
  RGB(0xFF8C00);

  PRINT("Reading offset...");

  float ax = 0.0f;
  float ay = 0.0f;
  float az = 0.0f;
  float gx = 0.0f;
  float gy = 0.0f;
  float gz = 0.0f;

  for (float i = 0; i < OFFSET_MEASUREMENTS; i++)
  {
    ay += myIMU.readFloatAccelY();
    ax += myIMU.readFloatAccelX();
    az += myIMU.readFloatAccelZ();
    gx += myIMU.readFloatGyroX();
    gy += myIMU.readFloatGyroY();
    gz += myIMU.readFloatGyroZ();
    delay(OFFSET_DELAY);
  }

  ax_offset = ax/OFFSET_MEASUREMENTS;
  ay_offset = ay/OFFSET_MEASUREMENTS;
  az_offset = az/OFFSET_MEASUREMENTS;
  gx_offset = gx/OFFSET_MEASUREMENTS;
  gy_offset = gy/OFFSET_MEASUREMENTS;
  gz_offset = gz/OFFSET_MEASUREMENTS;

  PRINT("Offset (ax, ay, az, gx, gy, gz):")

  PRINT(ax_offset);
  PRINT(ay_offset);
  PRINT(az_offset);
  PRINT(gx_offset);
  PRINT(gy_offset);
  PRINT(gz_offset);

  RGB(0x3AE4FF);   

  PRINT("Connecting to");
  PRINT(ssid)
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    PRINT(".");
  }
  PRINT("connected");
  PRINT(WiFi.localIP());

  udp.begin(8888);
  PRINT("UDP Server Started"); 

  //Send pilot packet
  udp.beginPacket(ip, port);
  udp.write(0x42);
  udp.endPacket();


  RGB(0x00FF00);
}


void loop()
{
  int packet = udp.parsePacket();
  if(packet)
  {
    PRINT("Received packet");

    int size = udp.read(packetBuffer, 3);

    int color = (packetBuffer[0] << 16) | (packetBuffer[1] << 8) | packetBuffer[2];
    
    PRINT(packetBuffer[0]);
    PRINT(packetBuffer[1]);
    PRINT(packetBuffer[2]);
    PRINT(color);
    RGB(color);
  }


  //PRINT(millis());
  float ax = myIMU.readFloatAccelX();
  float ay = myIMU.readFloatAccelY();
  float az = myIMU.readFloatAccelZ();
  float gx = myIMU.readFloatGyroX() - gx_offset;
  float gy = myIMU.readFloatGyroY() - gy_offset;
  float gz = myIMU.readFloatGyroZ() - gz_offset;
  //bool button = readButton();
  bool button = digitalRead(BUTTON);
  udp.beginPacket(ip, port);
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
  data += "\nbtn\n";
  data += button ? "1" : "0";
  data += "\n";
  data += "";
  

  char data2[data.length()];
  data.toCharArray(data2, data.length());

  PRINT(data2);

  udp.write(data2);
  udp.endPacket();


  delay(10);
}


void printserial(LSM6DS3 myIMU)
{
#ifdef DEBUG
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
#endif
}

void RGB(unsigned int color)
{
  unsigned int r = color & 0xFF0000;
  r = r >> 16;

  unsigned int g = color & 0xFF00;
  g = g >> 8;

  unsigned int b = color & 0xFF;
  
  analogWrite(RGB_RED, r);
  analogWrite(RGB_GREEN, g);
  analogWrite(RGB_BLUE, b);
}

bool readButton()
{
  if(button_state)
  {
    button_state = false;
    return true;
  }
  return false;
}

void buttonInterrupt()
{
  button_state = true; 
}