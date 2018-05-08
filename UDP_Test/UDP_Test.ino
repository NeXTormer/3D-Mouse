#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

WiFiUDP Client;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  WiFi.begin("htl-IoT", "iot..2015");
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Client.begin(8888);
}

void loop() {
  Client.beginPacket("10.66.209.233", 8888);
  char replyPacket[] = "Peter Rendl";
  Client.write(replyPacket);
  Client.endPacket();

  delay(3000);
}
