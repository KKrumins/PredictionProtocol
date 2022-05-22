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

#ifndef UTILITY_LOGISTICREGRESSION_H_
#define UTILITY_LOGISTICREGRESSION_H_

#include <iostream>

using namespace std;

class LinearRegression {
private:
    double b0 = 0; // initializing b0
    double b1 = 0; // initializing b1
    double b2=  0; // initializing b2
    double b3=  0; // initializing b3
    double b4=  0; // initializing b4
    double b5=  0; // initializing b5
public:
    LinearRegression();
    virtual ~LinearRegression();


    void train();
    double predict(double test1, double test2);
    void createAdditionalData();
    static bool custom_sort(double a, double b)
    {
        /* this custom sort function is defined to sort on basis of min absolute value or error*/
        double  a1=abs(a-0);
        double  b1=abs(b-0);
        return a1<b1;
    }
};

#endif /* UTILITY_LOGISTICREGRESSION_H_ */
