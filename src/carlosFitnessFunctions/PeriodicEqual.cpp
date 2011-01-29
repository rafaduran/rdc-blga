//
// C++ Implementation: PeriodicEqual
//
// Description: 
//
//
// Author: rdc,,, <rdc_120@hotmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "PeriodicEqual.h"

/* Constructor, solamente debe obtener el valor de la dimensión usada, que 
   determinará la precisión usada para representar cada individuo */
PeriodicEqual::PeriodicEqual(int dimension) {
  _dimension = dimension;
}

// Función usada para pasar de binario a real
double PeriodicEqual::binaryToDouble(char *genes){
  int contador=0;

  for(int i = 0; i < _dimension; i++) {
    contador += genes[i] * pow((double)2,_dimension-1-i);
  }

  return contador/((pow((double)2, _dimension))-1);
}

// Función usada para el calculo del fitness
double PeriodicEqual::fitness(char *genes) {
  char binary[_dimension];
  
  this->inverseGray(genes,binary);
  
  double x = binaryToDouble(binary);

  return pow((sin (5*PI*x)),6);
}

/* Función usada para comparar dos fitness, debe devolver > 0 si f1 es mayor,
   < 0 si f2 es mayor. En este caso valdrá simplemente con restar los valores de
   fitness. */
double PeriodicEqual::compare(double f1, double f2) {
  return f1-f2;
}

/* Función usada para obtner el nombre de la función de fitness. Dicho nombre
   se usará para crear el archivo donde se guarden los resultados */
stringstream & PeriodicEqual::getName() {
  _ss << "Periodic equal";
  return this->_ss;
}

// Función usada para obtener la dimensión usada
int PeriodicEqual::getDim() {
  return _dimension;
}

/* Función que calcula la distancia entre dos individuos dadas sus cadenas 
   binarias */
double PeriodicEqual::distance(char *firstGen,char *secondGen) {
  char fbinary[_dimension],sbinary[_dimension];
  
  this->inverseGray(firstGen,fbinary);
  this->inverseGray(secondGen,sbinary);

  return abs(binaryToDouble(fbinary) - binaryToDouble(sbinary));
}

