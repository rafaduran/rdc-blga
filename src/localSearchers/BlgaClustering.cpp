/***************************************************************************
 *   Copyright (C) 2008 by rdc   *
 *   rafadurancastaneda@gmail.com   *
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

#include "BlgaClustering.h"
 
BlgaClustering::BlgaClustering(int itC, int popSize, int dimension, double probMux, 
                               int numMates, int pamNass, int rtsNass,
                               FitnessFunction *ff, Random *random, 
                               int nCentroids, double dmin, double dmax,
                               double alpha)
  : Blga(itC, popSize,dimension,probMux,numMates,pamNass, rtsNass, ff, random) {
  
   this->_nCentroids = nCentroids;
   _nCaux = nCentroids;
   this->_dmin = dmin;
   this->_dmax = dmax;
   this->_alpha = alpha;
  
   _cluster = new int[popSize];

}

BlgaClustering::BlgaClustering(int itC, int popSize, int dimension, int alfa, 
                               int numMates, int pamNass, int rtsNass,
                               FitnessFunction *ff, Random *random, 
                               int nCentroids, double dmin, double dmax,
                               double alpha)
  : Blga(itC, popSize,dimension,alfa,numMates,pamNass, rtsNass, ff, random) {
  
  this->_nCentroids = nCentroids;
  _nCaux = nCentroids;
  this->_dmin = dmin;
  this->_dmax = dmax;
  this->_alpha = alpha;
  
  _cluster = new int[popSize];
}

BlgaClustering::~BlgaClustering() {
  delete[] _cluster;
}

void BlgaClustering::iterate( ) {
   double fCopy[popSize];
   _nCentroids = _nCaux; // Así mantenemos el valor recibido inicialmente
  
   for(int i = 0; i < popSize; i++) {
      fCopy[i] = fValues[i]; /* Guardamos los valores de fitness, evitando
                                tener que reevaluar la población */
   }
   
   // REINICAMOS LOS CLUSTERS EN CADA ITERACION  
   for(int i = 0; i < popSize; i++) 
      _cluster[i] = -1; 
    
   _centroids.clear();
   
   // CREAMOS LOS CLUSTERS
   cout << "populate" << endl;
   this->populateCentroids();
   
   // CALCULAMOS EL FIINESS MODIFICADO
   cout << "clustering" << endl;
   this->clustering();

   // SELECCION, no hacemos PAM, sino que escogemos sólo centroides
   cout << "selección" << endl;
   char **mates = new char * [ _nCentroids ];
  
   if(_nCentroids < numMates)
      numMates = _nCentroids;
   // El orden en que forman parte de mates es aleatorio, por eso usamos vector 
   int *vector = new int[numMates];
   for(int i = 0; i < numMates; i++)
      vector[i] = i;
    
   vector = random->RandPerm(vector,numMates);
    
   for(int i = 0; i < numMates; i++){
      mates[i] = population[_centroids[vector[i]]];
   }
  
   //CRUCE  MULTIPADRE UNIFORME
   cout << "cruce" << endl;
   this->crossMUX( this->probMUX, this->cl, mates, numMates,
                   this->offspring, this->mask, this->dimension );
  
   //EVALUACION DE OFFSPRING
   double fOffspring = this->ff->fitness(offspring);
  
  
   //ACTUALIZACION DE CL
   if ( ff->compare( fOffspring, fCL ) > 0 ) {
      memcpy( auxStr, offspring, sizeof( char ) * dimension );
      memcpy( offspring, cl, sizeof( char ) * dimension );
      memcpy( cl, auxStr, sizeof( char ) * dimension );
      double auxF = fOffspring;
      fOffspring = fCL;
      fCL = auxF;
    
      //RESETEAMOS LA MASCARA CUANDO CAMBIA CL
      this->numProtected = 0;
    
      for ( int i = 0; i < dimension; i++ )
         this->mask[ i ] = 1;
   }
  
   // Aplicamos clustering a offspring, para poder comparar fitness modificados
   cout << "clustering offspring" << endl;
   this->clustering(offspring,fOffspring);
  
   // REEMPLAZAMOS EL PEOR SEGÚN EL FITNESS MODIFICADO
   cout << "busqueda peor" << endl;
   int worst = this->searchWorstClus();
    
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
   delete []vector;
   cout << "fin" << endl;
}

/* fastIterate se encargará de la realización de iteraciones rápidas, de forma
   que el clustering no es realizado, pero como mantenemos información sobre los
   clusters formados, usaremos dicha información */
void BlgaClustering::fastIterate( ) {

   // SELECCION, no hacemos PAM, sino que escogemos sólo centroides
   char **mates = new char * [ _nCentroids ];
  
   if(_nCentroids < numMates)
      numMates = _nCentroids;
   // El orden en que forman parte de mates es aleatorio, por eso usamos vector 
   int *vector = new int[numMates];
   for(int i = 0; i < numMates; i++)
      vector[i] = i;
    
   vector = random->RandPerm(vector,numMates);
    
   for(int i = 0; i < numMates; i++){
      mates[i] = population[_centroids[vector[i]]];
   }
    
   //CRUCE MULTIPADRE UNIFORME
   this->crossMUX( this->probMUX, this->cl, mates, numMates,
                   this->offspring, this->mask, this->dimension );
  
   //EVALUACION OFFSPRING
   double fOffspring = this->ff->fitness(offspring);
  
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

   // REEMPLAZAMOS EL PEOR
   int worst = this->searchWorstClus();
  
   if ( ff->compare( fOffspring, fValues[ worst ] ) > 0 ) {
      memcpy( population[ worst ], offspring, sizeof( char ) * dimension );
      fValues[ worst ] = fOffspring;
   }
  
   delete [] mates;
}

