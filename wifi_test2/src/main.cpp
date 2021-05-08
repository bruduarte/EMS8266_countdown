

#include <Arduino.h>
#include <ArduinoJson.h>
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
#define DBFILE "schedule.txt"
#define STOPSFILE "stops.txt"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// const char* fileTest = "/schedule.txt";
bool flagTimeOk = false;
bool flagFetchTimezone = false;
int timeOffset = 0;
// credentials for connection.

const char* ssid       = "UPC7472663";
const char* password   = "a6Qmsbhnwrdk";

// object created to use UDP transmission in order to communicate with the NTP server.
WiFiUDP ntpUDP;

// variable that identifies the timezone in ms (Budapest is at +0, in ms = +1*60*60)

/*object to get the timestamp.*/
NTPClient timeClient(ntpUDP, 0);
/*object to local database*/
LocalDatabase database;
/*object to countdown*/
Countdown countdown;
/*struct to keep the stops data*/
stopsInfo* stops;


StaticJsonDocument<400> doc;
StaticJsonDocument<200> filter;
DeserializationError error;

bool fecthTimeZone()
{
  char url[100]; 

  const char* apiResponse; //to save the response from http fetch
  filter["dst"]= true;
  filter["raw_offset"] = true;

  strcpy(url, "http://worldtimeapi.org/api/timezone/");

  char* area = configManager.data.Area;

  strcat(url, area);

  char* city = configManager.data.City;
  strcat(url, "/");
  strcat(url, city);

  Serial.println(url);
  int httpCode = fetch.GET(url);
  
  if(httpCode != 200){
    Serial.println(httpCode);
    return false;
  }
  
  apiResponse = fetch.readString().c_str();
  Serial.println(apiResponse);
  

    // Deserialize the JSON document
  error = deserializeJson(doc, apiResponse, DeserializationOption::Filter(filter));

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return false;
  }

  // Fetch values.
   long rawOffset = doc["raw_offset"];
   bool dst = doc["dst"]; 
   
   timeOffset = rawOffset;

   if (dst){
     timeOffset = rawOffset + 3600; 
   } 

  Serial.println(rawOffset);
  return true;

}

bool setupTime()
{
  // int timeOffset = 3600; 
  
  //Updating the timeStamp. 

  timeClient.setTimeOffset(timeOffset);
  bool ret = timeClient.forceUpdate();

  if(!ret){
    Serial.println("Time update failed!");
    return false;
  }
 
  Serial.println("Time update... ok \n");
  Serial.println(timeClient.getFormattedTime());
  fetch.clean();
  return true;
}





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

  display.setTextSize(2);
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

  database.loadStopsInfo(STOPSFILE);
  database.loadTimetable(DBFILE);

  Serial.println("Loaded!");

  database.sortDatabase();
  stops = database.getLocalStopsInfo();
  //database.printDatabase();
  
}

void loop() {
  if(!flagFetchTimezone)
  {
    flagFetchTimezone = fecthTimeZone();
    if(!flagFetchTimezone) Serial.println("Could not fetch API data!");
    delay(500);
    return;
  }
  
  if(!flagTimeOk)
  {

    flagTimeOk = setupTime();
    if(!flagTimeOk) Serial.println("Could not setup time!");
    delay(500);
    return;
  }

  //Always updating and priting the timeStamp
  timeClient.update();
  

  //testing framework*********
  WiFiManager.loop();
  updater.loop();

  //Blinking LEDs for debugging
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  
  
  Serial.println();
  // countdown.displayCountdown(display, timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  // countdown.serialDisplayCountdown(timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  // countdown.serialDisplayPerStopCountdown(stops[0].stopID,6,20,10, database.getLocalDatabase(), database.getLocalStopsInfo());
  // countdown.displayCountdownPerStop(display, stops[0].stopID, 6,20,10, database.getLocalDatabase(), database.getLocalStopsInfo());

  for (int i = 0; i < MAXSTOPS; i++)
  {
    countdown.serialDisplayPerStopCountdown(stops[i].stopID,timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase(), database.getLocalStopsInfo());
    countdown.displayCountdownPerStop(display, stops[i].stopID, timeClient.getHours(),timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase(), database.getLocalStopsInfo());
    delay(5000);
  }
  
  Serial.println();
  
}


