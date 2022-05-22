//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Tree.h"
#include <bits/stdc++.h>
#include "C:/MagWorkspace/PredictionRouting/src/utility/DataGenerator.h"
#include "C:/MagWorkspace/PredictionRouting/src/utility/Helpers.h"
using namespace std;

Tree::Tree() {
    // TODO Auto-generated constructor stub
}

Tree::~Tree() {
    // TODO Auto-generated destructor stub
}

Tree::PeriodNode *Tree::newPeriodNode(int key, int index){
    PeriodNode *temp = new PeriodNode;
    temp->key = key;
    temp->index = index;
    return temp;
}

Tree::NeighbourNode *Tree::newNeigbourNode(int id, double predValue, int index){
    NeighbourNode *temp = new NeighbourNode;
    temp->neighId = id;
    temp->predictedValue = predValue;
    temp->index = index;
    return temp;
}

Tree::RootNode *Tree::newRootNode(int key, int index){
    RootNode *temp = new RootNode;
    temp->key = key;
    temp->index = index;
    return temp;
}

bool Tree::treeExists(int key){
    int treeListSize = static_cast<std::vector<int>::size_type>(treeList.size());
    for(int i = 0; i < treeListSize; i++){
        int k = treeList[i]->key;
        if(k == key) return true;
    }
    return false;
}

Tree::RootNode *Tree::getTreeRoot(int key){
    int treeListSize = static_cast<std::vector<int>::size_type>(treeList.size());
    for(int i = 0; i < treeListSize; i++){
      int k = treeList[i]->key;
      if(k == key) return treeList[i];

    }
    return NULL;
}

Tree::PeriodNode *Tree::getPeriod(int key, RootNode *root){
    if(key == 2){
        int x = 1;
    }
    int periodListSize = static_cast<std::vector<int>::size_type>(root->periodList.size());
    for(int i = 0; i < periodListSize; i++){
        int k = root->periodNode[i]->key;
        if(k == key) return root->periodNode[i];
    }
    return NULL;
}

Tree::NeighbourNode *Tree::getNeigbour(int id, RootNode *root, PeriodNode *periodNode){
    int neigbourListSize = static_cast<std::vector<int>::size_type>(periodNode->neigbourList.size());
    for(int i = 0; i < neigbourListSize; i++){
        int nId = periodNode->neighbourNode[i]->neighId;
        if(nId == id) return periodNode->neighbourNode[i];
    }
}

bool Tree::periodExists(int key,RootNode *root){
    if(find(root->periodList.begin(),root->periodList.end(),key) != root->periodList.end()) return true;
    else return false;
};

bool Tree::neigbourExists(int id, PeriodNode * period){
    if(period == NULL) return false;
    if(find(period->neigbourList.begin(),period->neigbourList.end(),id) != period->neigbourList.end()) return true;
    else return false;
};

void Tree::process(int neighbourId, double predictedValue){
    DataGenerator dataGenerator;

    int currentDay = dataGenerator.getCurrentDay(simTime());
    int currentPeriod = dataGenerator.getTimePeriod(simTime());
    if(currentDay == 2){
        int debugger = 1;
    }

    if(!treeExists(currentDay)) {
        createNewTree(neighbourId,predictedValue,currentDay,currentPeriod);
    }
    else{
        updateTree(neighbourId,predictedValue,currentDay,currentPeriod);
    }
}

void Tree::deleteNeigbour(int neighbourId){
    DataGenerator dataGenerator;

    int currentDay = dataGenerator.getCurrentDay(simTime());
    int currentPeriod = dataGenerator.getTimePeriod(simTime());

    RootNode *root = getTreeRoot(currentDay);
    PeriodNode *period = getPeriod(currentPeriod, root);
    NeighbourNode *neigbour = getNeigbour(neighbourId, root, period);

    period->neighbourNode.erase(remove(period->neighbourNode.begin(),period->neighbourNode.end(), neigbour), period->neighbourNode.end());
    //Delete from tree
    //Delete from neigbourList
    period->neigbourList.erase(remove(period->neigbourList.begin(),period->neigbourList.end(), neighbourId), period->neigbourList.end());
}

void Tree::createNewTree(int neighbourId, double predictedValue, int currentDay, int currentPeriod){

    RootNode *dayNode = newRootNode(currentDay,0);
    dayNode->dayList.push_back(currentDay);

    treeList.push_back(dayNode); //Holds pointers to all trees

    //Create new period
    int periodSize = static_cast<std::vector<int>::size_type>(dayNode->periodList.size());
    (dayNode->periodNode).push_back(newPeriodNode(currentPeriod,periodSize));
    dayNode->periodList.push_back(currentPeriod);

    (dayNode->periodNode[0]->neighbourNode).push_back(newNeigbourNode(neighbourId, predictedValue, 0));
    dayNode->periodNode[0]->neigbourList.push_back(neighbourId);
}

