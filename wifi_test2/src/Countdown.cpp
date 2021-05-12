#include "Countdown.h"



Countdown::Countdown(/* args */)
{
}

Countdown::~Countdown()
{
}

void Countdown::displayFormatting(char* char_in, char* char_out, int maxSize){
    int inSize = strlen(char_in);

    if(inSize > maxSize){
        inSize = maxSize;
    }
    strncpy(char_out,char_in, inSize); //copies input string to output
    for(int i = inSize; i < maxSize; i++){ //if the input string is smaller than max size, fill gaps with spaces! 
        char_out[i] = ' ';
    }
    char_out[maxSize]='\0'; //end it with \0
}




void Countdown::serialDisplayCountdown(String time, int hour, int min, int sec, timetableEntry *timetable){
    
    int currentTime = hour*60*60 + min*60 + sec; // current time in seconds
    // Serial.println(currentTime);//******************************************TESTE
    // Serial.println();

    int scheduleTime = 0;
    int displayLimit = 0; //limit to show only 5 entries

    debug_print("############### %s ################\n", time.c_str());
    for(int i = 0; i < DBSIZE; i++){
        timetableEntry item = timetable[i]; //current timetable line

        scheduleTime = item.arriveTime; //schedule time of the transport in seconds

        // Serial.println(scheduleTime);//**************************************TESTE

        int remainingTime = scheduleTime - (currentTime + item.walkTime); //time difference in seconds between now + walk time to the station and the schedule of the transport
        // Serial.println(remainingTime);//**************************************TESTE
        // Serial.println();
        int remainingTimeHour = remainingTime/3600;
        int remainingTimeMin = (remainingTime - remainingTimeHour*3600)/60;
        int remainingTimeSec = remainingTime - remainingTimeHour*3600 - remainingTimeMin*60;

        // Serial.println(remainingTimeHour);Serial.println(":");Serial.println(remainingTimeMin);Serial.println(":");Serial.println(remainingTimeSec);//**************************************TESTE

        /*If you still get time to catch the transport, print to display*/        
        if(remainingTime > 0){
            if(displayLimit < 4){
            	debug_print("%s\t%s\t%i:%i\t%i:%i:%i \n", item.stopID, item.routeID, scheduleTime/3600, (scheduleTime - (scheduleTime/3600)*3600)/60, remainingTimeHour, remainingTimeMin, remainingTimeSec);
                displayLimit++;
            }
        

        }

    }
    debug_print("#########################################\n\n");
    


}

void Countdown::serialDisplayPerStopCountdown(char* stopID,int hour, int min, int sec, int isHoliday, int weekday, timetableEntry *timetable, stopsInfo *stops){

	int currentTime = hour*60*60 + min*60 + sec; // current time in seconds
	int scheduleTime = 0;
	int displayLimit = 0; //limit to show only 5 entries
	char* stopName;

	for (int n = 0; n < MAXSTOPS; n++) {
		if(strcmp(stopID, stops[n].stopID) == 0){
			stopName = stops[n].stopName;
			break;
		}
	}

	debug_print("(%s) - %s \n\n",stopID, stopName);

	for(int i = 0; i < DBSIZE; i++){
		timetableEntry item = timetable[i]; //current timetable line

		if(strcmp(item.stopID, stopID)==0){
            bool holiday = item.day[0] == 'H' && (weekday == 0 || weekday == 6 || isHoliday);
            bool workDay = item.day[0] == 'D' && weekday>0 && weekday<6 && !isHoliday;

            if(holiday||workDay){

                scheduleTime = item.arriveTime; //schedule time of the transport in seconds

                // Serial.println(scheduleTime);//**************************************TESTE

                int remainingTime = scheduleTime - (currentTime + item.walkTime); //time difference in seconds between now + walk time to the station and the schedule of the transport

                int remainingTimeHour = remainingTime/3600;
                int remainingTimeMin = (remainingTime - remainingTimeHour*3600)/60;
                int remainingTimeSec = remainingTime - remainingTimeHour*3600 - remainingTimeMin*60;


                /*If you still get time to catch the transport, print to display*/
                if(remainingTime > 0){
                    if(displayLimit < 4){
                        debug_print("%s\t%i:%i\t%i:%i:%i \n", item.routeID, scheduleTime/3600, (scheduleTime - (scheduleTime/3600)*3600)/60, remainingTimeHour, remainingTimeMin, remainingTimeSec);
                        displayLimit++;
                    }


                }

            }
		}
	}
	debug_print("\n");
}


