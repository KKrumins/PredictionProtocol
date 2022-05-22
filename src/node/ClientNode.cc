#include <omnetpp.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <random>
#include <algorithm>

#include "../regression/linear_regression.cc"
#include "C:/MagWorkspace/PredictionRouting/src/message/InfoMessage_m.h"
#include "C:/MagWorkspace/PredictionRouting/src/message/HelloMessage_m.h"
#include "C:/MagWorkspace/PredictionRouting/src/message/RoutingPacket_m.h"
#include "C:/MagWorkspace/PredictionRouting/src/message/ResponseMessage_m.h"
#include "C:/MagWorkspace/PredictionRouting/src/message/ConfirmationMsg_m.h"
#include "C:/MagWorkspace/PredictionRouting/src/message/ErrorMessage_m.h"

#include "C:/MagWorkspace/PredictionRouting/src/utility/DataGenerator.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/LinearRegression.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/Tree.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/RoutingTable.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/Helpers.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/EnergyManager.h"

using namespace std;
using namespace omnetpp;

class clientNode : public cSimpleModule {
private:

public:
    //Timers
    simtime_t timerInterval;
    simtime_t waitingTimeInterval;
    cMessage * sendHelloMessages; //Timer for sending hello messages
    cMessage * waitForResponse;
    cMessage * updateRoutes;
    cMessage * startRoutingTask;
    cMessage * waitMsgFromDestination;
    cMessage * waitConfirmMessage;
    cMessage * dayChecker;
    cMessage * idle;
    cMessage * dayChange;
    cMessage * periodChecker;

    //Variables
    int numberOfNodesInNetwork = 4;//TODO: Should be changes to automatic node generation
    int myId;
    int routingSent = 0;
    int routingReceived = 0;
    int numSent = 0;
    int numReceived = 0;
    int numLost = 0;
    int hops = 0;
    int helloMsgTimes = 0;
    int controlHelloMessageTimes = 3;
    int confirmationNeighId = 0;
    double capacity;
    int totalSentCount = 0;

    //Times
    simtime_t sentTime = 0;
    simtime_t receiveTime = 0;
    simtime_t latency = 0;

    //Vectors
    vector<int> helloMsgNeigbours; //neighIds of whom the hello message was sent to
    vector<int> receivedHelloMsgsNeighbours; //neighIds from whom the hello message was received back;
    vector<int> noResponseNodes;
    vector<int> neigboursWithOneOrNoneNeighbours;
    RoutingPacket * tempRoutingPck;

    //Objects
    Tree tree;
    RoutingTable rt;
    Helpers helper;
    EnergyManager energy;

    //Booleans
    bool receivedLostNodes = false;
    bool routeDiscoveryFinished = false;
    bool useBestRoutes;
    bool sabotageRoutes;
    bool dataGatherFaseFinished = false;
    bool hasError = false;

    //Signals
    simsignal_t sentPacketCount;
    simsignal_t energyCapacity;
    simsignal_t hopCount;
    simsignal_t lostPackets;
    simsignal_t Node1_Node2;
    simsignal_t Node2_Node1;
    simsignal_t PDR;
    //simsignal_t RoutingSent;
    //simsignal_t RoutingReceived;
    simsignal_t SuccessRate;
    simsignal_t Latency;
    simsignal_t TotalPacketCount;

    //Methods
    virtual void initialize();
    virtual void handleMessage(omnetpp::cMessage *msg);
    void sendDirectMsg(cMessage * msg, int destinationId);
    void sendHelloMessage();
    void sendDirectHelloMessage(int receiverId);
    void sendInfoPacket(int neighbourId);
    void doCheckup();
    void processInfoPacket(InfoMessage *infoMsg, int neighbourId,LinearRegression logReg);
    void sendRoutingPacket(int destinationId);
    void reSendRoutingPacket(RoutingPacket *routingPacket);
    vector<int> assigntReachableNeigbours();
    int chooseRoute(int destinationId,int senderId,vector<int>route);
    void sendResponseMessage(RoutingPacket *routingPacket);
    void reSendResponseMessage(ResponseMessage *responseMessaeg);
    bool messageSent(int receiverId);
    void sendConfirmMessage(RoutingPacket *routingPacket);
    void sendErrorMessage(RoutingPacket *routingPacket, int deadNode);
    void reSendErrorMessage(ErrorMessage *error);
};

