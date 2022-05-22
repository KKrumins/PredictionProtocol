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

#include "LinearRegression.h"
#include<bits/stdc++.h>
#include "C:/MagWorkspace/PredictionRouting/src/utility/DataGenerator.h"

using namespace std;

LinearRegression::LinearRegression() {
    // TODO Auto-generated constructor stub

}

LinearRegression::~LinearRegression() {
    // TODO Auto-generated destructor stub
}

void LinearRegression::train(){

//    DataGenerator dataGenerator;
//       double x1[10], x2[10], y[10];
//       for(int i = 0; i < 10; i++){
//           if(i <= 5){
//               x1[i] = dataGenerator.getGoodLQI();
//               x2[i] = dataGenerator.getGoodRSSI();
//               y[i] = 1.0;
//           }
//           else{
//               x1[i] = dataGenerator.getBadLQI();
//               x2[i] = dataGenerator.getBadRSSI();
//               y[i] = 0.0;
//           }//
//       }

    //Only lqi and rssi
    double x1[] = {0,20,40,75,90,        95,120,180,220,255};
    double x2[] = {-90,-85,-80,-75,-70,  -65,-60,-55,-50,-40};
    double y[] =  {0, 0, 0, 0, 0,        1, 1, 1, 1, 1};

    vector<double>error; // for storing the error values
    double err;    // for calculating error on each stage
    double alpha = 0.01; // initializing learning rate
    double  e = 2.71828;

    /*Training Phase*/
    for (int i = 0; i < 12; i ++) { 
        int idx = i % 10;   //for accessing index after every epoch
        double p = -(b0 + b1 * x1[idx]+ b2* x2[idx]); //making the prediction
        double pred  = 1/(1+ pow(e,p)); //calculating final prediction applying sigmoid
        err = y[idx]-pred;  //calculating the error
        b0 = b0 - alpha * err*pred *(1-pred)* 1.0;   //updating b0
        b1 = b1 + alpha * err * pred*(1-pred) *x1[idx];//updating b1
        b2 = b2 + alpha * err * pred*(1-pred) * x2[idx];//updating b2
        cout<<"B0="<<b0<<" "<<"B1="<<b1<<" "<<"B2="<<b2<<" error="<<err<<endl;// printing values after every step
        error.push_back(err);
        }
    sort(error.begin(),error.end(),custom_sort);//custom sort based on absolute error difference
    cout<<"Final Values are: "<<"B0="<<b0<<" "<<"B1="<<b1<<" "<<"B2="<<b2<<" error="<<error[0];
}

double LinearRegression::predict(double test1, double test2){
    double pred=b0+b1*test1+b2*test2; //make prediction
    cout<<"The value predicted by the model= "<<pred<<endl;
    return pred;
}

void LinearRegression::createAdditionalData(){

}
