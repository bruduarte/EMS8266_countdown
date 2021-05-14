//============================================================================
// Name        : ESP8266_eclipse.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Util.h"

#include <iostream>
using namespace std;

#include "LocalDatabase.h"
#include "Countdown.h"

#define DBFILE "schedule.txt"
#define STOPSFILE "stops.txt"
#define HOLIDAYSFILE "holidays.txt"

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	LocalDatabase db;
	Countdown c;
	stopsInfo* stop;
	stop = db.getLocalStopsInfo();

	db.loadStopsInfo(STOPSFILE);
	db.loadTimetable(DBFILE);
	db.loadHolidays(HOLIDAYSFILE);
	db.sortDatabase();


	cout << "DB sorted" << endl;
	db.printDatabase();
//	db.printDBFile(DBFILE);

	db.getLocaHolidays();

	c.serialDisplayCountdown("19:50:30", 6, 0, 30, db.getLocalDatabase());

	for (int i = 0; i < MAXSTOPS; i++) {

		c.serialDisplayCountdownPerStop(stop[i].stopID, 0, 20, 10, db.getLocalDatabase(), db.getLocalStopsInfo());

	}

	return 0;
}
