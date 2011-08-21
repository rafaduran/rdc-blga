/***************************************************************************
 *   Copyright (C) 2006 by Carlos García Martínez   *
 *   carlos@linux   *
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "StopCondition.h"
#include "FitnessFunction.h"
#include "Random.h"
#include "LocalSearcher.h"


using namespace std;

char *** creaInitSolutions(Random *random, int length, int numSolutions, 
                           int numRuns){
  
  char ***matrix;
  
  matrix = new char**[numRuns];
  
  for (int i = 0; i < numRuns; i++){
    
    matrix[i] = new char*[numSolutions];
    
    for (int j = 0; j < numSolutions; j++){
      matrix[i][j] = new char[length];
      
      for (int k = 0; k < length; k++)
	matrix[i][j][k] = random->Randint(0, 1);
    }
  }
  
  return matrix;
}

void randomSolution(Random *random, char *sol, int n){
  
  for (int i = 0; i < n; i++)
    sol[i] = random->Randint(0, 1);
}

void liberaInitSolutions(char ***matrix, int numSolutions, int numRuns){
  
  for (int i = 0; i < numRuns; i++){
    
    for (int j = 0; j < numSolutions; j++){
      
      delete [] matrix[i][j];
    }
    
    delete [] matrix[i];
  }
  
  delete [] matrix;
}
 

int main(int argc, char *argv[]){


  if (argc < 6){
    cout << argv[0] << " <localSearch> <functionNumber> <stopc> <initRun> <numRuns>" << endl;
    cout << endl;
    cout << "<localSearch>: ";
    LocalSearcher::showLSs();
    cout << "<stopC>: (f<fitness>|n<numEvaluations>)" << endl;
    return 0;
  }
  
  int itCalls = atoi(argv[2]);
  int blgaPopSize = atoi(argv[3]);
  char *lsName = argv[1];
  Random *random = new Random();
  int functionNumber = atoi(argv[argc - 4]);
  char *stopCstring = argv[argc - 3];
  int initRun = atoi(argv[argc - 2]);
  int numRuns = atoi(argv[argc - 1]);
  char ***initSolutions;
  FitnessFunction *ff = FitnessFunction::getFitnessFunction(functionNumber);
  
  if( ff == NULL )
    return EXIT_FAILURE;
  //int size = dimensionesFFs[functionNumber];
  int numSolutions = 1000;
  int incS = 10;  //incrementSampling
  int nextS = 0; //nextSampling
  double bestF, cF; //bestFitness y currentFitness
  ostringstream algName;
  algName << "msls_";// << lsName;
  char *cS; //currentSolution
  random->cambiaSemilla(semillas[49]);
  ff->getDim();
  initSolutions = creaInitSolutions(random, ff->getDim(), numSolutions, numRuns);
  StopCondition *auxsc = StopCondition::getSC(stopCstring, ff);
  LocalSearcher *ls = LocalSearcher::getLS(argc - 5, argv + 1, ff, random, 
                                           algName, auxsc->getMaxEvaluations());
  delete auxsc;
  
  if (ls == NULL){
    cout << argv[0] << " <localSearch> <functionNumber> <stopc> <initRun> <numRuns>" << endl;
    cout << endl;
    cout << "<localSearch>: ";
    LocalSearcher::showLSs();
    cout << "<stopC>: (f<fitness>|n<numEvaluations>)" << endl;
    return 0;
  }
  
  ostringstream aux;
  aux << "mkdir " << algName.str() << " 2> /dev/null";
  system(aux.str().data());
  aux.str("");
  
  aux << algName.str() << "/" << ff->getName().str();
  
  time_t comienzo,final;
 
  for (int iRuns = initRun; iRuns < numRuns; iRuns++){
    
    comienzo = time(NULL);
    
    ff = FitnessFunction::getFitnessFunction(functionNumber, iRuns);
    StopCondition *sc = StopCondition::getSC(stopCstring, ff);
    StopCStruct stopStruct;
    ls->setFF(ff);
    
    ls->reset(lsName);
    //#include <iostream>
    random->cambiaSemilla(semillas[iRuns]);
    bestF = ff->fitness(initSolutions[iRuns][0]);
    stopStruct.nFEs = 1;
    stopStruct.nRestarts = 0;
    stopStruct.fitness = bestF;
    int iSols = 0;
   
    /*sF << stopStruct.fitness << "\t";
    sE << stopStruct.nFEs << "\t";*/
    
    while (!sc->isTrue(stopStruct)){
      //for (int iSols = 0; iSols < numSolutions; iSols++){
      if (iSols < 1000)
	cS = initSolutions[iRuns][iSols];
      else{
	randomSolution(random, initSolutions[0][0], ff->getDim());
	cS = initSolutions[0][0];
      }
      cF = ff->fitness(cS);
      
      
      int current_nFEs = 0;
      
      if ( stopCstring[ 0 ] == 'n' )
        current_nFEs = ls->improve(cS, cF, ff->getDim(), sc->getMaxEvaluations()
                                   - stopStruct.nFEs - 1,
                                   iRuns, stopStruct.nFEs, functionNumber,
                                   aux.str().data()) + 1;
      else
        current_nFEs = ls->improve( cS, cF, ff->getDim() ) + 1;
      
      stopStruct.nFEs += current_nFEs;
      
      if (ff->compare(cF, bestF) > 0)
	bestF = cF;
      
      stopStruct.fitness = bestF;
      stopStruct.nRestarts++;
      
      iSols++;
    }
    
      //Escribimos los resultados
      ofstream tm("time",ios::app);
      final = time(NULL);
      tm << "Tiempo ejecución para iteración " << iRuns << " del algoritmo " <<
            aux.str().data() <<  " " << difftime(final,comienzo) << endl;
      tm.close();
      
      ls->writeResults(iRuns, stopStruct.nFEs, true, 
                       (iRuns==(numRuns-1))?true:false);
      
      delete sc;
  }
  
  liberaInitSolutions(initSolutions, numSolutions, numRuns);

  delete random;
  delete ls;
  cout << "Todo bien!" << endl;
  
  return EXIT_SUCCESS;
}
