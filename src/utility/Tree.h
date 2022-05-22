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

#ifndef UTILITY_TREE_H_
#define UTILITY_TREE_H_

#include <bits/stdc++.h>
using namespace std;

class Tree {
public:
    Tree();
    virtual ~Tree();

    struct NeighbourNode{
         int neighId;
         int index;
         double predictedValue;
         //No children
    };

    struct PeriodNode{
        int key;
        int index;
        vector<int> neigbourList;
        vector<NeighbourNode*> neighbourNode;
    };

    struct RootNode{
        int key;
        int index;
        vector<PeriodNode*> periodNode;
        vector<int> dayList;
        vector<int> periodList;
    };

    struct NeighbourPredictions{
        int neighbourId;
        double predictedValue;
    };

    //General tree list
    vector<RootNode *> treeList;

    PeriodNode *newPeriodNode(int key, int index);
    NeighbourNode *newNeigbourNode(int key, double predValue, int index);
    RootNode *newRootNode(int key, int index);
    bool periodExists(int key,RootNode *root);
    bool neigbourExists(int id, PeriodNode * period);

    bool treeExists(int key);
    RootNode *getTreeRoot(int key);
    PeriodNode *getPeriod(int key, RootNode *root);
    NeighbourNode *getNeigbour(int id, RootNode *root, PeriodNode *periodNode);

    void process(int id, double predValue);
    void createNewTree(int neighbourId, double predictedValue, int cureentDay, int currentPeriod);
    void updateTree(int neighbourId, double predictedValue, int cureentDay, int currentPeriod);
    void deleteNeigbour(int neighbourId);
    int decideReceiverId(int senderId,vector<int>route, vector<int> neigboursWithOneOrNoneNeighbours);
    bool canSendDirect(int neighId);
    double getNeighbourProbability(int neighId);
};

#endif /* UTILITY_TREE_H_ */
