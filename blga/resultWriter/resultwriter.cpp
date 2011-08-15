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

#include "resultwriter.h"

ResultWriter::~ResultWriter()
{
    fclose(stdout);
}

const char* ResultWriter::getFilename()
{
    return this->_filename;
}

void ResultWriter::setFilename(const char* filename)
{
    this->_filename = filename;
}

void ResultWriter::setNVariables(int nVar)
{
    this->_nVariables = nVar;
}

int ResultWriter::getNVariables()
{
    return this->_nVariables;
}

ResulWriter<T>* ResultWriter::getResultWriter(int rwNumber)
{
    if(rw != NULL)
        delete rw;
    
    switch(rwNumber){
    
        case 0:
            // Creating a ResultWriter
            rw = new ResultWriter<char**>(true, "test.txt");
            break;
    }
    return rw;
}

void ResultWriter::redirect()
{
    freopen (this->_filename,"w",stdout);
}






