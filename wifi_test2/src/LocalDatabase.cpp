#include "LocalDatabase.h"
#include <stdlib.h> 


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
            
            #ifdef MY_DEBUG
            Serial.println(timetable[entryNumber].lineID); // ****************************TESTE
            #endif

            field = strtok(NULL, DELIMITER); //strtok saves its state!
            
            //Filling second field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2;
                timetable[entryNumber].hour = (field[0] - '0')*10 + (field[1]-'0'); 
            }else{
                timetable[entryNumber].hour = (field[0]- '0');
            }
            

            #ifdef MY_DEBUG
            Serial.println(timetable[entryNumber].hour);// ****************************TESTE
            #endif
            
            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling third field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2; 
                timetable[entryNumber].min = (field[0] - '0')*10 + (field[1]-'0');
            }else{
                timetable[entryNumber].min = (field[0]- '0');
            }
            
            #ifdef MY_DEBUG
            Serial.println(timetable[entryNumber].min); // ****************************TESTE
            #endif

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling fourth field
            fieldSize = strlen(field);
            if (fieldSize > 10){ //limits the first field size
                fieldSize = 10; 
            }
            strncpy(timetable[entryNumber].stopName, field, fieldSize);

            #ifdef MY_DEBUG
            Serial.println(timetable[entryNumber].stopName); // ****************************TESTE
            #endif

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling sixth field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2;
                if (field[1] == 13){
                    timetable[entryNumber].walkTime = (field[0]- '0');
                } else
                timetable[entryNumber].walkTime = (field[0] - '0')*10 + (field[1]-'0');
            }
        

            #ifdef MY_DEBUG
            Serial.println(timetable[entryNumber].walkTime); // ****************************TESTE
            #endif

            
            entryNumber++;
            
            

        }
        Serial.println("Closing file...");
        f.close();
        Serial.println("File closed!");


    }   

}


void LocalDatabase::printDBFile(const char* filename){
    File f = LittleFS.open(filename, "r");

    if(!f){
        Serial.println("File open failed on read.");
    }
    else{
        Serial.println("Printing contents of the database:");
        for(int i=0;i<f.size();i++) //Read upto complete file size
        {
        Serial.print((char)f.read());
        }
      Serial.println();
      f.close(); 
        
    }

}

void LocalDatabase::printDatabase(){
    for(int i = 0; i < DBSIZE; i++){
        
        Serial.printf("%s, %i, %i, %s, %i \n",timetable[i].lineID, timetable[i].hour,timetable[i].min,timetable[i].stopName,timetable[i].walkTime);

    }

}


int LocalDatabase::compareElements(const void *a, const void *b){
    /*casting to use the corresponding type of the database*/
    timetableEntry* um = (timetableEntry*)a;
    timetableEntry* dois = (timetableEntry*)b;
    
  
    if (((um->hour * 60 + um->min) - (dois->hour * 60 + dois->min)) < 0){ //'um' is smaller than 'dois' means transport 'um' passes earlier than 'dois' in the timetable.
        return -1;
    } else if (((um->hour * 60 + um->min ) - (dois->hour * 60 + dois->min)) > 0) { //'um' is bigger than 'dois' means transport 'um' passes later than 'dois' in the timetable.
       return 1;
    } else{
        return 0; //case both have same time.
    }
    

}

void LocalDatabase::sortDatabase(){

    qsort(timetable, DBSIZE, sizeof(timetableEntry),LocalDatabase::compareElements);

    

}