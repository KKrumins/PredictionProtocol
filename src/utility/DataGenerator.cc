#include <iostream>
#include <sstream>
#include <string.h>
#include <random>
#include <omnetpp.h>
#include "DataGenerator.h"

DataGenerator::DataGenerator() {
    // TODO Auto-generated constructor stub

}

DataGenerator::~DataGenerator() {
    // TODO Auto-generated destructor stub
}

//START -Random data generator methods

int DataGenerator::random(int low, int high)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}
int DataGenerator::getGoodLQI(){
    return random(170,255);
}
int DataGenerator::getGoodRSSI(){
    return random(-55,-40);
}
int DataGenerator::getMediumLQI(){
    return  random(85,170);
}
int DataGenerator::getMediumRSSI(){
    return random(-70,-55);
}
int DataGenerator::getBadLQI(){
    return  random(1,85);
}
int DataGenerator::getBadRSSI(){
    return random(-90,-70);
}
int DataGenerator::getCurrentDay(simtime_t currentTime){
    double timeInSeconds = currentTime.dbl();
    int secondsInDay = 86400;

    if(timeInSeconds > secondsInDay*7) timeInSeconds = timeInSeconds / 7; //If more than one day goes by

    if(timeInSeconds <= secondsInDay) return 1;
    if(timeInSeconds > secondsInDay && timeInSeconds <= secondsInDay*2) return 2;
    if(timeInSeconds > secondsInDay*2 && timeInSeconds <= secondsInDay*3) return 1;
    if(timeInSeconds > secondsInDay*3 && timeInSeconds <= secondsInDay*4) return 2;

//    Not used in simulations
//    if(timeInSeconds > secondsInDay*2 && timeInSeconds <= secondsInDay*3) return 3; //Wednesday
//    if(timeInSeconds > secondsInDay*3 && timeInSeconds <= secondsInDay*4) return 4; //Thursday
//    if(timeInSeconds > secondsInDay*4 && timeInSeconds <= secondsInDay*5) return 5; //Friday
//    if(timeInSeconds > secondsInDay*5 && timeInSeconds <= secondsInDay*6) return 6; //Saturday
//    if(timeInSeconds > secondsInDay*6 && timeInSeconds <= secondsInDay*7) return 7; //Sunday

    return 0;
}
int DataGenerator::getTimePeriod(simtime_t currentTime){
    double timeInSeconds = currentTime.dbl();
    int twoHours = 7200; //In seconds

    if(timeInSeconds > 86400 && timeInSeconds < 172000) timeInSeconds = timeInSeconds - 86400; //If more than one day goes by
    if(timeInSeconds > 172000) timeInSeconds = timeInSeconds /12; //If more than one day goes by

    if(timeInSeconds > 0 && timeInSeconds <= twoHours) return 1; //00:00 - 02:00
    if(timeInSeconds > twoHours && timeInSeconds <= twoHours * 2) return 2; //02:00 - 04:00
    if(timeInSeconds > twoHours * 2 && timeInSeconds <= twoHours * 3) return 3; //04:00 - 06:00
    if(timeInSeconds > twoHours * 3 && timeInSeconds <= twoHours * 4) return 4; //06:00 - 08:00
    if(timeInSeconds > twoHours * 4 && timeInSeconds <= twoHours * 5) return 5; //08:00 - 10:00
    if(timeInSeconds > twoHours * 5 && timeInSeconds <= twoHours * 6) return 6; //10:00 - 12:00
    if(timeInSeconds > twoHours * 6 && timeInSeconds <= twoHours * 7) return 7; //12:00 - 14:00
    if(timeInSeconds > twoHours * 7 && timeInSeconds <= twoHours * 8) return 8; //14:00 - 16:00
    if(timeInSeconds > twoHours * 8 && timeInSeconds <= twoHours * 9) return 9; //16:00 - 18:00
    if(timeInSeconds > twoHours * 9 && timeInSeconds <= twoHours * 10) return 10; //18:00 - 20:00
    if(timeInSeconds > twoHours * 10 && timeInSeconds <= twoHours * 11) return 11; //20:00 - 22:00
    if(timeInSeconds > twoHours * 11 && timeInSeconds <= twoHours * 12) return 12; //22:00 - 00:00

    return 0;//Just return 0 of none of the periods
}

//END -Random data generator methods
