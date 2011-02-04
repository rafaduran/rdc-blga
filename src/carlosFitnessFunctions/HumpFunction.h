//
// C++ Interface: HumpFunction
//
// Description: 
//
//
// Author: rdc,,, rafadurancastaneda@gmail.com, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef HUMP_FUNCTION_H
#define HUMP_FUNCTION_H

#include "FitnessFunction.h"
#include "Random.h"
#include <cmath>
#include <fstream>

using namespace std;

class HumpFunction:public FitnessFunction {
   private:
      void inverseGrayVector(char *gray, char *binary);
      void getPeaks(char *filename);
      // Función para leer un fichero de picos

      double _radius;   // Radio de los picos
      double _alpha;    // Constante usada para calcuar el fitness
      double _height;   // Altura de los picos
      int _nVariables;  // Número de variables
      int _kPeaks;      // Número de picos
      int _dimension;   // Dimensión de cada variable (número de bits)
      double **_peaks;  // Aqui mantenemos los picos
      //stringstream _ss; // Nombre devuelto
      //Random *_random;  // _random se utiliza para calculos aleatorios 
   public:
      HumpFunction(char *filename, int nvariables,int kpeaks, double
         radius,double alpha = 1.0, double height = 1.0, int dimension = 30);

      ~HumpFunction();
    
      double fitness(char *genes); // Calcula el fitness
      
      int searchClosestPeak(double *vector,double &distance);

      double compare(double f1, double f2); /* Compara dos fitness, devolviendo:
                                               > 0 si f1 es mayor
                                               0 si son iguales
                                               < 0 si f2 es mayor */

      stringstream &getName();   // Devuelve el nombre de la función

      int getDim();  /* Devuelve la dimensión total ( suma de la dimensión de
                        cada variable )*/
    
      double distance(char *firstPeak, char *secondPeak);
         // Distancia real de dos individuos
    
      double distance(double *firstPeak,double *secondPeak);
         // Distancia de dos individuos dado su valor real
    
      void binaryToDoubleVector(char *genes,double *vector);
         // Devuelve un vector con el valor real de cada una de las variables
    
      double BinaryToDouble(char *genes) {return 0;};
         /* Función que debe ser implementada en cada función de fitness
            pero que no es usada en esta función */
    
      int getNvariables(); // Devuelve el número de variables
};

#endif