Define_Module(clientNode);

void clientNode::initialize() {
    EV << "Client "<< myId <<" initialize" << "\n";

    //Take params
    myId = par("myId");
    useBestRoutes = par("useBestRoutes");
    sabotageRoutes = par("sabotageRoutes");
    capacity = par("capacity");

    //Register signals for statistics
    sentPacketCount = registerSignal("sentPacketCount");
    energyCapacity = registerSignal("energyCapacity");
    hopCount = registerSignal("hopCount");
    lostPackets = registerSignal("lostPackets");
    Node1_Node2 = registerSignal("Node1_Node2");
    Node2_Node1 = registerSignal("Node2_Node1");
    PDR = registerSignal("PDR");
    SuccessRate = registerSignal("SuccessRate");
    Latency = registerSignal("Latency");
    TotalPacketCount = registerSignal("TotalPacketCount");

    energy.setEnergyCapacity(capacity);

    //Register time counter
    timerInterval = 1 + myId; //every two hours
    sendHelloMessages = new cMessage("HelloMessageTimerr");
    waitForResponse = new cMessage("wait for response timer");
    updateRoutes = new cMessage("Route info updating");
    startRoutingTask = new cMessage("Routing task");
    waitMsgFromDestination = new cMessage("Wait msg from dest. timer");
    waitConfirmMessage = new cMessage("Wait for confirm message timer");
    dayChecker = new cMessage("Check if day has changed");
    idle = new cMessage("Idle energy cost");
    dayChange = new cMessage("Day change event");
    periodChecker = new cMessage("Period change event");

    //scheduleAt(simTime() + 1,idle);
    scheduleAt(simTime() + 1, dayChecker);
    scheduleAt(simTime() + timerInterval + myId, sendHelloMessages);
    scheduleAt(simTime() + 7200, updateRoutes);
    scheduleAt(simTime() + 7200, periodChecker);
    scheduleAt(simTime() + 86400, dayChange);

    if(myId == 1){
        scheduleAt(simTime() + 40, startRoutingTask);
    }
    EV << "Client "<< myId <<" initialized" << "\n";
}

