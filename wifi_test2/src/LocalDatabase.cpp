#include "LocalDatabase.h"



#define DELIMITER ","

LocalDatabase::LocalDatabase(){}

LocalDatabase::~LocalDatabase(){}


void LocalDatabase::loadTimetable(const char* filename){
    // int curCnt = 0;
    // timetableEntry entry;
    File f = LittleFS.open(filename, "r");
    int entryNumber = 0; //tracks the entries in the database
    
    if (!f) {
      Serial.println("File open failed on read.");
    } 
    else { 
        while(f.available() && entryNumber<DBSIZE){
            
            this->line = f.readStringUntil('\n');
            Serial.println(this->line);
            Serial.println(entryNumber);
            if (this->line == ""){
                Serial.println("****EMPTY STRING!!!****");
                break;
            }

            char linechar[20];

            strcpy(linechar,line.c_str()); //converting the String to char[]

            char *field = strtok(linechar, DELIMITER);
            
            
            
            //Filling first field
            int fieldSize = strlen(field); // gets the size of the field
            if (fieldSize > 4){ //limits the first field size
                fieldSize = 4; 
            }
            strncpy(timetable[entryNumber].lineID, field, fieldSize); // copies the first field
            Serial.println(timetable[entryNumber].lineID); // ****************************TESTE

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling second field
            fieldSize = strlen(field);
            if (fieldSize > 2){ //limits the first field size
                fieldSize = 2; 
            }
            strncpy(timetable[entryNumber].hour, field, fieldSize);
            Serial.println(timetable[entryNumber].hour);// ****************************TESTE

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling third field
            fieldSize = strlen(field);
            if (fieldSize > 2){ //limits the first field size
                fieldSize = 2; 
            }
            strncpy(timetable[entryNumber].min, field, fieldSize);

            Serial.println(timetable[entryNumber].min); // ****************************TESTE

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling fourth field
            fieldSize = strlen(field);
            if (fieldSize > 10){ //limits the first field size
                fieldSize = 10; 
            }
            strncpy(timetable[entryNumber].stopName, field, fieldSize);
            Serial.println(timetable[entryNumber].stopName); // ****************************TESTE

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling sixth field
            fieldSize = strlen(field);
            if (fieldSize > 2){ //limits the first field size
                fieldSize = 2; 
            }
            strncpy(timetable[entryNumber].walkTime, field, fieldSize);
            Serial.println(timetable[entryNumber].walkTime); // ****************************TESTE

            
            entryNumber++;
            
            

        }
        Serial.println("Closing file...");
        f.close();
        Serial.println("File closed!");


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