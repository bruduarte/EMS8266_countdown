#include "LocalDatabase.h"
#include <FS.h>
#include <LittleFS.h>
#include <string.h>

LocalDatabase::LocalDatabase(){}

LocalDatabase::~LocalDatabase(){}


void LocalDatabase::loadTimetable(const char* filename){
    // int curCnt = 0;
    // timetableEntry entry;
    File f = LittleFS.open(filename, "r");

    if (!f) {
      Serial.println("File open failed on read.");
    } 
    else { 
        while(f.available()){
          
            String line = f.readStringUntil('\n');
            const char *linechar = line.c_str(); 
            char* teste = (char*)linechar;
            // char *part = strtok(teste, ",");

            // while (part != NULL) 
            // {

            // }
            
            
            
            


        }
    }   

}

void LocalDatabase::printDatabase(const char* filename){
    File f = LittleFS.open(filename, "r");

    if(!f){
        Serial.println("File open failed on read.");
    }
    else{
        Serial.println("Teste");
        for(int i=0;i<f.size();i++) //Read upto complete file size
        {
        Serial.print((char)f.read());
        }
      Serial.println();
      f.close(); 
        
    }

}