void clientNode::handleMessage(omnetpp::cMessage *msg){
    energy.doInstruction();
    emit(energyCapacity,energy.getEnergyLeft());
    if(energy.getEnergyLeft() > 0){
        if(msg == periodChecker){
          scheduleAt(simTime() + 7200, updateRoutes);
          if(periodChecker->isScheduled()) cancelEvent(periodChecker);
          scheduleAt(simTime() + 7200, periodChecker);
          emit(sentPacketCount,numSent);
          emit(lostPackets,numLost);
          double successRate  = ((double)routingReceived / (double)routingSent) * 100;
          if(routingReceived != 0)  {
              emit(SuccessRate,successRate);
          }
          numSent = 0;
          numLost = 0;
        }
        else if(msg == dayChecker){
            if(routingReceived != 0) emit(PDR,routingSent/routingReceived);
            scheduleAt(simTime() + 1, dayChecker);
            if(simTime() > 7200) {
                //Sabotage routes for one period and revert later
                sabotageRoutes = !sabotageRoutes;
            }
            if(simTime() > 7200 * 2){
                //Revert sabotageRoutes
                sabotageRoutes = !sabotageRoutes;
            }
           if(simTime() > 172000){
               dataGatherFaseFinished = true;
           }
           if(simTime() > 172800){
               dataGatherFaseFinished = true;
           }
           if(simTime() > 345550){
               emit(TotalPacketCount, totalSentCount);
           }
        }
        else if (msg == dayChange){
            scheduleAt(simTime()+ 86400,dayChange);
        }
        else if (msg == sendHelloMessages) {
           if(helloMsgTimes != 2){
               sendHelloMessage();
           }
           else{
               doCheckup();
           }
        }
        else if(msg == updateRoutes){
           //If it is time for update, schedule hello message sending
           receivedHelloMsgsNeighbours.clear();
           helloMsgTimes = 0;
           if(!dataGatherFaseFinished){
               bubble("New period! Need to update routes");
               scheduleAt(simTime() + myId, sendHelloMessages);
           }
           //if(sendHelloMessages->isScheduled()) cancelEvent(sendHelloMessages);
           if(dataGatherFaseFinished && hasError){
               bubble("There was an error. Need to update");
               scheduleAt(simTime() + myId, sendHelloMessages);
               hasError = false;
           }
        }
        else if(msg == startRoutingTask){
           //Start routing task
           int routingTasks = 0;
           if(routingTasks != 20){
               int destinationNodeId = rand() %20 + 1;
               EV << "Client "<< myId <<" is starting routing task! Destination node:" << destinationNodeId <<" \n";
               if(destinationNodeId != myId) {
                   bubble("I am starting routing task!");
                   sendRoutingPacket(destinationNodeId);
               }
               routingTasks++;
           }else routingTasks = 0;
           if(startRoutingTask->isScheduled()) cancelEvent(startRoutingTask);
           scheduleAt(simTime() + 320, startRoutingTask);
        }
        else if(msg == waitForResponse){
           EV << "Client waiting for lost nodes \n";

           bubble("Didn't receive any lost messages");
           numLost++;
           hasError = true;
           int noResponseNodesListSize = static_cast<std::vector<int>::size_type>(noResponseNodes.size());
           if(updateRoutes->isScheduled()){
              cancelEvent(updateRoutes);
              scheduleAt(simTime() + 1, updateRoutes);
           }
           for(int i = 0; i < noResponseNodesListSize; i++){
               tree.deleteNeigbour(noResponseNodes[i]);
               rt.deleteNeighbour(noResponseNodes[i]);
               rt.deadNeighbourList.push_back(noResponseNodes[i]);
               helper.removeDuplicates(rt.deadNeighbourList);
               sendErrorMessage(tempRoutingPck, noResponseNodes[i]);
           }
           receivedLostNodes = false;
           noResponseNodes.clear();

        }
        else if(msg == waitMsgFromDestination){
           bubble("Didn't receive response from destination");
           numLost++;
        }
        else if(msg == waitConfirmMessage){
           bubble("Didn't receive confirmation msg");
           numLost++;
           //emit(lostPackets,numLost);
           receivedHelloMsgsNeighbours.erase(remove(receivedHelloMsgsNeighbours.begin(),receivedHelloMsgsNeighbours.end(), confirmationNeighId), receivedHelloMsgsNeighbours.end());
           //helper.removeDuplicates(receivedHelloMsgsNeighbours);
           doCheckup();
        }
        else
        {
           numReceived++; //Anything that gets received
           //Get the message type(kind)
           short msgKind = msg->getKind();
           if(msg->isSelfMessage()){
               EV << "Client"<<myId<<" handle self message \n";
           }

           //If receive hello message, send info message
           if(msgKind == 1){
               HelloMessage *helloMsg = check_and_cast<HelloMessage *>(msg);

               int neighbourId = helloMsg->getSenderId();
               vector<int> senderNeighbours = helloMsg->getMyNeigbours();

               // *********** Create routing table and update it every time message is received from others
               rt.alterRoutingTable(neighbourId, senderNeighbours);

               EV << "Client " << myId << "handle HELLO message from " << neighbourId << "\n";

               std::string value = "Received HELLO msg from ";
               std::string s = value + std::to_string(neighbourId);
               char const *bubbleString = s.c_str();

               bubble(bubbleString);
               //Send back infoMsg
               sendInfoPacket(neighbourId);
           }

           //If receive infoMessage
           if(msgKind == 2){
               LinearRegression logReg;
               InfoMessage *infoMsg = check_and_cast<InfoMessage *>(msg);
               int neighbourId = infoMsg->getSenderId();
               if(helloMsgTimes < 2){
                   if(sendHelloMessages->isScheduled()) cancelEvent(sendHelloMessages);
                   scheduleAt(simTime() + 1, sendHelloMessages);

                   if(waitForResponse->isScheduled()){
                       EV << "Client "<<myId<<" received message form lost node \n";

                       vector<int> originalNoResp = noResponseNodes;
                       vector<int> alteredNoResp = noResponseNodes;
                       remove(alteredNoResp.begin(),alteredNoResp.end(),neighbourId);
                       noResponseNodes = helper.intersetVectors(originalNoResp,alteredNoResp);

                       receivedLostNodes = true;
                       processInfoPacket(infoMsg,neighbourId,logReg);
                   }
                   else{
                       processInfoPacket(infoMsg,neighbourId,logReg);
                   }
               }
           }
           //If receive routing packet
           if(msgKind == 3){
               bubble("Received routing msg");
               RoutingPacket *routingPacket = check_and_cast<RoutingPacket *>(msg);

               //Get packet kind
               if(routingPacket->getDestinationNodeId() == myId){
                   sendResponseMessage(routingPacket);
               }
               else{
                   //Send confirmation msg
                   tempRoutingPck = routingPacket;
                   sendConfirmMessage(routingPacket);
                   reSendRoutingPacket(routingPacket);
               }
           }
           //If receive response message
           if(msgKind == 4){
               bubble("Received response msg");
               if(waitConfirmMessage->isScheduled()){
                   cancelEvent(waitConfirmMessage);
               }
               ResponseMessage *responseMessage = check_and_cast<ResponseMessage *>(msg);
               if(waitMsgFromDestination->isScheduled()&& myId == responseMessage->getRoute().front()){
                   cancelEvent(waitMsgFromDestination);
                   bubble("Received the response to message I sent out earlier :) ");

                   routingReceived++;
                   //Get receive time
                   receiveTime = responseMessage->getArrivalTime().dbl();
                   //Calculate latency
                   latency = receiveTime - sentTime.dbl();
                   //Emit
                   emit(Latency,latency.dbl());
                   emit(hopCount,responseMessage->getHops());
               }
               else{
                   bubble("Not for me! Will re-send");
                   reSendResponseMessage(responseMessage);
               }
           }
           //If receive confirm message
           if(msgKind == 5){
               bubble("Received confirm msg. All coolio");
               cancelEvent(waitConfirmMessage);
           }
           //If receive error msg
           if(msgKind == 6){
               ErrorMessage *error = check_and_cast<ErrorMessage *>(msg);
               if(error->getReceiverId() == myId){
                   //TODO: What to do with the error
                   bubble("This error was for me");
                   cancelEvent(waitMsgFromDestination);
               }
               else{
                   //re-send
                   reSendErrorMessage(error);
               }
           }
        }
    }
    else{
        bubble("I died !!!!!!");
        emit(TotalPacketCount, totalSentCount);
        cDisplayString& displayString = getDisplayString();
        displayString.setTagArg("i", 0, "old/x_cross");
    }

}

