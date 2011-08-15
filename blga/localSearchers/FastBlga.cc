#include "fastblga.h"

void FastBlga::initMates(char *cl, int size, int numMates, Random *random){
  
  this->mates.clear();
  this->distMates.clear();
  this->resetMeanMates();
  int *perm = new int[popSize];
  
  random->RandPerm(perm, popSize);
  
  for (int i = 0; i < numMates; i++)
    this->mates.push_back(perm[i]);
  
  this->indexFarestMate = 0;
  this->indexNearestMate = 0;
  
  for (int i = 0; i < numMates; i++){
    this->distMates.push_back(this->distance(cl, this->population[mates.at(i)], size));
    this->addMateToMean(population[mates.at(i)], size, numMates);
    
    if (this->distMates.at(i) > this->distMates.at(this->indexFarestMate))
      this->indexFarestMate = i;
    
    if (this->distMates.at(i) < this->distMates.at(this->indexNearestMate))
      this->indexNearestMate = i;
  }
  
  delete [] perm;
}

void FastBlga::updateIndexNearestFarest(){
  
  this->indexFarestMate = 0;
  this->indexNearestMate = 0;
  
  for (int i = 1; i < numMates; i++){
    
    if (this->distMates.at(i) > this->distMates.at(this->indexFarestMate))
      this->indexFarestMate = i;
    
    if (this->distMates.at(i) < this->distMates.at(this->indexNearestMate))
      this->indexNearestMate = i;
  }
}

int FastBlga::isAMate(int index){
  
  for (int i = 0; i < numMates; i++){
    
    if (this->mates.at(i) == index)
      return i;
  }
  
  return -1;
}

void FastBlga::resetMeanMates(){
  
  for (int i = 0; i < this->dimension; i++)
    this->meanMates[i] = 0;
}

void FastBlga::addMateToMean(char *mate, int size, int numMates){
  
  double increment = 1. / numMates;
  
  for (int i = 0; i < this->dimension; i++)
    this->meanMates[i] += mate[i] * increment;
}

void FastBlga::removeMateFromMean(char *mate, int size, int numMates){
  
  double decrement = 1. / numMates;
  
  for (int i = 0; i < this->dimension; i++)
    this->meanMates[i] -= mate[i] * decrement;
}

void FastBlga::updateDistMates(char *offspring, char *cl, int size){
  
  for (int i = 0; i < size; i++){
    
    if (offspring[i] != cl[i]){
      
      for (int j = 0; j < numMates; j++){
        
        if (offspring[i] != population[mates.at(j)][i])
          distMates.at(j) += 1;
        else
          distMates.at(j) -=1;
      }
    }
  }
  
  this->updateIndexNearestFarest();
}

/*int FastBlga::indexNearestMate(){

 int nearest = 0;

 for (int i = 1; i < numMates; i++){

 if (distMates[i] < distMates[nearest])
 nearest = i;
 }

 return nearest;
 }*/

void FastBlga::crossMUX(double probM, char *cl, vector<int> &mates, int numMates,
double* meanMates, char *off, char *mask, int size){
  
  int numDiferences = 0;
  memcpy( offspring, cl, size * sizeof( char ));
  int *perm = new int[ size ];
  random->RandPerm(perm, size);
  
  for (int i = 0; i < size; i++){
    
    int cIndex = perm[i];
    
    if (mask[cIndex] == 1){
      
      double probChange;
      
      if (offspring[cIndex] == 0)
        probChange = meanMates[cIndex];     //to assign a '1'
      else                                    //to assign a '0'
        probChange = 1.0 - meanMates[cIndex];
      
      if (random->Rand() < (probChange * probM)){
        offspring[ cIndex ] = 1 - offspring[cIndex];
        numDiferences++;
        this->numProtected++;
        mask[cIndex] = 0;
      }
    }
  }
  
  if (numDiferences == 0){
    
    for (int i = 0; i < size; i++){
      
      int cIndex = perm[i];
      
      if (mask[cIndex] == 1){
        offspring[cIndex] = 1 - offspring[cIndex];
        numDiferences++;
        this->numProtected++;
        mask[cIndex] = 0;
        break;
      }
    }
  }
  
  delete [] perm;
}

FastBlga::FastBlga(int popSize, int dimension, double probMux, int numMates, int rtsNass, FitnessFunction *ff, Random *random)
:BLga(popSize, dimension, probMux, numMates, 0, rtsNass, ff, random){
  
  meanMates = new double[dimension];
}


FastBlga::FastBlga(int popSize, int dimension, int alfa, int numMates, int rtsNass, FitnessFunction *ff, Random *random)
:BLga(popSize, dimension, alfa, numMates, 0, rtsNass, ff, random){
  
  meanMates = new double[dimension];
}

