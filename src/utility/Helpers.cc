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

#include "Helpers.h"

Helpers::Helpers() {
    // TODO Auto-generated constructor stub

}

Helpers::~Helpers() {
    // TODO Auto-generated destructor stub
}

void Helpers::removeDuplicates(std::vector<int> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

vector<int> Helpers::intersetVectors(vector<int> v1, vector<int> v2){
    vector<int> returnVector;
        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        std::set_symmetric_difference(
                v1.begin(), v1.end(),
                v2.begin(), v2.end(),
        std::back_inserter(returnVector));
        return returnVector;
}
