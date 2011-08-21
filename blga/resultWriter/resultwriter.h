/*
    Writes results for LocalSearcher classes
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
#include <iostream>
#include <fstream>
#include "blgajsonrw.h"

#ifndef RESULTWRITER_H
#define RESULTWRITER_H

template <class T>
class ResultWriter
{

public:
    ResultWriter(){return;};
    virtual ~ResultWriter();
    virtual void start(int iteration){return;};
    virtual void write(double* variables,double fitness, bool is_last=false)
        {return;};
    virtual void end(bool is_last){return;};
    void setNVariables(int nVar);
    int getNVariables();
    void setFilename(const char *filename);
    const char* getFilename();
    static ResultWriter<T>* getResultWriter(int rwNumber, const char* filename, 
        int nVariables);
protected:
    const char *filename_;
    int nVariables_;
    T out_;
private:
    static ResultWriter<T>* _rw;
};

#endif // RESULTWRITER_H
