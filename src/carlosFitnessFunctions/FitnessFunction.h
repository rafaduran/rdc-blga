//
// C++ Interface: FitnessFunction
//
// Description: 
//
//
// Author:  <>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef __FITNESSFUNCTIONS_H__
#define __FITNESSFUNCTIONS_H__

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

/* Longtitud de las cadenas binarias usada, determina la precisión usada por
   por nuestras variables cuando pasamos a real */
#define P1_P2_dimension 30

using namespace std;

class FitnessFunction {
  static FitnessFunction* ff;
  
  public:
      // Calcula el fitness
      virtual double fitness(char *genes) = 0;
      
      //Devuelve un valor positivo si f1 es mejor que f2, negativo en otro caso
      virtual double compare(double f1, double f2) = 0;
   
      // Devuelve el nombre de la función
      virtual stringstream &getName() = 0;
        
      virtual int getFunctionNumber() = 0 ;

      // Devuelve la dimensión usada (longitud de las cadenas binarias)
      virtual int getDim() = 0;
      
      // Devuelve la distancia real  
      virtual double distance(char *first, char *second) {return 0;};
      
      // Paso de binario a real para más de una variable  
      virtual void binaryToDoubleVector(char *genes,double *vector) {return;};
      
      // Paso de binario a real                                  
      virtual double binaryToDouble(char *genes){ return 0;};
      
      /* Función estatica que permite la creación de todas las funciones de
         incluidas */
      static FitnessFunction* getFitnessFunction(int i, int numRun = 0);
      
      // Función usada para obtener el número de variables  
      virtual int getNvariables() {return 1;};
      
      // Paso de código gray a código binario con más de una variable 
      virtual void inverseGrayVector(char *gray, char *binary){return;};
      
      // Función usada para crear las funciones hump  
      static FitnessFunction* getHumpFunction(int numRun);
      
      // Paso de código gray a código binario  
      void inverseGray(char *gray, char *binary);
        
   protected:
      char *_binary;
      int _dimension;
      stringstream _ss;
};

extern int numEjecuciones;
extern int semillas[];

#endif
