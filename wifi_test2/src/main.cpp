#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "LittleFS.h"


#define LED 2

const char* filename = "/samplefile.txt";

// credentials for connection.

const char* ssid       = "UPC7472663";
const char* password   = "a6Qmsbhnwrdk";

// object created to use UDP transmission in order to communicate with the NTP server.
WiFiUDP ntpUDP;

// variable that identifies the timezone in ms (Budapest is at +1, in ms = +1*60*60)
int timeOffset = 3600; 

// object to get the timestamp.  
NTPClient timeClient(ntpUDP, timeOffset);

void setup()
{
    //Initialize File System
  if(LittleFS.begin())
  {
    Serial.println("LittleFS Initialize....ok");
  }
  else
  {
    Serial.println("LittleFS Initialization...failed");
  }
  
  //Initialize Serial Port with baud rate 9600.
  Serial.begin(9600);
  Serial.println();

  //using LEDs just for debugging.
  pinMode(LED, OUTPUT);

  //Initialize WiFi connection.
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  //While the connection is not succesful print "..."
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  //If the connection was succesful, print the IP.
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //Initialize the NTP client.
  timeClient.begin();
  Serial.println();

  Serial.println("Trying to get current time from NTP server.");

  //Updating the timeStamp. 
  bool ret = timeClient.forceUpdate();

  //Verifying if the update worked.
  if(ret){
    Serial.println("Update... ok \n");
    Serial.println(timeClient.getFormattedTime());
  }
  else {
    Serial.println("Update... failed \n");
  }

  
  
  //Create New File And Write Data to It
  //w=Write Open file for writing
  File f = LittleFS.open(filename, "w");
  
  if (!f) {
    Serial.println("file open failed");
  }
  else
  {
      //Write data to file
      Serial.println("Writing Data to File");
      f.print("This is sample data which is written in file");
      f.close();  //Close file
  }




}

void loop() {
  //Always updating and priting the timeStamp
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  //Blinking LEDs for debugging
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("LED is off");

  //Trying to read file data
  File f = LittleFS.open(filename, "r");
  
  if (!f) {
    Serial.println("File open failed");
  }
  else
  {
      Serial.println("Reading Data from File:");
      //Data from file
      for(int i=0;i<f.size();i++) //Read upto complete file size
      {
        Serial.print((char)f.read());
      }
      Serial.println();
      f.close();  //Close file
      Serial.println("File Closed");
  }

  delay(1000);
}
