#include "LocalDatabase.h"
#include <stdlib.h> 


#define DELIMITER ","

LocalDatabase::LocalDatabase(){}

LocalDatabase::~LocalDatabase(){}

timetableEntry* LocalDatabase::getLocalDatabase(){
    return this->timetable;
}

stopsInfo* LocalDatabase::getLocalStopsInfo(){
	return this->stopInfo;
}

holidays* LocalDatabase::getLocaHolidays(){
    return this->holiday;
}

unsigned char LocalDatabase::getNumStops(){
    return this->numStops;
}

bool LocalDatabase::loadHolidays(const char* holidaysFile){
    char lineChar[10]; //to storage the line being read

#ifdef _WIN32
    FILE* f = fopen(holidaysFile, "r");
#else
    File f = LittleFS.open(holidaysFile, "r");
#endif

    int entryNumber = 0; //tracks the entries in the file

    if (!f) {
        debug_print("File open failed on read.");
        return false;
    }
    else {
        debug_print("File opened!\n");

#ifdef _WIN32
		while(f != NULL && !feof(f) && entryNumber<MAXHOLIDAYS){
			fgets(lineChar, 5, f);
			if(strlen(lineChar) == 0){
				debug_print("****EMPTY STRING!!!****");
				break;
			}
#else
        debug_print("Holidays:\n");
        while(f.available() && entryNumber<MAXHOLIDAYS){
            this->line = f.readStringUntil('\n');
            // Serial.printf("%d: %s\n", entryNumber,this->line.c_str());
            if (this->line == ""){
                debug_print("****EMPTY STRING!!!****\n");
                break;
            }
#endif
            strncpy(lineChar,line.c_str(), 4); //converting the String to char[]
            lineChar [4] = '\0';
            // int lineSize = strlen(lineChar);
            strncpy(holiday[entryNumber].dates, lineChar, 5); //copies the dates!

            debug_print("%s\n", holiday[entryNumber].dates);
            entryNumber++;
        }
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        return true;
    }
}

bool LocalDatabase::loadStopsInfo(const char* stopFile){

#ifdef _WIN32
    FILE* f = fopen(stopFile, "r");
#else
    File f = LittleFS.open(stopFile, "r");
#endif

    int entryNumber = 0; //tracks the entries in the file

    if (!f) {
        debug_print("File open failed on read.");
        return false;
    }
    else {
        debug_print("File opened!\n");

        char lineChar[50]; //to storage the line being read
#ifdef _WIN32
		while(f != NULL && !feof(f) && entryNumber<MAXSTOPS){
			fgets(lineChar, 50, f);
			if(strlen(lineChar) == 0){
				debug_print("****EMPTY STRING!!!****");
				break;
			}
#else
        while(f.available() && entryNumber<MAXSTOPS){
            this->line = f.readStringUntil('\n');
            /*debug_print(this->line);
            debug_print(entryNumber);*/
            if (this->line == ""){
                debug_print("****EMPTY STRING!!!****");
                break;
            }
            strcpy(lineChar,line.c_str()); //converting the String to char[]
#endif

            char *field = strtok(lineChar, DELIMITER); //taking each field from the line
            //Filling stopID field
            int fieldSize = strlen(field); // gets the size of the field
            if (fieldSize > 10){ //limits the first field size
                fieldSize = 10;
            }
            field [fieldSize] = '\0';
            strncpy(stopInfo[entryNumber].stopID, field, fieldSize+1); // copies the first field


            debug_print("%s ", stopInfo[entryNumber].stopID);

            field = strtok(NULL, DELIMITER); //strtok saves its state! taking next field of the line
            //Filling stopName field
            fieldSize = strlen(field);
            if (fieldSize > 26){ //limits the first field size
                fieldSize = 26;
            }
            field[fieldSize] = '\0';
            strncpy(stopInfo[entryNumber].stopName, field, fieldSize+1);

            debug_print("%s ", stopInfo[entryNumber].stopName);

            field = strtok(NULL, DELIMITER); //strtok saves its state!

            //Filling walkingTime field
            stopInfo[entryNumber].walkTime = atoi(field);

            debug_print("%d\n", stopInfo[entryNumber].walkTime);

            entryNumber++;

		}
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        this->numStops = entryNumber;
        return true;
	}
}

int LocalDatabase::walkTimeForStop(char* stopID){

	for(int i =0; i<MAXSTOPS; i++){

		int ret = strcmp(this->stopInfo[i].stopID, stopID);

		if (ret == 0){
			return this->stopInfo[i].walkTime;
		}

	}

	return -1;

}

bool LocalDatabase::loadTimetable(const char* scheduleFile){
    // int curCnt = 0;
    // timetableEntry entry;
#ifdef _WIN32
    FILE* f = fopen(scheduleFile, "r");
#else
    File f = LittleFS.open(scheduleFile, "r");
#endif
    int entryNumber = 0; //tracks the entries in the database
    
    if (!f) {
        debug_print("File open failed on read.");
        return false;
    }else {
        debug_print("File opened!\n");
        char lineChar[50]; //to storage the line being read
#ifdef _WIN32
        while(f != NULL && !feof(f) && entryNumber<DBSIZE){
            fgets(lineChar, 50, f);
            if(strlen(lineChar) == 0){
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
            strcpy(lineChar,line.c_str()); //converting the String to char[]
#endif
            char *field = strtok(lineChar, DELIMITER); //taking each field from the line
            
            //Filling routeID field
            int fieldSize = strlen(field); // gets the size of the field
            if (fieldSize > 8){ //limits the first field size
                fieldSize = 8;
            }
            field [fieldSize] = '\0';
            strncpy(timetable[entryNumber].routeID, field, fieldSize+1); // copies the first field


            // debug_print("%s ", timetable[entryNumber].routeID);

            field = strtok(NULL, DELIMITER); //strtok saves its state! taking next field of the line
            
            //Filling arriveTime field
            fieldSize = strlen(field);
            if (fieldSize >= 5){ //limits the first field size
                fieldSize = 5;
            }

            timetable[entryNumber].arriveTime = atoi(field); //converting string to int, saving second field


            // debug_print("%d ", timetable[entryNumber].arriveTime);
            
            field = strtok(NULL, DELIMITER); //strtok saves its state! taking next field of the line

            //Filling stopID field
            fieldSize = strlen(field);
            if (fieldSize > 10){ //limits the first field size
                fieldSize = 10;
            }
            field[fieldSize] = '\0';
            strncpy(timetable[entryNumber].stopID, field, fieldSize+1); // copies the third field

            // debug_print("%s ", timetable[entryNumber].stopID);

            field = strtok(NULL, DELIMITER); //strtok saves its state! taking next field of the line

            //filling day field
            fieldSize = strlen(field);
			if (fieldSize > 2){ //limits the first field size
				fieldSize = 2;
			}
			field[1] = '\0';

            strncpy(timetable[entryNumber].day,field, fieldSize+1); //saves the 'day' field

            //Filling walkingTime field
            timetable[entryNumber].walkTime = walkTimeForStop(this->timetable[entryNumber].stopID);

            // debug_print("%d\n", timetable[entryNumber].walkTime);

            
            entryNumber++;
            
            

        }
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        return true;
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
      debug_print(" ");
#ifdef _WIN32
        fclose(f);
#else
        f.close();
#endif
        
    }

}

void LocalDatabase::printDatabase(){
    for(int i = 0; i < DBSIZE; i++){
        
        debug_print("%i %s %i %s %s %i \n",i, timetable[i].routeID, timetable[i].arriveTime,timetable[i].stopID,timetable[i].day,timetable[i].walkTime);

    }

}


int LocalDatabase::compareElements(const void *a, const void *b){
    /*casting to use the corresponding type of the database*/

    timetableEntry* um = (timetableEntry*)a;
    timetableEntry* dois = (timetableEntry*)b;
    

    if (((um->arriveTime - um->walkTime) - ((dois->arriveTime - dois->walkTime))) < 0){ //'um' is smaller than 'dois' means transport 'um' passes earlier than 'dois' in the timetable.
        return -1;
    } else if (((um->arriveTime - um->walkTime ) - ((dois->arriveTime - dois->walkTime))) > 0) { //'um' is bigger than 'dois' means transport 'um' passes later than 'dois' in the timetable.
       return 1;
    } else{
        return 0; //case both have same time.
    }
    

}

void LocalDatabase::sortDatabase(){

    qsort(timetable, DBSIZE, sizeof(timetableEntry),LocalDatabase::compareElements);


}

int LocalDatabase::isItHoliday(int day, int month){

    char today[5] = {}; //saves date DDMM
    snprintf(today, 3, "%02d", day);
    snprintf(today+2, 3, "%02d", month);
    today[4] = '\0';

    for (int i = 0; i < MAXHOLIDAYS; i++) {
        if(strcmp(today, holiday[i].dates) == 0){
            debug_print("Holiday: %s\n", today);
            return 1;
        }
                
    }

    return 0;

}