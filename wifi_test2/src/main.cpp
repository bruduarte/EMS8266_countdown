#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED 2

void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(LED, OUTPUT);
  WiFi.begin("UPC7472663", "a6Qmsbhnwrdk");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");
  delay(1000);
}