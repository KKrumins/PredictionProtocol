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

#ifndef UTILITY_ROUTINGTABLE_H_
#define UTILITY_ROUTINGTABLE_H_

#include <bits/stdc++.h>
using namespace std;

class RoutingTable {
public:
    RoutingTable();
    virtual ~RoutingTable();

    struct NeigbourInfo{
        int neigbourId;
        vector<int> reachableNeigbours;
    };
    vector<NeigbourInfo *> myRoutingTable;
    vector<int> myNeigbours;
    vector<int> deadNeighbourList;
    NeigbourInfo *newNeigbourInfo(int neigbourId, vector<int> neighbourNeighbours);
    void alterRoutingTable(int neigbourId, vector<int> neighbourNeighbours);
    void deleteNeighbour(int neighbourId);
};

#endif /* UTILITY_ROUTINGTABLE_H_ */
