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

#include "RoutingTable.h"

RoutingTable::RoutingTable() {
    // TODO Auto-generated constructor stub

}

RoutingTable::~RoutingTable() {
    // TODO Auto-generated destructor stub
}

RoutingTable::NeigbourInfo *RoutingTable::newNeigbourInfo(int neigbourId, vector<int> neighbourNeighbours){
    NeigbourInfo *temp = new NeigbourInfo;
    temp->neigbourId = neigbourId;
    temp->reachableNeigbours = neighbourNeighbours;
    return temp;
}

void RoutingTable::alterRoutingTable(int neigbourId, vector<int> neighbourNeighbours){
    int routingTableSize = static_cast<std::vector<int>::size_type>(myRoutingTable.size());

    bool neighExists = false;
    int neighIndex = -1;
    if(routingTableSize == 0) myRoutingTable.push_back(newNeigbourInfo(neigbourId,neighbourNeighbours));
    else{
        for(int i = 0; i < routingTableSize; i++){
            int nId = myRoutingTable[i]->neigbourId;
            if(nId == neigbourId) {
                neighExists = true;
                neighIndex = i;
            }
        }

        if(neighExists){
            myRoutingTable[neighIndex]->reachableNeigbours = neighbourNeighbours;
        }
        else {
            myRoutingTable.push_back(newNeigbourInfo(neigbourId,neighbourNeighbours));
            if(std::find(deadNeighbourList.begin(), deadNeighbourList.end(), neigbourId) != deadNeighbourList.end()){
                deadNeighbourList.erase(remove(deadNeighbourList.begin(),deadNeighbourList.end(), neigbourId), deadNeighbourList.end());
            }
        }
    }
}

void RoutingTable::deleteNeighbour(int neighbourId){
    int routingTableSize = static_cast<std::vector<int>::size_type>(myRoutingTable.size());

    myNeigbours.erase(remove(myNeigbours.begin(),myNeigbours.end(), neighbourId), myNeigbours.end());

    for(int i = 0; i < routingTableSize; i++){
       int nId = myRoutingTable[i]->neigbourId;
       if(nId == neighbourId) {
           NeigbourInfo *info = myRoutingTable[i];
           myRoutingTable.erase(remove(myRoutingTable.begin(),myRoutingTable.end(), info), myRoutingTable.end());
       }
    }
}
