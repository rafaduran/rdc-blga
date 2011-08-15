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


#ifndef RESULTWRITER_H
#define RESULTWRITER_H

template <class T>
class ResultWriter
{

public:
    virtual ResultWriter(bool tofile, const char* filename);
    ~ResultWriter();
    virtual write(T population,double *fValues);
    void setNVariables(int nVar);
    int getNVariables();
    void setFilename(const char *filename);
    const char* getFilename();
    static ResultWriter<T>* getResultWriter(int rwNumber);
protected:
    const char *_filename;
    int _nVariables;
    void redirect();
private:
    static ResultWriter* rw;
};

#endif // RESULTWRITER_H
