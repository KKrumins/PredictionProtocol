#ifndef UTILITY_DATAGENERATOR_H_
#define UTILITY_DATAGENERATOR_H_

#include <omnetpp.h>

using namespace omnetpp;

class DataGenerator {
public:
    DataGenerator();
    virtual ~DataGenerator();

    int random(int low, int high);
    int getGoodLQI();
    int getGoodRSSI(); //dBm
    int getMediumLQI();
    int getMediumRSSI();//dBm
    int getBadLQI();
    int getBadRSSI();//dBm
    int getCurrentDay(simtime_t currentTime);
    int getTimePeriod(simtime_t currentTime);
};

#endif /* UTILITY_DATAGENERATOR_H_ */