void clientNode::sendHelloMessage(){
    //Send this message to all other nodes to find neighbour nodes and get info from them
    std::string value = "HelloMessage_From_";
    std::string s = value + std::to_string(myId);
    char const *helloMessageName = s.c_str();

    HelloMessage *helloMessage = new HelloMessage(helloMessageName);
    helloMessage->setSenderId(myId);
    helloMessage->setWaitInterval(0);
    helloMessage->setMyNeigbours(rt.myNeigbours);
    helloMessage->setKind(helloMessage->getPacketId());

    helloMsgNeigbours.clear();

    //Hardcoded neighbour IDs for each node in order to simulate radio transmission range and reachable nodes
    vector<int> reachableNodesList = assigntReachableNeigbours();
    int reachableNodeListSize = static_cast<std::vector<int>::size_type>(reachableNodesList.size());

    for(int i = 0; i < reachableNodeListSize; i++){
        int nId = reachableNodesList[i];
        if(nId != myId){
            //Copy message so it can be sent to multiple nodes without need of scheduling
            cMessage *copy = helloMessage->dup();

            // *********** Put neighbour ids, whom hello message is sent to, in the list;
            helloMsgNeigbours.push_back(nId);
            bubble("Sending hello message");
            sendDirectMsg(copy,nId);
        }
    }
    energy.doInstruction();
    emit(energyCapacity,energy.getEnergyLeft());
    helloMsgTimes++;
    delete helloMessage;
}

