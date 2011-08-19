/*
    Writes results for Blga classes as JSON objects
    Copyright (C) 2011  Rafael Durán Castanñeda rafadurancastaneda@gmail.com 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cstring>
#include <iostream>
using namespace std;


#include "blgajsonrw.h"

BlgaJsonRW::BlgaJsonRW(bool tofile, const char* filename)
{
    this->filename_ = filename;
    if(tofile)
        this->redirect();
}

void BlgaJsonRW::start()
{
    cout << "[" << endl;
}


void BlgaJsonRW::write(double* variables, double fitness, bool is_last)
{        
    cout << "\t{\"fitness\": " << fitness << ", "; // Start 
    cout << "\"variable\": [";
    for(int j = 0; j < this->nVariables_; j++){
        
        cout << " {\"" << j << "\": " << variables[j] << "}";
        if( j != (this->nVariables_ - 1)){
            cout << "," ;
        }
    }
    cout << "]";
    // End
    if (is_last){
        cout << "}," << endl;
    } else {
        cout << "}" << endl;
    }
}

void BlgaJsonRW::end()
{
    cout << "]";
}

