/*This class deals with the database of the timtable of the public transportation*/

#include <iostream>

#define MAXSTOPS        5         //maximum number of stops
#define MAXLINES        3         //maximum number of lines to work with
#define MAXTIMEPERLINE  (60/5)*24 //maximum number of times the line stops at a station in a day, assuming every 5min.

//database entry
struct timetableEntry
{
    char     lineID [100];
    char     hour [100];
    char     min [100];      
    char     stopName [100]; //name of the stop
    char     walkTime [100]; //time it takes from your location to the stop

};
  const uint16_t size =  MAXSTOPS * MAXLINES * MAXTIMEPERLINE;

class LocalDatabase
{
private:
    struct timetableEntry timetable[size];

public:
    LocalDatabase();
    ~LocalDatabase();
    void loadTimetable(const char* filename);
    // void saveTimetable();
    void printDatabase(const char* filename);
};


