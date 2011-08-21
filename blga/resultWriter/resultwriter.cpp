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

template <class T>
ResultWriter<T>* ResultWriter<T>::_rw = NULL;

template <class T>
const char* ResultWriter<T>::getFilename()
{
    return this->filename_;
}
template const char* ResultWriter<std::ofstream>::getFilename();

template <class T>
void ResultWriter<T>::setFilename(const char* filename)
{
    this->filename_ = filename;
    this->out_.open(filename, std::ios::app);
}
template void ResultWriter<std::ofstream>::setFilename(const char* filename);

template <class T>
void ResultWriter<T>::setNVariables(int nVar)
{
    this->nVariables_ = nVar;
}
template void ResultWriter<std::ofstream>::setNVariables(int nVar);

template <class T>
int ResultWriter<T>::getNVariables()
{
    return this->nVariables_;;
}
template int ResultWriter<std::ofstream>::getNVariables();

template <class T>
ResultWriter<T>* ResultWriter<T>::getResultWriter(int rwNumber, 
    const char* filename, int nVariables)
{
    if(_rw != NULL)
        delete _rw;
    
    switch(rwNumber){
    
        case 0:
            // Creating a BlgaJsonRW
            _rw = new BlgaJsonRW<T>(filename, nVariables);
            break;
    }
    return _rw;
}

template ResultWriter<std::ofstream>* ResultWriter<std::ofstream>::
    getResultWriter(int rwNumber, const char* filename, int nVariables);