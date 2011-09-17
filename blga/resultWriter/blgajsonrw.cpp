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
#include <vector>

#include "blgajsonrw.h"

using namespace std;

template <class T>
BlgaJsonRW<T>::BlgaJsonRW(const char* filename, int nVariables)
{
    this->nVariables_ = nVariables;
    this->filename_ = filename;
    this->out_.open(filename, std::ios::app);
    this->out_.setf(ios::scientific,ios::floatfield);
    this->out_.precision(15);
    this->out_ << "{" << endl;
}
template BlgaJsonRW<std::ofstream>::BlgaJsonRW(const char* filename, 
                                               int nVariables);

template <class T>
BlgaJsonRW<T>::~BlgaJsonRW()
{
    this->out_.close();
}


template <class T>
void BlgaJsonRW<T>::writeParams(vector<Param> params)
{
	vector<Param>::iterator it,before_last;
	this->out_ << "\t\"params\":{";
	before_last = params.end();
	before_last--;
	for(it=params.begin(); it< before_last; it++){
		this->out_ << "\"" << it->name << "\":" << it->value << ",";
	}
	this->out_ << "\"" << it->name << "\":" <<
			it->value << "}," << endl;
}


template <class T>
void BlgaJsonRW<T>::startRun(int run){
    this->out_ << "\t\""<< run << "\": {" << endl;
}

template <class T>
void BlgaJsonRW<T>::endRun(bool is_last=false){
    if(is_last){
        this->out_  << endl << "}" << endl;
    } else {
        this->out_<< "," << endl;
    }
    
}

template <class T>
void BlgaJsonRW<T>::startIteration(int iteration)
{
    this->out_ << "\t\t\""<< iteration << "\": [" << endl;
}

template <class T>
void BlgaJsonRW<T>::endIteration(bool is_last=false)
{
    this->out_ << "\t\t]";
    if(is_last){
        this->out_ << "}";
    } else {
        this->out_ << "," << endl;
    }
}

template <class T>
void BlgaJsonRW<T>::write(double* variables, double fitness, bool is_last)
{        
    this->out_ << "\t{\"fitness\": " << fitness << ", "; // Start 
    this->out_ << "\"variables\": {";
    for(int j = 0; j < this->nVariables_; j++){
        
        this->out_ << "\"" << j << "\": " << variables[j];
        if( j != (this->nVariables_ - 1)){
            this->out_ << "," ;
        }
    }
    this->out_ << "}";
    // End
    if (!is_last){
        this->out_ << "}," << endl;
    } else {
        this->out_ << "}" << endl;
    }
}
