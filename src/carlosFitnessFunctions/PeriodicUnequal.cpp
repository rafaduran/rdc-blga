//
// C++ Implementation: PeriodicUnequal
//
// Description: 
//
//
// Author: rdc,,, rafadurancastaneda@gmail.com, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "PeriodicUnequal.h"

/* Constructor, solamente debe obtener el valor de la dimensión usada, que 
   determinará la precisión usada para representar cada individuo */
PeriodicUnequal::PeriodicUnequal(int dimension) {
  _dimension = dimension;
}

// Función usada para pasar de binario a real
double PeriodicUnequal::binaryToDouble(char *genes){
  int contador=0;

  for(int i = 0; i < _dimension; i++) {
    contador += genes[i] * pow((double)2,_dimension-1-i);
  }

  return contador/((pow((double)2, _dimension))-1);
}

// Función usada para el calculo del fitness
double PeriodicUnequal::fitness(char *genes) {
  char binary[_dimension];
  
  this->inverseGray(genes,binary);                
  double x = binaryToDouble(binary);

  return pow(E,((-2*log(2))*(pow((x-0.01)/0.8,2)))) *
         pow(sin(5*PI*(pow(x,0.75)-0.05)),6);
}

/* Función usada para comparar dos fitness, debe devolver > 0 si f1 es mayor,
   < 0 si f2 es mayor. En este caso valdrá simplemente con restar los valores de
   fitness. */
double PeriodicUnequal::compare(double f1, double f2) {
  return f1-f2;
}

/* Función usada para obtner el nombre de la función de fitness. Dicho nombre
   se usará para crear el archivo donde se guarden los resultados */
stringstream & PeriodicUnequal::getName() {
  _ss << "Periodic unequal";
  
  return _ss;
}

// Función usada para obtener la dimensión usada
int PeriodicUnequal::getDim() {
  return _dimension;
}

/* Función que calcula la distancia entre dos individuos dadas sus cadenas 
   binarias */
double PeriodicUnequal::distance(char *firstGen,char *secondGen) {
  char fbinary[_dimension],sbinary[_dimension];
  
  this->inverseGray(firstGen,fbinary);
  this->inverseGray(secondGen,sbinary);
  
  return abs(binaryToDouble(fbinary) - binaryToDouble(sbinary));
}

int PeriodicUnequal::getFunctionNumber(){

  return 1;
}
