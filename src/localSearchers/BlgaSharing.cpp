/***************************************************************************
 *   Copyright (C) 2008 by rdc   *
 *   rdc_120@hotmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <cmath>
#include <cstring> 

#include "BlgaSharing.h"

BlgaSharing::BlgaSharing(int itC, int popSize, int dimension, int alfa, 
                         int numMates, int pamNass, int rtsNass, 
                         FitnessFunction *ff, Random *random, double alpha, 
                         double shRadius)
  : Blga(itC, popSize, dimension, alfa, numMates,pamNass, rtsNass, ff, random) {

  this->_alpha = alpha;
  this->_shRadius = shRadius;
 
}

BlgaSharing::BlgaSharing(int itC, int popSize, int dimension, double probMux, 
                         int numMates, int pamNass, int rtsNass, 
                         FitnessFunction *ff, Random *random, double alpha,
                         double shRadius)
  : Blga(itC, popSize,dimension,probMux,numMates,pamNass, rtsNass, ff, random) {
  this->_alpha = alpha;
  this->_shRadius = shRadius;
}

void BlgaSharing::iterate() {
  double fCopy[popSize];
  
  for(int i = 0; i < popSize; i++) {
    fCopy[i] = fValues[i]; /* Guardamos los valores de fitness, evitando tener 
                              que reevaluar la población */
  }

  //SHARING
  this->sharing();

  //SELECCIÓN PAM
  char **mates = new char * [ numMates ];
  
  for ( int i = 0; i < numMates; i++ ) {
    int selected = this->pamSelection( this->pamNass, this->cl,
                                       this->dimension);
    mates[ i ] = this->population[ selected ];
  }
  
  //CRUCE MULTIPADRE UNIFORME
  this->crossMUX( this->probMUX, this->cl, mates,
                  numMates, this->offspring, this->mask, this->dimension );
  
  //EVALUACION DE OFFSPRING
  double fOffspring = this->ff->fitness( offspring);
  
  //ACTUALIZAMOS CL
  if ( ff->compare( fOffspring, fCL ) > 0 ) {
    memcpy( auxStr, offspring, sizeof( char ) * dimension );
    memcpy( offspring, cl, sizeof( char ) * dimension );
    memcpy( cl, auxStr, sizeof( char ) * dimension );
    double auxF = fOffspring;
    fOffspring = fCL;
    fCL = auxF;
    
    //RESETEAMOS LA MASCARA
    this->numProtected = 0;
    
    for ( int i = 0; i < dimension; i++ )
      this->mask[ i ] = 1;
  }
  
  // Aplicamos sharing a offspring
  double n = this->sh(offspring);
  fOffspring /= n;
  
  //REEMPLAZAMOS EL PEOR SEGUN EL FITNESS MODIFICADO
  int worst = this->searchWorst();
  
  if ( ff->compare( fOffspring, fValues[ worst ] ) > 0 ) {
    memcpy( population[ worst ], offspring, sizeof( char ) * dimension );
    fValues[worst] = fOffspring;
    fCopy[worst] = fOffspring;
  }
  
  // RECUPERAMOS LOS VALORES DE FITNESS   
  for(int i = 0; i < popSize; i ++) {
    fValues[i] = fCopy[i];
  }
  
  delete [] mates;
}

/* fastIterate se encargará de la realización de iteraciones rápidas, de forma
   que el sharing no es realizado */
void BlgaSharing::fastIterate() {
 
  //SELECCIÓN PAM
  char **mates = new char * [ numMates ];
  
  for ( int i = 0; i < numMates; i++ ) {
    int selected = this->pamSelection( this->pamNass, this->cl,
                                       this->dimension);//, mates, i );
    mates[ i ] = this->population[ selected ];
  }
  
  //CRUCE MULTIPADRE UNIFORME
  this->crossMUX( this->probMUX, this->cl, mates,
  numMates, this->offspring, this->mask, this->dimension );
  
  //EVALUACION DE OFFSPRING
  double fOffspring = this->ff->fitness( offspring); 
  
  //ACTUALIZAMOS CL
  if ( ff->compare( fOffspring, fCL ) > 0 ) {
    memcpy( auxStr, offspring, sizeof( char ) * dimension );
    memcpy( offspring, cl, sizeof( char ) * dimension );
    memcpy( cl, auxStr, sizeof( char ) * dimension );
    double auxF = fOffspring;
    fOffspring = fCL;
    fCL = auxF;
    
    //RESEATEAMOS LA MASCARA CUANDO CL CAMBIA
    this->numProtected = 0;
    
    for ( int i = 0; i < dimension; i++ )
      this->mask[ i ] = 1;
  }
  
  //REEMPLAZAMOS MEDIANTE RTS
  int nearest = this->pamSelection( this->rtsNass, this->offspring, 
                                    this->dimension );
  
  if ( ff->compare( fOffspring, fValues[ nearest ] ) > 0 ) {
    memcpy( population[ nearest ], offspring, sizeof( char ) * dimension );
    fValues[ nearest ] = fOffspring;
  }
   
  delete [] mates;
}

/* Hace el sharing a toda la población */
void BlgaSharing::sharing() {
  for(int i = 0; i < popSize; i++) {
    double n = this->sh(i);
    fValues[i] /= n;
  }
}

/* Función auxiliar para calcular el sharing */
double BlgaSharing::sh(int index) {
  double n = 0,distance;

  for(int i = 0; i < popSize; i++) {
    if(i != index) {
      distance = ff->distance(population[i],population[index]);
      
      if(distance  < _shRadius){
        n += 1 - (pow(distance/_shRadius,_alpha));
      }
    }
  }
  
  if(n == 0)
    n = 1;
  
  return n;
}

/* Función auxiliar para calcular el sharing */
double BlgaSharing::sh(char *genes) {
  double n = 0;

  for(int i = 0; i < popSize; i++) {
     
    if(double distance = ff->distance(population[i],genes) <= _shRadius){
      n += 1 - (pow(distance/_shRadius,_alpha));
    }
  }
  
  return n;
}

/* Busca el peor, según el fitness modificado */
int BlgaSharing::searchWorst() {
  int index = 0;
  
  for(int i = 1; i < this->popSize; i++) {
    if(ff->compare(fValues[index], fValues[i]) > 0) {
      index = i;
    }
  }
  
  return index;
}
