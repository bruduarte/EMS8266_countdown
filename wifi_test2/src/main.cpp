

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
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"
#include <time.h>



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DBFILE "schedule.txt"
#define STOPSFILE "stops.txt"
#define HOLIDAYSFILE "holidays.txt"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)


bool flagTimeOk = false;
bool flagFetchTimezone = false;
bool flagStopsLoaded = false;
bool flagHolidaysLoaded = false;
bool flagScheduleLoaded = false;
int timeOffset = 0;
int isHoliday = 0; 


// credentials for connection.
const char* ssid       = "UPC7472663";
const char* password   = "a6Qmsbhnwrdk";

/*object created to use UDP transmission in order to communicate with the NTP server.*/
WiFiUDP ntpUDP;

/*object to get the timestamp.*/
NTPClient timeClient(ntpUDP, 0);
/*object to local database*/
LocalDatabase database;
/*object to countdown*/
Countdown countdown;
/*struct to keep the stops data*/
stopsInfo* stops;
/*variable to get unix time*/
time_t unixTime;
struct tm ts;

/*Display object*/
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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


void saveCallback() {
    Serial.println("EEPROM saved");
    flagFetchTimezone = false;
    flagTimeOk = false;
}


void setup()
{
  //Initialize Serial Port with baud rate 9600.
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    while(1); // Don't proceed, loop forever
  }

  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println ("Welcome!");
  display.display(); 

  //Init File System
  if(LittleFS.begin())
  {
    Serial.println("LittleFS init ok!");
  }
  else
  {
    Serial.println("LittleFS init failed!");
      while(1);      
  }
  //Init framework
  GUI.begin();
  configManager.begin();
  configManager.setConfigSaveCallback(saveCallback);
  WiFiManager.begin(configManager.data.projectName);
  // timeSync.begin();
 
  //Init WiFi connection.
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
}

void printWaitingTime(){
  display.clearDisplay();
	display.setCursor(0,0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.printf("Wait while we update the time to\n");
  display.setTextSize(2);
  display.printf("\n%s", configManager.data.City);
	display.display();
}

void printConfigRequest(const char* str){
  display.clearDisplay();
	display.setCursor(0,0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.printf("Please setup the\n");
  display.setTextSize(2);
  display.printf("\n%s", str);
	display.display();
}

void loop() {
  delay(500);
  if(!flagFetchTimezone)
  {
    printWaitingTime();
    flagFetchTimezone = fecthTimeZone();
    if(!flagFetchTimezone) Serial.println("Could not fetch API data!");
    return;
  }
  
  if(!flagTimeOk)
  {
    flagTimeOk = setupTime();
    if(!flagTimeOk) Serial.println("Could not setup time!");
    return;
  }
  if(!flagStopsLoaded)
  {
    flagStopsLoaded = database.loadStopsInfo(STOPSFILE);
    if(!flagStopsLoaded)
    {
      debug_print("Could not load %s file\n", STOPSFILE);
      printConfigRequest(STOPSFILE);
      return;
    }
  }
  if(!flagScheduleLoaded)
  {
    flagScheduleLoaded = database.loadTimetable(DBFILE);
    if(!flagScheduleLoaded)
    {
      debug_print("Could not load %s file\n", DBFILE);
      printConfigRequest(DBFILE);
      return;
    }
    database.sortDatabase();
  }
  if(!flagHolidaysLoaded)
  {
    flagHolidaysLoaded = database.loadHolidays(HOLIDAYSFILE);
    if(!flagHolidaysLoaded)
    {
      debug_print("Could not load %s file\n", HOLIDAYSFILE);
      printConfigRequest(HOLIDAYSFILE);
      return;
    }
    stops = database.getLocalStopsInfo();
    Serial.println("Database files loaded!");
  }

  //Always updating and priting the timeStamp
  timeClient.update();
  
  unixTime = timeClient.getEpochTime();
  ts = *localtime(&unixTime);
  
  isHoliday = database.isItHoliday(ts.tm_mday,ts.tm_mon+1);

  //Framework loops
  WiFiManager.loop();
  configManager.loop();  
  
  Serial.println();
  // countdown.displayCountdown(display, timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  // countdown.serialDisplayCountdown(timeClient.getFormattedTime(), timeClient.getHours(), timeClient.getMinutes(),timeClient.getSeconds(), database.getLocalDatabase());
  // countdown.serialDisplayPerStopCountdown(stops[0].stopID,6,20,10, database.getLocalDatabase(), database.getLocalStopsInfo());
  // countdown.displayCountdownPerStop(display, stops[0].stopID, 6,20,10, database.getLocalDatabase(), database.getLocalStopsInfo());

  for (int i = 0; i < MAXSTOPS; i++)
  {
    Serial.printf("%d - %s\t%s\n", i, timeClient.getFormattedTime().c_str(), WiFi.localIP().toString().c_str());
    countdown.serialDisplayPerStopCountdown(stops[i].stopID,6,20,10, isHoliday,ts.tm_wday, database.getLocalDatabase(), database.getLocalStopsInfo());
    countdown.displayCountdownPerStop(display, stops[i].stopID, 6,20,10, isHoliday,ts.tm_wday, database.getLocalDatabase(), database.getLocalStopsInfo());
    delay(3000);
  }

  if(!flagFetchTimezone && !flagTimeOk)
  {
    printWaitingTime();
  }
  
  Serial.println();  
}