FastBlga::~FastBlga(){
  mates.clear();
  distMates.clear();
  delete [] meanMates;
}

int FastBlga::improve(char *s, double &fitness, int size, int maxEvaluations){
  this->initMates(s, size, numMates, random);
  int numEvaluations = this->BLga::improve(s, fitness, size, maxEvaluations);
  
  //Insert in the population
  /*int indexNearest = this->indexNearestMate();
  
   if (ff->compare(fitness, fValues[mates.at(indexNearest)]) > 0){
   memcpy(population[mates.at(indexNearest)], s, sizeof(char) * dimension);
   fValues[mates.at(indexNearest)] = fCL;
   }*/
  
  return numEvaluations;
}

int FastBlga::improve(char *s, double &fitness, int size){
  this->initMates(s, size, numMates, random);
  int numEvaluations = this->BLga::improve(s, fitness, size);
  
  //Insert in the population
  /*int indexNearest = this->indexNearestMate();
  
   if (ff->compare(fitness, fValues[mates.at(indexNearest)]) > 0){
   memcpy(population[mates.at(indexNearest)], s, sizeof(char) * dimension);
   fValues[mates.at(indexNearest)] = fCL;
   }*/
  
  return numEvaluations;
}

void FastBlga::iterate(){
  
  //SELECCIÓN MATES (el más lejano es reemplazado si es peor que uno aleatorio
  int selected = random->Randint(0, this->lastIndex);
  
  if (isAMate(selected) < 0){
    
    int newDist = this->distance(cl, population[selected], this->dimension);
    
    //REEMPLAZAR SI ESTA MÁS CERCA O IGUAL
    if (newDist <= this->distMates[this->indexFarestMate]){
      this->removeMateFromMean(population[mates[this->indexFarestMate]], dimension, numMates);
      this->mates[this->indexFarestMate] = selected;
      this->distMates[this->indexFarestMate] = newDist;
      this->updateIndexNearestFarest();
      this->addMateToMean(population[selected], dimension, numMates);
    }
    
    //REEMPLAZAR SI ES MEJOR AL MÁS LEJANO XXXXXX
    /*if (ff->compare(fValues[selected], fValues[mates[this->indexFarestMate]]) > 0){
     this->removeMateFromMean(population[mates[this->indexFarestMate]], dimension, numMates);
     int newDist = this->distance(cl, population[selected], this->dimension);
     this->mates[this->indexFarestMate] = selected;
     this->distMates[this->indexFarestMate] = newDist;
     this->updateIndexFarest();
     this->addMateToMean(population[selected], dimension, numMates);
     }*/
  }
  
  //CRUCE
  crossMUX(probMUX, cl, mates, numMates, meanMates, offspring, mask, dimension);
  
  //EVALUACION
  double fOffspring = this->ff->fitness( offspring, cl, fCL, dimension );
  
  //UPDATE CL
  if ( ff->compare( fOffspring, fCL ) > 0 ) {
    this->updateDistMates(offspring, cl, dimension);
    memcpy( auxStr, offspring, sizeof( char ) * dimension );
    memcpy( offspring, cl, sizeof( char ) * dimension );
    memcpy( cl, auxStr, sizeof( char ) * dimension );
    double auxF = fOffspring;
    fOffspring = fCL;
    fCL = auxF;
    
    //RESET THE MASK
    this->numProtected = 0;
    
    for ( int i = 0; i < dimension; i++ )
      this->mask[ i ] = 1;
  }
  
  //INSERT IN THE POPULATION
  /*if (ff->compare(fOffspring, fValues[mates[this->indexNearestMate]]) >= 0) {
   int newDist = this->distance(cl, offspring, this->dimension);
   this->removeMateFromMean(population[mates[this->indexNearestMate]], dimension, numMates);
   memcpy(population[mates[this->indexNearestMate]], offspring, dimension * sizeof(char));
   this->distMates[this->indexNearestMate] = newDist;
   this->fValues[mates[this->indexNearestMate]] = fOffspring;
   this->updateIndexNearestFarest();
   this->addMateToMean(offspring, dimension, numMates);
   }*/
  
  int nearest = this->pamSelection( this->rtsNass, this->offspring, this->dimension );
  
  if ( ff->compare( fOffspring, fValues[ nearest ] ) > 0 ) {
    
    int isMate = this->isAMate(nearest);
    
    if (isMate >= 0){
      this->removeMateFromMean(population[nearest], this->dimension, this->numMates);
      this->addMateToMean(offspring, this->dimension, this->numMates);
      this->distMates[isMate] = this->distance(cl, offspring, dimension);
      this->updateIndexNearestFarest();
    }
      
    memcpy( population[ nearest ], offspring, sizeof( char ) * dimension );
    fValues[ nearest ] = fOffspring;
  }
}


