//
// C++ Interface: PeriodicEqual
//
// Description: 
//
//
// Author: rdc,,, rafadurancastaneda@gmail.com, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef PERIODIC_EQUAL_H
#define PERIODIC_EQUAL_H

#include "FitnessFunction.h"
#include <cmath>

using namespace std;

#define PI 3.14159265358979323846

class PeriodicEqual:public FitnessFunction {
  public:
    PeriodicEqual(int dimension); // Constructor
  
    double fitness(char *genes); // Función que calcula el fitness

    double compare(double f1, double f2); // Función usada para comparar fitness

    stringstream &getName(); /* Función usada para obtener el nombre de la
                                función */
                                
    int getFunctionNumber();

    int getDim(); /* Función usada para obtener la dimensión de la cadena binaria 
                     de cada individuo */
    
    
    double distance(char *firstGen,char *secondGen);
      // Función usada para calcular la distancia real entre dos individuos
    
    double binaryToDouble(char *genes);
      // Función usada para realizar el paso de binario a real
};

#endif
