#ifndef LOCALDATABASE
#define LOCALDATABASE

/*This class deals with the database of the timetable of the public transportation*/

#include <iostream>
#include <string.h>
#include "Util.h"

#ifdef _WIN32
    #include <string>
    using namespace std;
    #define String string

#else
#include <FS.h>
#include <LittleFS.h>
#endif

#define MAXSTOPS        5         //maximum number of stops
#define MAXLINES        7         //maximum number of lines to work with
#define MAXHOLIDAYS     13        //maximum number of holidays
#define MAXTIMEPERLINE  (60/5)*10 //maximum number of times the line stops at a station in a day, assuming every 5min.
// #define DBSIZE  MAXSTOPS*MAXLINES*MAXTIMEPERLINE // maximum size of the database!
#define DBSIZE  225
// #define MY_DEBUG

//database entry - schedule
typedef struct timetableEntry_type
{
    char     routeID [9];	//ID of the route/line
    int      arriveTime;	//Scheduled time for the route to arrive in the stop
    char     stopID [11]; 	//ID of the stop
    int      walkTime; 		//time it takes from your location to the stop
    char	 day[2]; 			//D = default H = holidays and weekends

}timetableEntry;

typedef struct stopsInfo_type
{
	char	stopID [11];
	char	stopName [27];
	int		walkTime;
}stopsInfo;

typedef struct holidays_type
{
    char dates[5];
}holidays;

class LocalDatabase
{
private:
    timetableEntry	timetable[DBSIZE];
    stopsInfo		stopInfo[MAXSTOPS];
    holidays        holiday[MAXHOLIDAYS];
    String			line;
    unsigned char            numStops;
public:
    LocalDatabase();
    ~LocalDatabase();

    timetableEntry *getLocalDatabase();
    stopsInfo *getLocalStopsInfo();

    unsigned char getNumStops();

    holidays *getLocaHolidays();

    bool loadHolidays(const char* holidaysFile);

    bool loadStopsInfo(const char* stopsFile);

    int walkTimeForStop(char* stopID);

    /*This function loads the DB from a csv file and stores the data into the array of struct*/
    bool loadTimetable(const char* scheduleFile);

    // void saveTimetable();

    /*This function receives a DB file and prints it to screen */
    void printDBFile(const char* filename);

    void printDatabase ();

    /*function used in qsort -> sorts the db array*/
    static int  compareElements(const void *a, const void *b);

    /*Calls qsort in order sort the array*/
    void sortDatabase();

    int isItHoliday(int day, int month);
    

};


#endif
