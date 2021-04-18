#include "Countdown.h"



Countdown::Countdown(/* args */)
{
}

Countdown::~Countdown()
{
}



void Countdown::displayCountdown(String time, int hour, int min, int sec, timetableEntry *timetable){
    
    int currentTime = hour*60*60 + min*60 + sec; // current time in seconds
    // Serial.println(currentTime);//******************************************TESTE
    // Serial.println();

    int scheduleTime = 0;
    int displayLimit = 0; //limit to show only 5 entries

    Serial.printf("############### %s ################\n", time.c_str());
    for(int i = 0; i < DBSIZE; i++){
        timetableEntry item = timetable[i]; //current timetable line

        scheduleTime = item.hour*60*60 + item.min*60; //schedule time of the transport in seconds

        // Serial.println(scheduleTime);//**************************************TESTE

        int remainingTime = scheduleTime - (currentTime + item.walkTime*60); //time difference in seconds between now + walk time to the station and the schedule of the transport
        // Serial.println(remainingTime);//**************************************TESTE
        // Serial.println();
        int remainingTimeHour = remainingTime/3600;
        int remainingTimeMin = (remainingTime - remainingTimeHour*3600)/60;
        int remainingTimeSec = remainingTime - remainingTimeHour*3600 - remainingTimeMin*60;

        // Serial.println(remainingTimeHour);Serial.println(":");Serial.println(remainingTimeMin);Serial.println(":");Serial.println(remainingTimeSec);//**************************************TESTE

        /*If you still get time to catch the transport, print to display*/        
        if(remainingTime > 0){
            if(displayLimit < 4){
                Serial.printf("%s\t\t%s\t%i:%i\t%i:%i:%i \n", item.stopName, item.lineID, item.hour, item.min, remainingTimeHour, remainingTimeMin, remainingTimeSec);
                displayLimit++;
            }
        }
        
       

    }
    Serial.println("#########################################");
    


}