void clientNode::sendDirectHelloMessage(int receiverId){
    //Send this message to all other nodes to find neighbour nodes and get info from them
    std::string value = "ControlHelloMessage_From_";
    std::string s = value + std::to_string(myId);
    char const *helloMessageName = s.c_str();

    HelloMessage *helloMessage = new HelloMessage(helloMessageName);
    helloMessage->setSenderId(myId);
    helloMessage->setWaitInterval(0);
    helloMessage->setMyNeigbours(rt.myNeigbours);
    helloMessage->setKind(helloMessage->getPacketId());

    for(int i = 1; i < controlHelloMessageTimes; i++){
       //Copy message so it can be sent to multiple nodes without need of scheduling
       cMessage *copy = helloMessage->dup();

       bubble("Sending hello message");
       sendDirectMsg(copy,receiverId);
    }
    delete helloMessage;
    energy.doInstruction();
    emit(energyCapacity,energy.getEnergyLeft());
}

void clientNode::sendInfoPacket(int neighbourId){
    //This is a response message to hello message
    DataGenerator dataGenerator;

    InfoMessage *infoMessage = new InfoMessage("InfoMessageFromClient");

    infoMessage->setSenderId(myId);
    infoMessage->setReceiverId(neighbourId);
    if(sabotageRoutes && ((myId == 1 && neighbourId == 2) || (myId == 2 && neighbourId == 1))){
        infoMessage->setLQI(dataGenerator.getBadLQI());
        infoMessage->setRSSI(dataGenerator.getBadRSSI());
    }
    else if(sabotageRoutes && ((myId == 9 && neighbourId == 10) || (myId == 10 && neighbourId == 9))){
           infoMessage->setLQI(dataGenerator.getBadLQI());
           infoMessage->setRSSI(dataGenerator.getBadRSSI());
    }
    else{
        infoMessage->setLQI(dataGenerator.getGoodLQI());
        infoMessage->setRSSI(dataGenerator.getGoodRSSI());
    }

    infoMessage->setDay(dataGenerator.getCurrentDay(simTime()));
    infoMessage->setTimePeriod(dataGenerator.getTimePeriod(simTime()));

    infoMessage->setKind(infoMessage->getPacketId());

    bubble("Send info message");
    sendDirectMsg(infoMessage,neighbourId);
    energy.doInstruction();
}

void clientNode::doCheckup(){
    // *********** Check if all nodes responded. If no, send controlHelloMessage 3 times to those who didn't
    bubble("I will do a checkup");
    if(!startRoutingTask->isScheduled() && myId == 1){
        //scheduleAt(simTime() + 40, startRoutingTask);
    }
    //Delete not responding node from tree, neigbourList and routing table;
    noResponseNodes = helper.intersetVectors(helloMsgNeigbours,receivedHelloMsgsNeighbours);

    int noResponseNodesListSize = static_cast<std::vector<int>::size_type>(noResponseNodes.size());
    if(noResponseNodesListSize > 0){
        bubble("Lost nodes found !!!");
        for(int i = 0; i < noResponseNodesListSize; i++){
            sendDirectHelloMessage(noResponseNodes[i]);
        }
        waitingTimeInterval = simTime() + 2;
        scheduleAt(waitingTimeInterval, waitForResponse);
    }
    else{
        bubble("No lost nodes found");
    }
}

void clientNode::processInfoPacket(InfoMessage *infoMsg, int neighbourId,LinearRegression logReg){
    EV << "Client " << myId << "handle INFO message from " << neighbourId << "\n";

    std::string value = "Received INFO msg from ";
    std::string s = value + std::to_string(neighbourId);
    char const *bubbleString = s.c_str();
    bubble(bubbleString);

    // *********** Send to learning model

//    int length_train;
//    const char* filename = "C:\MagWorkspace\PredictionRouting\src\regression\test.csv";
//    std::cout << "Reading CSV \n";
//    Dataset data = read_csv(filename);
//
//    // Regression Variables
//    int max_iteration = 1000;
//    float learning_rate = 0.1;
//
//    // Training
//    std::cout << "Making LinearRegressionModel \n";
//    LinearRegressionModel linear_reg = LinearRegressionModel(data);
//    std::cout << "Training \n";
//    linear_reg.train(max_iteration, learning_rate);
//
//    float X_test[2];
//    X_test[0] = 1;
//    X_test[1] = 3;
//    float predVal = linear_reg.predict(X_test);

    logReg.train();
    double x1 = infoMsg->getLQI();
    double x2 = infoMsg->getRSSI();
    double predVal = logReg.predict(x1,x2);

    if(myId == 1 && neighbourId == 2){
        emit(Node1_Node2,predVal);
    }
    if(myId == 2 && neighbourId == 1){
        emit(Node2_Node1,predVal);
    }

    // *********** Create/update tree
    tree.process(neighbourId,predVal);

    // *********** Create list of own neighbour ids

    if(find(rt.myNeigbours.begin(),rt.myNeigbours.end(),neighbourId) == rt.myNeigbours.end())
      rt.myNeigbours.push_back(neighbourId);

    receivedHelloMsgsNeighbours.push_back(neighbourId);
    helper.removeDuplicates(receivedHelloMsgsNeighbours);
    receivedLostNodes = false;
}

