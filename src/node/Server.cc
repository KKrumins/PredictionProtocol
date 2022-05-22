#include <omnetpp.h>
#include "C:/MagWorkspace/PredictionRouting/src/message/InfoMessage_m.h"

using namespace omnetpp;

// !!!!!!!!!!!! Not used !!!!!!!!!!!!!!!!

class server : public cSimpleModule {
    cMessage *msg;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};

Define_Module(server);

void server::initialize() {

}

void server::handleMessage(cMessage *msg){

    msg = new cMessage("CTS");

    InfoMessage *lohs = new InfoMessage("InfoMessageFromServer");

    lohs->setPacketId(2);
    lohs->setSenderId(1);
    lohs->setReceiverId(2);
    lohs->setLQI(166);
    lohs->setRSSI(11);
    lohs->setDay(1);
    //infoMessage->setTimePeriod();;

    cModule *target = getParentModule()->getSubmodule("clientNode");
    scheduleAt(omnetpp::simTime() + dblrand(),lohs->dup());

    sendDirect(msg,target,"radioIn");
}
