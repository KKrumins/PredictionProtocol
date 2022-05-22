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

#ifndef UTILITY_HELPERS_H_
#define UTILITY_HELPERS_H_

#include <bits/stdc++.h>
using namespace std;

class Helpers {
public:
    Helpers();
    virtual ~Helpers();

    void removeDuplicates(std::vector<int> &v);
    vector<int> intersetVectors(vector<int> v1, vector<int> v2);

};

#endif /* UTILITY_HELPERS_H_ */
