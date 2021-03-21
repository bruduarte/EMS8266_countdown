#include "LocalDatabase.h"
#include <FS.h>
#include <LittleFS.h>
#include <StringSplitter.h>

void LoadTimetable(const char* filename){

    File f = LittleFS.open(filename, "r");

    for (int i = 0; i < f.size(); i++){

        String entry = f.readStringUntil('\n');
        


    }

}