void Countdown::displayCountdown (Adafruit_SSD1306 &display, String time, int hour, int min, int sec, timetableEntry *timetable){
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    int currentTime = hour*60*60 + min*60 + sec; // current time in seconds
    
    int scheduleTime = 0;
    int displayLimit = 0; //limit to show only 5 entries

    display.printf("###### %s #####\n\n", time.c_str());
    display.display();

    for(int i = 0; i < DBSIZE; i++){
        timetableEntry item = timetable[i]; //current timetable line

        scheduleTime = item.arriveTime; //schedule time of the transport in seconds

        int remainingTime = scheduleTime - (currentTime + item.walkTime); //time difference in seconds between now + walk time to the station and the schedule of the transport

        int remainingTimeHour = remainingTime/3600;
        int remainingTimeMin = (remainingTime - remainingTimeHour*3600)/60;
       // int remainingTimeSec = remainingTime - remainingTimeHour*3600 - remainingTimeMin*60;

       
        /*If you still get time to catch the transport, print to display*/        
        if(remainingTime > 0){
            if(displayLimit < 4){

                // char tempStopName[10];
                // char tempLineID[4];

                // displayFormatting(item.stopName,tempStopName, 9);
                // displayFormatting(item.lineID, tempLineID, 3);

                display.printf("%s %i:%i %i:%i \n", item.routeID, scheduleTime/3600, (scheduleTime - (scheduleTime/3600)*3600)/60, remainingTimeHour, remainingTimeMin);
                displayLimit++;
                display.display();
            }
        }
        
       

    }
    display.println("#####################");
    display.display();



}

void Countdown::displayCountdownPerStop(Adafruit_SSD1306 &display, char* stopID,int hour, int min, int sec, int isHoliday, int weekday, timetableEntry *timetable, stopsInfo *stops){
	display.clearDisplay();
	display.setCursor(0,0);
	display.setTextSize(1);
	display.setTextColor(WHITE);
    char* stopName = NULL;
	int currentTime = hour*60*60 + min*60 + sec; // current time in seconds
	int scheduleTime = 0;
	int displayLimit = 0; //limit to show only 5 entries

    /*Getting the name of the current stop!*/
    for (int i = 0; i < MAXSTOPS; i++)
    {
        if(strcmp(stopID, stops[i].stopID) == 0){
            stopName = stops[i].stopName;
            break;
        }
    }
    

	display.printf("%s\n\n", stopName);
	display.display();

	for(int i = 0; i < DBSIZE; i++){
		
		
		timetableEntry item = timetable[i]; //current timetable line

        if(strcmp(item.stopID, stopID) == 0){
            bool holiday = (item.day[0] == 'H' && (weekday == 0 || weekday == 6 || isHoliday));
            bool workDay = (item.day[0] == 'D' && weekday>0 && weekday<6 && !isHoliday);
            

            if(holiday||workDay){
                
                scheduleTime = item.arriveTime; //schedule time of the transport in seconds

                int remainingTime = scheduleTime - (currentTime + item.walkTime); //time difference in seconds between now + walk time to the station and the schedule of the transport

                int remainingTimeHour = remainingTime/3600;
                int remainingTimeMin = (remainingTime - remainingTimeHour*3600)/60;
            // int remainingTimeSec = remainingTime - remainingTimeHour*3600 - remainingTimeMin*60;

            
                /*If you still get time to catch the transport, print to display*/        
                if(remainingTime > 0){
                    if(displayLimit < 4){

                        // char tempStopName[10];
                        // char tempLineID[4];

                        // displayFormatting(item.stopName,tempStopName, 9);
                        // displayFormatting(item.lineID, tempLineID, 3);

                        display.printf("%s %i:%i %i:%i \n", item.routeID, scheduleTime/3600, (scheduleTime - (scheduleTime/3600)*3600)/60, remainingTimeHour, remainingTimeMin);
                        displayLimit++;
                        display.display();
                    }
                }
            }    
        }

		
	   

	}
	
	display.display();
}



