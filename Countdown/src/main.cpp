#include <Arduino.h>
#include "LittleFS.h"

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED             2
#define OK              0
#define NOK             -1
#define SCREEN_WIDTH    128 // OLED display width, in pixels
#define SCREEN_HEIGHT   64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET      -1 // Reset pin # (or -1 if sharing Arduino reset pin)


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool flagWIFI = false;
bool flagLED = false;
const char* bkkReq = "https://futar.bkk.hu/api/query/v1/ws/otp/api/where/arrivals-and-departures-for-stop.json?key=apaiary-test&version=3&appVersion=apiary-1.0&includeReferences=true&stopId=BKK_F02461&onlyDepartures=true&limit=60&minutesBefore=2&minutesAfter=30";

int setupDisplay(){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    return NOK;
  }

  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  display.println ("Welcome!");
  display.display();

  return OK;
}

int setupFS(){
    //Initialize File System
    if(LittleFS.begin()){
        Serial.println("LittleFS init ok");
    }
    else{
        Serial.println("LittleFS init failed");
        return NOK;
    }    
    return OK;
}

int setupSerial(){
    Serial.begin(115200);
    return OK;
}

int setupIOTFramework(){
    GUI.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);
    timeSync.begin();

    return OK;
}

int setupLED(){
  //using LEDs just for debugging.
  pinMode(LED, OUTPUT);

  return OK;
}

void toggleLED(){
    digitalWrite(LED, flagLED);
    flagLED = !flagLED;
}

int fetchHttps(const char* url){
    Serial.printf("GET %s\n", url);
    int ret = fetch.GET(url);

    if(ret != HTTP_CODE_OK){
        Serial.printf("GET error: %d\n", ret);
        return NOK;
    }else{
        while (fetch.busy())
        {
            if (fetch.available())
            {
                Serial.write(fetch.read());
            }
        }

        fetch.clean();        
    }
    return OK;
}

void setup() {
    int ret = setupSerial();
    if(ret == NOK) while(1);

    Serial.println();
    Serial.println("Welcome!!");

    ret = setupFS();
    if(ret == NOK) while(1);

    // ret = setupDisplay();
    // if(ret == NOK) while(1);

    // ret = setupLED();
    // if(ret == NOK) while(1);

    ret = setupIOTFramework();
    if(ret == NOK) while(1);
}



void loop() {
    int ret = NOK;
    //software interrupts
    WiFiManager.loop();
    updater.loop();
    configManager.loop();
    
    const char* google = "https://www.google.com";
    ret = fetchHttps(google);
    if(ret != OK){
        Serial.printf("Fetch error");
    }

    delay(1000);
    // toggleLED();
}