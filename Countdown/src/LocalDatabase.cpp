#include "LocalDatabase.h"
#include <stdlib.h> 


#define DELIMITER ","

LocalDatabase::LocalDatabase(){}

LocalDatabase::~LocalDatabase(){}

timetableEntry* LocalDatabase::getLocalDatabase(){
    return this->timetable;
}

void LocalDatabase::loadTimetable(const char* filename){
    // int curCnt = 0;
    // timetableEntry entry;
#ifdef _WIN32
    FILE* f = fopen(filename, "r");
#else
    File f = LittleFS.open(filename, "r");
#endif
    int entryNumber = 0; //tracks the entries in the database
    
    if (!f) {
        debug_print("File open failed on read.");
    } 
    else { 
        debug_print("File opened!\n");
        char linechar[50];
#ifdef _WIN32
        while(f != NULL && !feof(f) && entryNumber<DBSIZE){
            fgets(linechar, 50, f);
            if(strlen(linechar) == 0){
                debug_print("****EMPTY STRING!!!****");
                break;
            }
#else
        while(f.available() && entryNumber<DBSIZE){
            this->line = f.readStringUntil('\n');
            /*debug_print(this->line);
            debug_print(entryNumber);*/
            if (this->line == ""){
                debug_print("****EMPTY STRING!!!****");
                break;
            }
            strcpy(linechar,line.c_str()); //converting the String to char[]
#endif
            char *field = strtok(linechar, DELIMITER);
            
            //Filling lineID field
            int fieldSize = strlen(field); // gets the size of the field
            if (fieldSize > 3){ //limits the first field size
                fieldSize = 3; 
            }
            field [fieldSize] = '\0';
            strncpy(timetable[entryNumber].lineID, field, fieldSize+1); // copies the first field


            debug_print("%s ", timetable[entryNumber].lineID);

            field = strtok(NULL, DELIMITER); //strtok saves its state!
            
            //Filling hour field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2;
                timetable[entryNumber].hour = (field[0] - '0')*10 + (field[1]-'0'); 
            }else{
                timetable[entryNumber].hour = (field[0]- '0');
            }
            

            debug_print("%d ", timetable[entryNumber].hour);
            
            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling min field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2; 
                timetable[entryNumber].min = (field[0] - '0')*10 + (field[1]-'0');
            }else{
                timetable[entryNumber].min = (field[0]- '0');
            }
            
            debug_print("%d ", timetable[entryNumber].min);

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling stopName field
            fieldSize = strlen(field);
            if (fieldSize > 9){ //limits the first field size
                fieldSize = 9; 
            }
            field[fieldSize] = '\0';
            strncpy(timetable[entryNumber].stopName, field, fieldSize+1);

            debug_print("%s ", timetable[entryNumber].stopName);

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling walkingTime field
            fieldSize = strlen(field);
            if (fieldSize >= 2){ //limits the first field size
                fieldSize = 2;
                if (field[1] == 13){
                    timetable[entryNumber].walkTime = (field[0]- '0');
                } else
                timetable[entryNumber].walkTime = (field[0] - '0')*10 + (field[1]-'0');
            }
        

            debug_print("%d\n", timetable[entryNumber].walkTime);

            
            entryNumber++;
            
            

        }
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        

    }   

}


void LocalDatabase::printDBFile(const char* filename){
#ifdef _WIN32
    FILE* f = fopen(filename, "r");
    if(f != NULL){
#else
    File f = LittleFS.open(filename, "r");
    if(!f){
#endif
        debug_print("File open failed on read.");
    }
    else{
        debug_print("Printing contents of the database:");

#ifdef _WIN32
        char line[50];
        while(fgets(line, 50, f) != NULL){
            debug_print(line);
        }
#else
        for(unsigned int i=0;i<f.size();i++) //Read up to complete file size
        {
            debug_print("%c", (char)f.read());
        }
#endif
      debug_print("");
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        
    }

}

void LocalDatabase::printDatabase(){
    for(int i = 0; i < DBSIZE; i++){
        
        debug_print("%s, %i, %i, %s, %i \n",timetable[i].lineID, timetable[i].hour,timetable[i].min,timetable[i].stopName,timetable[i].walkTime);

    }

}


int LocalDatabase::compareElements(const void *a, const void *b){
    /*casting to use the corresponding type of the database*/
    timetableEntry* um = (timetableEntry*)a;
    timetableEntry* dois = (timetableEntry*)b;
    
  
    if ((((um->hour * 60) + um->min - um->walkTime) - ((dois->hour * 60) + dois->min - dois->walkTime)) < 0){ //'um' is smaller than 'dois' means transport 'um' passes earlier than 'dois' in the timetable.
        return -1;
    } else if ((((um->hour * 60) + um->min - um->walkTime ) - ((dois->hour * 60) + dois->min - dois->walkTime)) > 0) { //'um' is bigger than 'dois' means transport 'um' passes later than 'dois' in the timetable.
       return 1;
    } else{
        return 0; //case both have same time.
    }
    

}

void LocalDatabase::sortDatabase(){

    qsort(timetable, DBSIZE, sizeof(timetableEntry),LocalDatabase::compareElements);

    

}
