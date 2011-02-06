/*
    ClusteredClearing
    Copyright (C) 2011  rafadurancastaenda@gmail.com

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

#include "ClusteredClearing.h"
#include <string.h>
#include <stdlib.h>

ClusteredClearing::ClusteredClearing(int nOff, int popSize, int dimension, int
  alfa, int numMates, int pamNass, int rtsNass, FitnessFunction *ff, Random
  *random, double clRadius = 0.1) : Blga(itC, popSize, dimension, alfa,
  numMates, pamNass, rtsNass, ff, random){

   this->_clRadius = clRadius;
  
   _cluster = new int[popSize];
   
   this->writeResults(2, 0, 2, "pinicial");
}

ClusteredClearing::ClusteredClearing(int nOff, int popSize, int dimension,
  double probMux, int numMates, int pamNass, int rtsNass, FitnessFunction *ff,
  Random *random, double clRadius) : Blga(itC, popSize, dimension, probMux,
  numMates, pamNass, rtsNass, ff, random){
   
   this->_clRadius = clRadius;
   
   _cluster = new int[popSize];
}

ClusteredClearing::~ClusteredClearing(){
  _cs.clear();
}

int ClusteredClearing::improve(char* s, double& fitness, int size, 
   int maxEvaluations, int iRuns, int current_nFEs, int fNumber, const char* 
   name) {
   
   int numEvaluations = 0;
   this->dimension = size;
   int cont = 0;
        
   while ( numEvaluations < maxEvaluations) {
      
      this->iterate();
      
      numEvaluations += _numEval;
      cont ++;
      
      this->writeResults(iRuns, numEvaluations, fNumber, name);
      cout << "Número de eval en iteración " << cont << ":" << _numEval << endl;
   }
  
   fitness = this->fCL; // Guardamos el fitness obtenido
   return numEvaluations;  // Devolvemos el número de evaluaciones realizadas
}

bool ClusteredClearing::hasConverged( ) {
   return false;
}

void ClusteredClearing::crossMUX ( double probM, Cluster &clus, char** mates,
   int numMates, char* off, int size ) {
   
   int numDiferences = 0;
   string oldMask (clus.mask);
   //memcpy( oldMask, clus.mask, size * sizeof( char ) );
   memcpy( off, population[clus.cIndex], size * sizeof( char ) );
      
   int *perm = new int[ size ];
   random->RandPerm( perm, size );
      
   if ( !repulse ) { // probM -> chance to get bits from mates
    
      for ( int i = 0; i < size; i++ ) {
      
         int cIndex = perm[ i ];
         
         // oldMask = 0 bit protected -> do nothing
         if ( oldMask.compare(cIndex,1,"1") == 0 ) {
            double probs1 = 0;
            double probChange = 0;
   
            for ( int j = 0; j < numMates; j++ ) {
               probs1 += mates[ j ][ cIndex ];
            }
   
            probs1 = probs1 / numMates;
            
            if ( off[ cIndex ] == 1 )
               probChange = 1.0 - probs1;
            else
               probChange = probs1;
            
            // If no change -> flip random unprotected bit
            if ( random->Rand() < ( probChange * probM ) ) {
               off[ cIndex ] = 1 - off[ cIndex ];
               numDiferences++;
               clus.numProtected++;
               clus.mask.replace(cIndex,1,"0");
            }
         }
      }
   } else { // Repulse -> probM = chance to not get bits from mates
    
      for ( int i = 0; i < size; i++ ) {
      
         int cIndex = perm[ i ];
      
         if ( oldMask.compare(cIndex,1,"1") == 0 ) {
   
            double probs1 = 0;
   
            double probChange = 0;
   
            for ( int j = 0; j < numMates; j++ ) {
               probs1 += mates[ j ][ cIndex ];

            }
   
            probs1 = probs1 / numMates;
   
            if ( off[ cIndex ] == 0 ) // repulsa
               probChange = 1.0 - probs1;
            else
               probChange = probs1;
   
            if ( random->Rand() < ( probChange * probM ) ) {
               off[ cIndex ] = 1 - off[ cIndex ];
               numDiferences++;
               clus.numProtected++;
               clus.mask.replace(cIndex,1,"0");
            }
         }
      }
   }
   
   // Si tras recorrer offspring no cambiamos ningún bit, cambiamos uno 
   //  aleatoriamente que no este protegido
   if ( numDiferences == 0 ) {
    
      for ( int i = 0; i < size; i++ ) {
      
         if ( oldMask.compare(perm[i],1,"1") == 0) {
            off[ perm[ i ] ] = 1 - off[ perm[ i ] ];
            numDiferences++;
            clus.numProtected++;
            clus.mask.replace(perm[i],1,"0");
            break;
         }
      }
   }
  
   delete [] perm;
   //delete [] oldMask;
}

void ClusteredClearing::clearing() {

   int vector[popSize];  /*No ordenamos la población en si, sino que utilizamos
                          vector para mantener el orden */
  
   for(int i = 0; i < popSize; i++) {
      vector[i] = i;
   }
  
   // Sort population by descending fitness
   this->quickSort(vector,0,popSize);

   for(int i = 0; i < popSize; i++) {
      if(_cluster[vector[i]] == -1) {
         
         // Creo un nuevo cluster
         _cluster[vector[i]] = -2;
         
         Cluster dummy;
         dummy.cIndex = vector[i];
         dummy.numProtected = 0;
         dummy.size = 1;
         for(int j = 0; j < dimension; j++){
            dummy.mask.append("1");
         }
                 
         _cs.push_back(dummy);
         
         // All individuals in _clRadius are added to new cluster
         for(int j = i + 1; j < popSize; j++) {
            if(_cluster[vector[j]]== -1) {
               if(ff->distance(population[vector[i]], population[vector[j]]) <= 
                  _clRadius) {
               
                  _cluster[vector[j]] = _cs.size()-1;
                  _cs.back().size++;            
               }
            }
         }
      }
   }
}

