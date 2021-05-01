

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "LittleFS.h"
#include "LocalDatabase.h"
#include "Countdown.h"

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"


#define LED 2
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* filename = "/samplefile.txt";
const char* fileTest = "/database.txt";


// credentials for connection.

const char* ssid       = "UPC7472663";
const char* password   = "a6Qmsbhnwrdk";

// object created to use UDP transmission in order to communicate with the NTP server.
WiFiUDP ntpUDP;

// variable that identifies the timezone in ms (Budapest is at +0, in ms = +1*60*60)
int timeOffset = 3600; //******* see how to get it from web*******

/*object to get the timestamp.*/
NTPClient timeClient(ntpUDP, timeOffset);
/*object to local database*/
LocalDatabase database;
/*object to countdown*/
Countdown countdown;


void setup()
{
  //Initialize Serial Port with baud rate 9600.
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println ("Welcome!");
  display.display(); 

    //Initialize File System
  if(LittleFS.begin())
  {
    Serial.println("LittleFS Initialize....ok");
  }
  else
  {
    Serial.println("LittleFS Initialization...failed");
      while(1);
      // Serial.println("formating FS");
      // if(LittleFS.format()){
      //   Serial.println("formated!!");
      // }else{
      //   Serial.println("formated FAILED");
      // }
  }
  //framework********************************************
  GUI.begin();
  configManager.begin();
  WiFiManager.begin(configManager.data.projectName);
  // timeSync.begin();

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

  database.loadTimetable(fileTest);

  Serial.println("Loaded!");

  database.sortDatabase();

  //database.printDatabase();

  
  


  
}

void loop() {
  
  //Always updating and priting the timeStamp
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  //testing framework*********
  WiFiManager.loop();
  updater.loop();

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

  Serial.println("Display countdoun!: ");
  Serial.println();
  countdown.displayCountdown(display, timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  countdown.serialDisplayCountdown(timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  Serial.println("CORRE!");
}


