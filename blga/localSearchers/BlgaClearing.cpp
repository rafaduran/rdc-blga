/***************************************************************************
 *   Copyright (C) 2008 by rdc                                             *
 *   rafadurancastaneda@gmail.com                                          *
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
#include <cstring>
#include "BlgaClearing.h"

BlgaClearing::BlgaClearing(int itC, int popSize, int dimension, int alfa, 
   int numMates, int pamNass, int rtsNass, FitnessFunction *ff, Random *random, 
   int k = 1, double clRadius = 0.1) 
   : Blga(itC,popSize,dimension,alfa,numMates,pamNass,rtsNass,ff,random){

   this->_k = k;
   this->_clRadius = clRadius;
   this->_cleared = new char[popSize];

   for(int i = 0; i < popSize; i++){
      _cleared[i] = 0; // Inicialmente ningún individuo esta cleared
   } 
}

BlgaClearing::BlgaClearing(int itC, int popSize, int dimension, double probMux, 
   int numMates, int pamNass, int rtsNass, FitnessFunction *ff, Random *random, 
   int k, double clRadius)
   : Blga(itC,popSize,dimension,probMux,numMates,pamNass,rtsNass,ff,random){
   
   this->_k = k;
   this->_clRadius = clRadius;
   this->_cleared = new char[popSize];

   for(int i = 0; i < popSize; i++){
      _cleared[i] = 0; // Inicialmente ningún individuo esta cleared
   }
}

BlgaClearing::~BlgaClearing() {
   delete [] _cleared;
}
 
void BlgaClearing::iterate( ) {
   fCopy = new double[popSize];
  
   for(int i = 0; i < popSize; i++) {
      fCopy[i] = fValues[i]; /* Guardamos los valores de fitness, evitando
                                tener que reevaluar la población */
   }
  
   // CLEARING  
   this->clearing();
  
   //SELECCIÓN PAM, MODIFICADA PARA OBTENER INDIVIDUOS NO CLEARED
   char **mates = new char * [ numMates ];
  
   for ( int i = 0; i < numMates; i++ ) {
      int selected = this->pamSelectionCl( this->pamNass, this->cl,
                                           this->dimension,0);
      mates[ i ] = this->population[ selected ];
   }
   //CRUCE MULTIPADRE UNIFORME
   this->crossMUX( this->probMUX, this->cl, mates,
                   numMates, this->offspring, this->mask, this->dimension );

   //EVALUACION OFFSPRING
   double fOffspring = this->ff->fitness( offspring);
  
   //ACTUALIZACIÓN CL
   if ( ff->compare( fOffspring, fCL ) > 0 ) {
      memcpy( auxStr, offspring, sizeof( char ) * dimension );
      memcpy( offspring, cl, sizeof( char ) * dimension );
      memcpy( cl, auxStr, sizeof( char ) * dimension );
      double auxF = fOffspring;
      fOffspring = fCL;
      fCL = auxF;
    
      //RESETEAMOS LA MASCARA CUANDO CL CAMBIA
      this->numProtected = 0;
    
      for ( int i = 0; i < dimension; i++ )
         this->mask[ i ] = 1;
   }
   
   //REEMPLAZAMOS EL PEOR CLEARED
   int aux = searchWorstCl();
   
   if(ff->compare(fOffspring,fCopy[aux]) > 0 ) {
      memcpy( population[aux], offspring, sizeof( char ) * dimension );
      fValues[aux] = fOffspring;
      fCopy[aux] = fOffspring;
      _cleared[aux] = 0;   // Ya no esta cleared
   }
   
   // RECUPERAMOS LOS VALORES DE FITNESS 
   for(int i = 0; i < popSize; i ++) {
      fValues[i] = fCopy[i];
   }
  
   delete [] mates;
   delete [] fCopy;
}

/* fastIterate se encargará de la realización de iteraciones rápidas, de forma
   que el clearing no es realizado, pero dado que con _cleared mantenemos
   información sobre los inidividuos cleared, usaremos dicha información */
void BlgaClearing::fastIterate( ) {
 
   //SELECCIÓN PAM, MODIFICADA PARA OBTENER INDIVIDUOS NO CLEARED
   char **mates = new char * [ numMates ];
  
   for ( int i = 0; i < numMates; i++ ) {
      int selected = this->pamSelectionCl( this->pamNass, this->cl,
                                           this->dimension,0);
      mates[ i ] = this->population[ selected ];
   }
  
   //CRUCE MULTIPADRE UNIFORME
   this->crossMUX( this->probMUX, this->cl, mates,
                   numMates, this->offspring, this->mask, this->dimension );
  
   //EVALUACION OFFSPRING
   double fOffspring = this->ff->fitness( offspring);
  
   //ACTUALIZACIÓN CL
    if ( ff->compare( fOffspring, fCL ) > 0 ) {
      memcpy( auxStr, offspring, sizeof( char ) * dimension );
      memcpy( offspring, cl, sizeof( char ) * dimension );
      memcpy( cl, auxStr, sizeof( char ) * dimension );
      double auxF = fOffspring;
      fOffspring = fCL;
      fCL = auxF;
    
      //RESETEAMOS LA MASCARA CUANDO CL CAMBIA
      this->numProtected = 0;
    
      for ( int i = 0; i < dimension; i++ )
         this->mask[ i ] = 1;
   }
  
   /* INSERTAMOS EN LA POBLACIÓN MEDIANTE RTS, MODIFICADO PARA REEMPLAZAR
      SOLO INDIVIDUOS CLEARED */
   int nearest = this->pamSelectionCl( this->rtsNass, this->offspring,
                                       this->dimension, 1 );
  
   if ( ff->compare( fOffspring, fValues[ nearest ] ) > 0 ) {
      memcpy( population[ nearest ], offspring, sizeof( char ) * dimension );
      fValues[ nearest ] = fOffspring;
      _cleared[nearest] = 0;  // Ya no esta cleared
    
   }
   delete [] mates;
}
/* PAM modificado para que obtenga sólo individuos cleared o sólo individuos no
   cleared, según el valor de cleared: 1 sólo cleared
                                       0 sólo no cleared */