vector<int> clientNode::assigntReachableNeigbours(){
    //Hardcoded values in order to simulate radio transmission range
    vector<int> reachableNodesList;
    if(myId == 1){
        reachableNodesList.push_back(2);
        reachableNodesList.push_back(4);
        reachableNodesList.push_back(20);
    }
    if(myId == 2){
        reachableNodesList.push_back(1);
        reachableNodesList.push_back(3);
        reachableNodesList.push_back(8);
        reachableNodesList.push_back(20);
    }
    if(myId == 3){
        reachableNodesList.push_back(2);
        reachableNodesList.push_back(8);
        reachableNodesList.push_back(15);
    }
    if(myId == 4){
        reachableNodesList.push_back(1);
        reachableNodesList.push_back(5);
        reachableNodesList.push_back(12);
        reachableNodesList.push_back(11);
    }
    if(myId == 5){
        reachableNodesList.push_back(4);
        reachableNodesList.push_back(6);
        reachableNodesList.push_back(20);
    }
    if(myId == 6){
       reachableNodesList.push_back(5);
       reachableNodesList.push_back(7);
       reachableNodesList.push_back(9);
       reachableNodesList.push_back(8);
    }
    if(myId == 7){
       reachableNodesList.push_back(6);
       reachableNodesList.push_back(10);
       reachableNodesList.push_back(14);
    }
    if(myId == 8){
      reachableNodesList.push_back(6);
      reachableNodesList.push_back(3);
      reachableNodesList.push_back(15);
    }
    if(myId == 9){
      reachableNodesList.push_back(6);
      reachableNodesList.push_back(10);
      reachableNodesList.push_back(15);
      reachableNodesList.push_back(16);
    }
    if(myId == 10){
      reachableNodesList.push_back(9);
      reachableNodesList.push_back(7);
      reachableNodesList.push_back(16);
      reachableNodesList.push_back(18);
    }
    if(myId == 11){
     reachableNodesList.push_back(1);
     reachableNodesList.push_back(4);
     reachableNodesList.push_back(12);
    }
    if(myId == 12){
     reachableNodesList.push_back(11);
     reachableNodesList.push_back(4);
     reachableNodesList.push_back(13);
    }
    if(myId == 13){
     reachableNodesList.push_back(14);
     reachableNodesList.push_back(12);
    }
    if(myId == 14){
     reachableNodesList.push_back(13);
     reachableNodesList.push_back(7);
     reachableNodesList.push_back(19);
    }
    if(myId == 15){
     reachableNodesList.push_back(3);
     reachableNodesList.push_back(8);
     reachableNodesList.push_back(9);
     reachableNodesList.push_back(16);
    }
    if(myId == 16){
     reachableNodesList.push_back(15);
     reachableNodesList.push_back(10);
     reachableNodesList.push_back(9);
     reachableNodesList.push_back(17);
    }
    if(myId == 17){
     reachableNodesList.push_back(16);
     reachableNodesList.push_back(18);
    }
    if(myId == 18){
     reachableNodesList.push_back(19);
     reachableNodesList.push_back(17);
     reachableNodesList.push_back(10);
    }
    if(myId == 19){
     reachableNodesList.push_back(14);
     reachableNodesList.push_back(18);
    }
    if(myId == 20){
     reachableNodesList.push_back(1);
     reachableNodesList.push_back(8);
     reachableNodesList.push_back(2);
     reachableNodesList.push_back(5);
    }

    return reachableNodesList;
}