void Tree::updateTree(int neighbourId, double predictedValue, int cureentDay, int currentPeriod){
    PeriodNode *periodPointer = nullptr;
    NeighbourNode *neigbourPointer = nullptr;

    RootNode *root = getTreeRoot(cureentDay);

    periodPointer = getPeriod(currentPeriod,root);

    if(periodPointer == NULL){
        //If no period found, create new one and create neigbour as well
        int periodSize = static_cast<std::vector<int>::size_type>(root->periodList.size());
        (root->periodNode).push_back(newPeriodNode(currentPeriod,periodSize));
        root->periodList.push_back(currentPeriod);
        periodPointer = root->periodNode[periodSize];

        (root->periodNode[periodSize]->neighbourNode).push_back(newNeigbourNode(neighbourId, predictedValue, 0));
        periodPointer->neigbourList.push_back(neighbourId);
    }
    else{
        if(!neigbourExists(neighbourId,periodPointer)){
            //Create new neigbour if it doesnt exist
            int neighbourListSize = static_cast<std::vector<int>::size_type>(periodPointer->neigbourList.size());
            (periodPointer->neighbourNode).push_back(newNeigbourNode(neighbourId, predictedValue, neighbourListSize ));
            periodPointer->neigbourList.push_back(neighbourId);
        }
        else{
            //If neigbour exists, then only update predicted value
            neigbourPointer = getNeigbour(neighbourId, root, periodPointer);
            neigbourPointer->predictedValue = predictedValue;
        }
    }
}

int Tree::decideReceiverId(int senderId, vector<int>route, vector<int> neigboursWithOneOrNoneNeighbours){
    //Decide based on the highest probability
    DataGenerator dataGenerator;
    Helpers helper;

    int currentDay = dataGenerator.getCurrentDay(simTime());
    int currentPeriod = dataGenerator.getTimePeriod(simTime());

    RootNode *root = getTreeRoot(currentDay);
    if(root == NULL) return 0;
    PeriodNode *period = getPeriod(currentPeriod, root);
    if(period == NULL) return 0;
    vector<int> tempNeighList = period->neigbourList;
    remove(tempNeighList.begin(),tempNeighList.end(),senderId);

    helper.removeDuplicates(tempNeighList);

    int neighbourListSize = static_cast<std::vector<int>::size_type>(period->neigbourList.size());

    double maxProbability = -100;
    int receiverId = 0;
    //Add send back if senderId == only neighbour id

    for(int i = 0; i < neighbourListSize; i++){
        if(std::find(tempNeighList.begin(), tempNeighList.end(), period->neighbourNode[i]->neighId) != tempNeighList.end()){

            if(std::find(route.begin(), route.end(), period->neighbourNode[i]->neighId) == route.end()
               && std::find(neigboursWithOneOrNoneNeighbours.begin(), neigboursWithOneOrNoneNeighbours.end(), period->neighbourNode[i]->neighId) == neigboursWithOneOrNoneNeighbours.end()){

                if(period->neighbourNode[i]->predictedValue > maxProbability){
                    maxProbability = period->neighbourNode[i]->predictedValue;
                    receiverId =period->neighbourNode[i]->neighId;
                }
            }
        }
    }
    return receiverId;
}

bool Tree::canSendDirect(int neighId){
    DataGenerator dataGenerator;

    int currentDay = dataGenerator.getCurrentDay(simTime());
    int currentPeriod = dataGenerator.getTimePeriod(simTime());

    if(currentDay == 2){
        int debugger = 1;
    }

    RootNode *root = getTreeRoot(currentDay);
    if(root == NULL) return false;
    PeriodNode *period = getPeriod(currentPeriod, root);

    if(period == NULL) return false;
    int neighbourListSize = static_cast<std::vector<int>::size_type>(period->neighbourNode.size());
    if(currentPeriod == 3) {
        int debugHere = 1;
    }

    double neighProbability = 0;

    for(int i = 0; i < neighbourListSize; i++){
        if(period->neighbourNode[i]->neighId == neighId){
            neighProbability = period->neighbourNode[i]->predictedValue;
            break;
        }
    }

    return neighProbability > 0;//Need to fix these values (-5 etc)
}

double Tree::getNeighbourProbability(int neighId){
    DataGenerator dataGenerator;

    int currentDay = dataGenerator.getCurrentDay(simTime());
    int currentPeriod = dataGenerator.getTimePeriod(simTime());

    RootNode *root = getTreeRoot(currentDay);
    if(root == NULL) return 0;
    PeriodNode *period = getPeriod(currentPeriod, root);
    if(period == NULL) {
        return 0;
    }

    int neighbourListSize = static_cast<std::vector<int>::size_type>(period->neighbourNode.size());

    double neighProbability = 0;

    for(int i = 0; i < neighbourListSize; i++){
       if(period->neighbourNode[i]->neighId == neighId){
           neighProbability = period->neighbourNode[i]->predictedValue;
           break;
       }
    }
    return neighProbability;
}