void ClusteredClearing::iterate( ) {
   
   for(int i = 0; i < popSize; i++)
      _cluster[i] = -1;
      
   clearing();
   
   cout << "Número de clusters:" << _cs.size() << endl;
   
   // Offspring generation
   for(int i = 0; i < _cs.size(); i++){
      while(_cs[i].numProtected < dimension) {
         Off newOff;
         
         newOff.genotype = new char[dimension];        

         char **mates = new char * [ numMates ];
         
         // PAM Selection
         for ( int k = 0; k < numMates; k++ ) {
            int selected = this->pamSelection( this->pamNass,
               population[_cs[i].cIndex], this->dimension);
            mates[ k ] = this->population[ selected ];
         }
         
         // MULTIPARENT UNIFORM CROSSOVER         
         this->crossMUX( this->probMUX, _cs[i], mates, numMates,
            newOff.genotype, this->dimension );
                    
         newOff.fitness = ff->fitness(newOff.genotype);
        
         _offsprings.push_back(newOff);
         
         delete [] mates;
      }
   }
   
   // Offsrping replacement
   clusteredClearingReplacement();
  
   _numEval = _offsprings.size();
   
   for(int i = 0; i < _offsprings.size(); i++){
      delete _offsprings[i].genotype;
   }
   
   _cs.clear();
   _offsprings.clear();
}

