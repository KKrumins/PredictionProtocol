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

#ifndef UTILITY_ENERGYMANAGER_H_
#define UTILITY_ENERGYMANAGER_H_

class EnergyManager {
public:
    double energyCapacity; //Watts (2 AA) 3.7W
    double instructionCost = 0.0000000042; //4.2 nW
    double idleCost = 0;

    EnergyManager();
    virtual ~EnergyManager();
    void setEnergyCapacity(double capacity);
    void doInstruction();
    void idle();
    double getEnergyLeft();
    void die();
};

#endif /* UTILITY_ENERGYMANAGER_H_ */