int BlgaClearing::pamSelectionCl( int nass, char *cl, int dimension,
   char cleared, char** selected, int numSelected ) {
   int * indexes = new int[ nass ];
   int minDistance;
   int indexMin;
   
   /* PARA BUSCAR INDIVIDUOS CLEARED/NO CLEARED RECORREREMOS ALEATORIAMENTE
      LA POBLACIÓN, PARA ELLO USAMOS vector */
   int *vector = new int[popSize];
   
   for(int i = 0; i < popSize; i++)
      vector[i] = i;
    
   vector = random->RandPerm(vector,popSize);
 
   int aux = 0;
   int cont = 0;
  
   for ( int i = 0; i < nass ; i++ ) {
      for(int j = aux; j < popSize; j++)
         if((fValues[vector[j]] && !cleared) ||
            (_cleared[vector[j]] && cleared)) { 
            indexes[i] = vector[j];
            aux = j + 1;
            cont++;
            break;
         }  
    
   }
   
   /* Si no hay suficientes que cumplan la condición de cleared el resto se
      escogerá aleatoriamente */
   if(cont < nass)
      for(int i = cont; i < nass; i++)
         indexes[i] = random->Randint(0,popSize - 1);

   /* UNA VEZ QUE TENEMOS LOS INDIVIDUOS A COMPARAR QUE CUMPLAN LA CONDICION DE 
      CLEARED EL RESTO DEL PROCESO ES IDENTICO QUE PAM */
   int k = indexes[ 0 ];
   indexMin = indexes[ 0 ];
   minDistance = distance( cl, population[ k ], dimension );
  
   for (int j = 0; j < numSelected; j++){
      int newDistance2 = distance( selected[j], population[k], dimension, 
                                   minDistance);
    
      if (newDistance2 < minDistance)
      minDistance = newDistance2;
   }
  
   for ( int i = 1; i < nass; i++ ) {
      k = indexes[ i ];
      int newDistance = distance( cl, population[ k ], dimension, minDistance);
    
      for (int j = 0; j < numSelected; j++){
         int newDistance2 = distance( selected[j], population[k], dimension, 
                                      minDistance);
      
         if (newDistance2 < newDistance)
         newDistance = newDistance2;
      }
    
      if ( newDistance < minDistance ) {
         minDistance = newDistance;
         indexMin = k;
      }
   }
  
   delete [] indexes;
   delete [] vector;
   return indexMin;
}

void BlgaClearing::clearing() {
   int vector[popSize];  /*No ordenamos la población en si, sino que utilizamos
                          vector para mantener el orden */
   double distance;
   int cont;
  
   for(int i = 0; i < popSize; i++) {
      vector[i] = i;
   }
  
   // ORDENAMOS
   this->quickSort(vector,0,popSize);
   
   /* HACEMOS CLEARING, para ello recorremos la población según el fitness de 
      forma decreciente, utilizando vector. Cuando encontramos un elemento que
      todavía no ha sido cleared (fValues != 0) tenemos un nuevo nicho,
      buscamos el resto de individuos en ese nicho, manteniendo no cleared k
      individuos y cleared el resto */  
   for(int i = 0; i < popSize; i++) {
      if(fValues[vector[i]] != 0) {
         cont = 1;
         for(int j = i + 1; j < popSize; j++) {
            if(ff->distance(population[vector[i]], population[vector[j]]) <= 
               _clRadius) {
               if(cont < _k) {   // No hacemos nada hasta obtener k individuos
                  cont++;  
               } else {          /* Cuando tenemos k, el resto de individuos
                                    dentro del radio de clearing son cleared */
                  fValues[vector[j]] = 0;
                  _cleared[vector[j]] = 1;
               }
            }
         }
      }
   }
}

/* Búsqueda el peor, con la condición de que el individuo este cleared, por
   tanto devuelve el peor cleared */
int BlgaClearing::searchWorstCl(){
   int index = 0;
   
   // Buscamos el primer cleared  
   for(int i = 0; i < popSize; i++){
      if(fValues[i]==0){
         index = i;
         break;
      }
   }
   
   // Comprobamos si hay uno peor
   for(int i = index + 1; i < popSize; i++){
      if(fCopy[i] < fCopy[index] && fValues[i] == 0 )
         index = i;
   }
  
  return index;
}
