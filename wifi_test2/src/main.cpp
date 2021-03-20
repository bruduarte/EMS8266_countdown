#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LED 2

const char* ssid       = "UPC7472663";
const char* password   = "a6Qmsbhnwrdk";

WiFiUDP ntpUDP;
int timeOffset = 3600; //+1*60*60

NTPClient timeClient(ntpUDP, timeOffset);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  pinMode(LED, OUTPUT);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();
  Serial.println();

  Serial.println("trying to get current time from NTP server.");

  bool ret = timeClient.forceUpdate();
  if(ret){
    Serial.println("Funfou");
  }
  else {
    Serial.println("Nao funfou");
  }

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(timeClient.getFormattedTime());
}

void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");

  Serial.println(timeClient.getFormattedTime());

  delay(1000);
}