void ClusteredClearing::clusteredClearingReplacement() {
   for(int i = 0; i < _offsprings.size(); i++){
      int index = searchReplaced();
      
      int offCluster = searchCluster(_offsprings[i].genotype);
          
      
      if((_cluster[index]!=-2)  && (_cluster[index] == offCluster) &&
         (fValues[index] < _offsprings[i].fitness)){
        // cout << " Not centroid, same cluster " << endl;
         //cout << index << endl;
         
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         fValues[index] = _offsprings[i].fitness;
         //cout << "Sale" << endl;
      } else if((_cluster[index]!=-2) && (offCluster != -1)){
         //cout << " Not centroid, different existent cluster " << endl;
         _cs[offCluster].size++;
         _cs[_cluster[index]].size--;
         _cluster[index] = offCluster;
         //cout << index << endl;
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         fValues[index] = _offsprings[i].fitness;
      } else if((_cluster[index]!=-2) && (-1 == offCluster)) {
         //cout << " Not centroid, different new cluster " << endl;
         _cs[_cluster[index]].size--;
         //cout << index << endl;
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         //cout << "sale if" << endl;
         fValues[index]=_offsprings[i].fitness;
         
         _cluster[index] = -2;
         
         Cluster dummy;
         dummy.cIndex = index;
         dummy.size = 1;
         _cs.push_back(dummy);
                  
      } else if((_cluster[index] == -2) && (offCluster == -1)) {
         //cout << " Centroid, different new cluster << endl;
         int j = 0;
         while((_cs[j].size == 0) || (_cs[j].cIndex != index))
           j++;
         _cs[j].size = 0;
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         fValues[index] = _offsprings[i].fitness;
         Cluster dummy;
         dummy.cIndex = index;
         dummy.size = 1;
         _cs.push_back(dummy);
      } else if((_cluster[index]==-2)  && (index == _cs[offCluster].cIndex) &&
         (fValues[index] < _offsprings[i].fitness)) {
         //cout << "Centroid same cluster -> just replace" << endl;
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         fValues[index] = _offsprings[i].fitness;
      } else if((_cluster[index]==-2) && (index != _cs[offCluster].cIndex)) {
         //cout << " Centroid different existent cluster " <<
         //_cs[offCluster].cIndex << " " << _cluster[index] <<endl;
         memcpy( population[index], _offsprings[i].genotype, dimension *
            sizeof(char));
         fValues[index] = _offsprings[i].fitness;
         _cluster[index] = offCluster;
          _cs[offCluster].size++;
         int j = 0;
         while((_cs[j].size == 0) || (_cs[j].cIndex != index))
            j++;
         _cs[j].size = 0;
      }
   }
}
int ClusteredClearing::searchCluster(char *gen){
   double dis; 
   int index, cont = 0;
   
   while(_cs[cont].size == 0)
      cont ++;
   
   index = cont;
   dis = ff->distance(gen, population[_cs[cont].cIndex]);
   
   
   while((dis > _clRadius) && (cont < _cs.size())){
      if(_cs[cont].size > 0){
         index = cont;
         dis = ff->distance(gen, population[_cs[cont].cIndex]);
      }
      cont++;
   }
   
   if(dis > _clRadius)
      return -1;
   else
      return index;
}

int ClusteredClearing::searchReplaced() {
   double newRV,rValueMin; 
   int cluster, cont = 0;
   
   while(_cs[cont].size == 0)
      cont++;
   
   cluster = cont;
   rValueMin = fValues[_cs[cont].cIndex]/(_cs[cont].size * _cs[cont].size);
  
   // Look for cluster to insert
   for(int i = cont + 1; i < _cs.size(); i++){
      if(_cs[i].size != 0){
         newRV = fValues[_cs[i].cIndex] / (_cs[i].size * _cs[i].size);
         if(rValueMin > newRV){
            rValueMin = newRV;
            cluster = i;
         }
      }
   }
 
   // Look for individual to replace
   if(_cs[cluster].size == 1){
      return _cs[cluster].cIndex;
   } else {
      return searchWorst(cluster);
   }
}


// Look for worst individual from specific cluster
int ClusteredClearing::searchWorst(int cluster) {
   int i = 0, indexMin;
   double fMin;
   
   while(_cluster[i] != cluster)
      i++;
   
      if(i >= popSize){
         for(int j = 0; j < popSize; j++){
            cout << _cluster[j] << endl;
         }
      }
   
   indexMin = i;
   fMin = fValues[i];
   
   for(int j = i+1; j < popSize; j++){
      if(_cluster[j] == cluster){
         if(fValues[j] < fMin){
            fMin = fValues[j];
            indexMin = j;
         }
      }
   }
   
   return indexMin;
}

