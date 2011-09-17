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


#ifndef BLGAJSONRW_H
#define BLGAJSONRW_H

#include "resultwriter.h"

template <class T>
class BlgaJsonRW : public ResultWriter<T>
{

public:
    BlgaJsonRW(const char* filename, int nVariables);
    ~BlgaJsonRW();
    void write(double* variables, double fitness, bool is_last = false);
    void startIteration(int iteration);
    void endIteration(bool is_last);
    void startRun(int run);
    void endRun(bool is_last);
    void writeParams(std::vector<Param> params);
};

#endif // BLGAJSONRW_H
