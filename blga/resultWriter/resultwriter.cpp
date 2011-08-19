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
#include <cstdio>

using namespace std;

ResultWriter* ResultWriter::_rw = NULL;

ResultWriter::~ResultWriter()
{
    fclose(stdout);
}

const char* ResultWriter::getFilename()
{
    return this->filename_;
}

void ResultWriter::setFilename(const char* filename)
{
    this->filename_ = filename;
}

void ResultWriter::setNVariables(int nVar)
{
    this->nVariables_ = nVar;
}

int ResultWriter::getNVariables()
{
    return this->nVariables_;;
}

ResultWriter* ResultWriter::getResultWriter(int rwNumber, bool tofile,
    const char* filename)
{
    if(_rw != NULL)
        delete _rw;
    
    switch(rwNumber){
    
        case 0:
            // Creating a BlgaJsonRW
            _rw = new BlgaJsonRW(tofile, filename);
            break;
    }
    return _rw;
}

void ResultWriter::redirect()
{
    freopen (this->filename_,"w",stdout);
}






