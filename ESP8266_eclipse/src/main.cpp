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

#define DBFILE "schedule.txt"
#define STOPSFILE "stops.txt"

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	LocalDatabase db;
	db.loadStopsInfo(STOPSFILE);
	db.loadTimetable(DBFILE);
	db.sortDatabase();


	cout << "DB sorted" << endl;
	db.printDatabase();
//	db.printDBFile(DBFILE);

	return 0;
}
