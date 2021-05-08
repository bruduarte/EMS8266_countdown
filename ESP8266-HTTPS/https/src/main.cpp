#include <Arduino.h>
#include "LittleFS.h"

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"

#define LED 2

void setup() {
    Serial.begin(115200);

    LittleFS.begin();
    GUI.begin();
    configManager.begin();
    WiFiManager.begin(configManager.data.projectName);
    timeSync.begin();

    Serial.println("Hello world");
}

void loop() {
    //software interrupts
    WiFiManager.loop();
    updater.loop();
    configManager.loop();

    //your code here
    int ret = fetch.GET("https://www.google.com");

    if(ret != HTTP_CODE_OK){
        Serial.printf("GET error: %d\n", ret);
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

    const char* bkkReq = "https://futar.bkk.hu/api/query/v1/ws/otp/api/where/arrivals-and-departures-for-stop.json?key=apaiary-test&version=3&appVersion=apiary-1.0&includeReferences=true&stopId=BKK_F02461&onlyDepartures=true&limit=60&minutesBefore=2&minutesAfter=30";
    ret = fetch.GET(bkkReq);

    if(ret != HTTP_CODE_OK){
        Serial.printf("GET error: %d\n", ret);
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

    delay(1000);

}