/* Busca todos los centroides y forma los clusters */
void BlgaClustering::populateCentroids() {
  

   // Ordeno de mayor a menor un vector según el fitness
   int vector[popSize];
  
   for(int i = 0; i < popSize; i++) {
      vector[i] = i;
   }
  
   this->quickSort(vector,0,popSize);
  
   /* Inserto en _centroids el elemento con mayor fitness, ya que será el primer
      centroide */
   _centroids.insert(_centroids.end(),vector[0]);
   _cluster[vector[0]] = -2; // Ya que es su propio centroide
  
   int cont = 0;
   int Ok = 1;
   double daux;
  
   // Busco el resto de centroides
 
   for(int i = 1; cont < _nCentroids && i < popSize; i++) {
      for(int j = 0; j < _centroids.size(); j++){
         daux = ff->distance(population[vector[i]],population[_centroids[j]]);
         if(daux < _dmin)
            Ok = 0;    
      }
    
      if(Ok == 1){
         _centroids.insert(_centroids.end(),vector[i]);
         _cluster[vector[i]] = -2;
         cont ++;
      }
    
      Ok = 1;
   }
  
   _nCentroids = _centroids.size();
  
   // BUSCAMOS EL CLUSTER DE CADA INDIVIDUO
   for(int i = 0; i < popSize; i++) {
      if( _cluster[i] != -2) {
         int nearest = searchCentroid(population[i]);
      
         if(nearest == -1) {  /* Si un individuo no pertenece a ninguno de los
                                 clusters, se creará un nuevo cluster */
            _cluster[i] = -2;
            _centroids.insert(_centroids.end(),i);
         } else {
            _cluster[i] = nearest;
         }
      }
   }
  
   /* Si creo nuevos clusters debo comprobar si algún individuo pertenece a los
      clusters */
   if(_nCentroids < _centroids.size()){
      for(int i = 0; i < popSize; i++) {
         if(_cluster[i]!= -2) {
          for(int j = _nCentroids ; j < _centroids.size(); j++){
            double d1 = ff->distance(population[i],population[_cluster[i]]);
            double d2 = ff->distance(population[i],population[_centroids[j]]);
              if(d1 > d2) {
                _cluster[i] = _centroids[j];
	           }
	         }
         }
      }
   }
}

/* Búsca el centroide más cercano para un individuo, si la distancia al más
   cercano es mayor que dmax, devuelve -1 */
int BlgaClustering::searchCentroid(int index){
  
   int nearest = 0;
   double aux,dmin = ff->distance(population[index],population[_centroids[0]]);
  
   for(int i = 1; i < _centroids.size(); i++) {
      aux = ff->distance(population[index],population[_centroids[i]]);
      if(aux < dmin) {
         nearest = _centroids[i];
         dmin = aux;
      }
   }
  
   if(dmin > _dmax)
      nearest = -1;
    
   return nearest;
}

/* Búsca el centroide más cercano para un individuo, si la distancia al más
   cercano es mayor que dmax, devuelve -1 */

int BlgaClustering::searchCentroid(char *gen){
  
   int nearest = 0;
   double aux,dmin = ff->distance(gen,population[_centroids[0]]);
  
   for(int i = 1; i < _centroids.size(); i++) {
      aux = ff->distance(gen,population[_centroids[i]]);
      if(aux < dmin) {
         nearest = _centroids[i];
         dmin = aux;
      }
   }
  
   if(dmin > _dmax)
      nearest = -1;
    
   return nearest;
}

/* Calcula el fitness modificado para toda la población */
void BlgaClustering::clustering() {
   for(int i = 0; i < popSize; i++) {
      int n = 1; // Para contar el centroide
      if(_cluster[i]!=-2){
         //n++;  // Debemos contar el centroide
         for(int j = 0; j < popSize; j++) {
            if(_cluster[i] == _cluster[j] )
               n++;
         }
      
         double distance = ff->distance(population[i],
                                        population[_cluster[i]]);
         fValues[i] /= n*(1-pow(distance /(2*_dmax),_alpha));
      } else {
         //n++; // Para contar el centroide 
         for(int j = 0; j < popSize; j++) {
            if(_cluster[j]== i)
               n++;
         }
         fValues[i] = fValues[i] / n;
      }
   }
}

/* Calcula el fitness modificado para un individuo */
void BlgaClustering::clustering(char *gen,double &fitness){
   int n = 1; 

   int cluster = searchCentroid(gen);
  
   if(cluster != -1){
      for(int j = 0; j < popSize; j++) {
         if(cluster == _cluster[j])
            n++;
      }
  
      fitness /= n*(1-pow(ff->distance(gen, population[cluster])
                / (2*_dmax),_alpha));
   }
}

/* Búsqueda el peor, evitando seleccionar centroides */
int BlgaClustering::searchWorstClus() {
  
   int index = 0;
  
   for(int i = 1; i < this->popSize; i++) {
      if(ff->compare(fValues[index], fValues[i]) > 0 && _cluster[i] != -2) {
         index = i;
      } 
   }  
   return index;
}