int clientNode::chooseRoute(int destinationId, int senderId, vector<int> route){
    int receiverId = 0;
    int myNeighboursListSize = static_cast<std::vector<int>::size_type>(rt.myNeigbours.size());
    int myRoutingTableSize = static_cast<std::vector<int>::size_type>(rt.myRoutingTable.size());

    if(myNeighboursListSize == 1) {
        return rt.myNeigbours.front();
    }

    if(std::find(rt.deadNeighbourList.begin(), rt.deadNeighbourList.end(), destinationId) != rt.deadNeighbourList.end()){
        bubble("Can't send to dead node!");
        return 0;
    }
    else {
        //Check if I can reach the node directly
           for(int i = 0; i < myNeighboursListSize; i++){
              if(std::find(rt.myNeigbours.begin(), rt.myNeigbours.end(), destinationId) != rt.myNeigbours.end()){
                  if(std::find(route.begin(), route.end(), destinationId) == route.end()){
                      if(useBestRoutes){
                          //If the probability is good
                          if(tree.canSendDirect(destinationId)) receiverId = destinationId;
                          break;
                      }
                      else {
                          receiverId = destinationId;
                          break;
                      }
                  }
              }
           }
           //Check if any neighbours can reach destination node directly
           if(receiverId == 0){ //If receiverId is still not set
              for(int i = 0; i < myRoutingTableSize; i++){
                   int neighId = rt.myRoutingTable[i]->neigbourId;
                   vector<int> reachableNeigbours = rt.myRoutingTable[i]->reachableNeigbours;
                   int reachableNeighbourSize = static_cast<std::vector<int>::size_type>(reachableNeigbours.size());
                   if(reachableNeighbourSize <= 1){
                       neigboursWithOneOrNoneNeighbours.push_back(neighId);
                       helper.removeDuplicates(neigboursWithOneOrNoneNeighbours);
                   }
                   //If can reach, set receiverId to that neighbourId
                   if(std::find(reachableNeigbours.begin(), reachableNeigbours.end(), destinationId) != reachableNeigbours.end() && reachableNeighbourSize > 1){
                       //Check if destination is in the route
                       if(std::find(route.begin(), route.end(), neighId) == route.end()){
                           if(useBestRoutes){
                               if(tree.canSendDirect(neighId)) receiverId = neighId;
                               break;
                           }
                           else {
                               receiverId = neighId;
                               break;
                           }

                       }
                   }
              }
           }

           if(receiverId == 0){
              //Get info from the tree
              receiverId = tree.decideReceiverId(senderId,route,neigboursWithOneOrNoneNeighbours);
           }

           //Check probability
           if(messageSent(receiverId)) return receiverId;
           else return 0;
    }
}

void clientNode::sendRoutingPacket(int destinationId){
    RoutingPacket *routingPacket = new RoutingPacket("RoutingPacket");

    routingPacket->setSenderId(myId);
    routingPacket->setDestinationNodeId(destinationId);

    vector<int> route = routingPacket->getRoute();
    route.push_back(myId);
    routingPacket->setRoute(route);
    routingPacket->setKind(3);

    //Get next receiver
    int receiverId = chooseRoute(destinationId, myId,route);
    if(receiverId != 0){
        confirmationNeighId = receiverId;

        bubble("Sending routing packet");
        sendDirectMsg(routingPacket,receiverId);
        routingSent++;
        scheduleAt(simTime() + 15,waitMsgFromDestination); //Wait for 15 seconds
        if(waitConfirmMessage->isScheduled()){
          cancelEvent(waitConfirmMessage);
        }
        scheduleAt(simTime() + 2,waitConfirmMessage);
        energy.doInstruction();
    }
}

void clientNode::reSendRoutingPacket(RoutingPacket *routingPacket){
    //Only update route
    vector<int> route = routingPacket->getRoute();
    route.push_back(myId);
    routingPacket->setRoute(route);

    //Choose receiver
    int receiverId = chooseRoute(routingPacket->getDestinationNodeId(),routingPacket->getSenderId(),route);

    routingPacket->setSenderId(myId);
    if(receiverId != 0 ){
        confirmationNeighId = receiverId;

        bubble("Re-sending routing packet");
        sendDirectMsg(routingPacket,receiverId);
        if(waitConfirmMessage->isScheduled()){
          cancelEvent(waitConfirmMessage);
        }
        scheduleAt(simTime() + 2,waitConfirmMessage);
        energy.doInstruction();
    }
}

