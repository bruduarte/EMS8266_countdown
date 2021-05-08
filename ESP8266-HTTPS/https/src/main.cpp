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

    delay(1000);

}