void clientNode::sendResponseMessage(RoutingPacket *routingPacket){
    //Remove myId from the list
    vector<int> route = routingPacket->getRoute();
    remove(route.begin(),route.end(),myId);
    helper.removeDuplicates(route);
    //Reverse the route list
    std::reverse(route.begin(), route.end());

    int spentHops = static_cast<std::vector<int>::size_type>(routingPacket->getRoute().size());

    ResponseMessage *responseMessage = new ResponseMessage("Response_msg");
    responseMessage->setRoute(route);
    responseMessage->setKind(4);
    responseMessage->setHops(spentHops);
    responseMessage->setArrivalTime(simTime()); //Used for latency

    bubble("Sending response message");
    sendDirectMsg(responseMessage,route.front());
    energy.doInstruction();
}

void clientNode::reSendResponseMessage(ResponseMessage *responseMessage){
    //Remove myId from the list and re-send
    vector<int> route = responseMessage->getRoute();
    remove(route.begin(),route.end(),myId);
    helper.removeDuplicates(route);
    responseMessage->setRoute(route);

    bubble("Re-sending response msg");
    sendDirectMsg(responseMessage,route.front());
    energy.doInstruction();
}

void clientNode::sendDirectMsg(cMessage * msg, int destinationId){
    omnetpp::cModule *target;

    std::string value = "clientNode";
    std::string s = value + std::to_string(destinationId);
    char const *clientModuleName = s.c_str();

    target = getParentModule()->getSubmodule(clientModuleName);

    //Ranadom decimal delay generator. Assumed distance between nodes is ~ 50m so travel time is ~ 0.005ms
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<double> distr(0.001, 0.010);

    simtime_t delay = distr(eng);
    if(msg->getKind() == 3) sentTime = (delay + simTime()).dbl();

    sendDirect(msg,delay,0,target,"radioIn");
    numSent++;
    totalSentCount++;
}

void clientNode::sendConfirmMessage(RoutingPacket *routingPacket){
    ConfirmationMsg * confirmMsg = new ConfirmationMsg("Confirmation message");
    confirmMsg->setSenderId(myId);
    confirmMsg->setReceiverId(routingPacket->getSenderId());
    confirmMsg->setKind(5);

    bubble("Sending confirmation msg");
    sendDirectMsg(confirmMsg,routingPacket->getSenderId());
    energy.doInstruction();
}

void clientNode::sendErrorMessage(RoutingPacket *routingPacket, int deadNode){
    vector<int> route = routingPacket->getRoute();
    remove(route.begin(),route.end(),myId);
    helper.removeDuplicates(route);
    //Reverse the route list
    std::reverse(route.begin(), route.end());

    ErrorMessage * error = new ErrorMessage("Error message");

    error->setReceiverId(route.back());
    error->setDeadNodeId(deadNode);
    error->setRoute(route);
    error->setKind(6);

    bubble("Sending error message");
    sendDirectMsg(error,route.front());
    energy.doInstruction();
    emit(energyCapacity,energy.getEnergyLeft());
}

void clientNode::reSendErrorMessage(ErrorMessage *error){
    //Remove myId from the list and re-send
    vector<int> route = error->getRoute();
    remove(route.begin(),route.end(),myId);
    helper.removeDuplicates(route);
    error->setRoute(route);

    bubble("Re-sending response msg");
    sendDirectMsg(error,route.front());
    energy.doInstruction();
    emit(energyCapacity,energy.getEnergyLeft());
}

bool clientNode::messageSent(int receiverId){
    double probability = tree.getNeighbourProbability(receiverId);
    int num = rand() % 100 + 1;
    //TODO: Split up more
    if(probability > 0){ //Good probability
        if(num >= 5) return true;
        else return false;
    }
    else if (probability < 0 && probability > -5){ //Medium probability
        if(num >= 30) return true;
        else return false;
    }
    else{ //Bad probability
        if(num >= 85) return true;
        else return false;
    }
}


