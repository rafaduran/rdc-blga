/***************************************************************************
 *   Copyright (C) 2006 by Carlos   *
 *   cgarcia@uco.es   *
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
#include <cstdlib>
#include <cstring>

#include "LocalSearcher.h"
#include "Blga.h"
#include "BlgaSharing.h"
#include "BlgaClearing.h"
#include "BlgaClustering.h"
#include "ClusteredClearing.h"

LocalSearcher::LocalSearcher() {
}


LocalSearcher::~LocalSearcher() {
}

/* Función estática usada para crear todos los algoritmos de búsqueda local
   incluidos. Para ello recibe todos los argumentos necesarios, un objeto de
   fitness que se asociará al algoritmo y un objeto Random usado para obtener
   los valores aleatorios. En algName  se guardará el nombre del algoritmo y los
   valores de los parámetros usados */
LocalSearcher * LocalSearcher::getLS(int nArgs, char *args[], 
   FitnessFunction *ff, Random *random, ostringstream &algName,
   int maxEvaluations){
   LocalSearcher *ls;
  
   int iParameters = 0;
   char *lsName = args[0]; iParameters++;
 
   if(0 == strcmp(lsName, "blga")){
    
      if (nArgs < 7){
         return NULL;
      }
      int itCalls = atoi(args[iParameters]); iParameters++;
      int blgaPopSize = atoi(args[iParameters]); iParameters++;
      char *alfaChar = args[iParameters]; iParameters++;
      int numMates = atoi(args[iParameters]); iParameters++;
      int pamnass = atoi(args[iParameters]); iParameters++;
      int rtsnass = atoi(args[iParameters]); iParameters++;
    
      if (alfaChar[0] == 'r'){
         double pf = atof(alfaChar + 1);
         ls = new Blga(itCalls, blgaPopSize, ff->getDim(), pf, numMates, 
                       pamnass, rtsnass, ff, random);
         algName << lsName << "_itCalls"<< itCalls << "_N" << blgaPopSize << 
                 "_pf" << pf << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass;
      }
      else{
         int alfa = atoi(alfaChar);
         ls = new Blga(itCalls, blgaPopSize, ff->getDim(), alfa, numMates, 
                       pamnass, rtsnass, ff, random);
         algName << lsName << "_itCalls" << itCalls << "_N" << blgaPopSize << 
                 "_A" << alfa << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass;
      }
   } else if(0 == strcmp(lsName,"blgaSharing")) {
  
      if(nArgs < 9) {
         return NULL;
      }
      int itCalls = atoi(args[iParameters]); iParameters++;
      int blgaPopSize = atoi(args[iParameters]); iParameters++;
      char *alfaChar = args[iParameters]; iParameters++;
      int numMates = atoi(args[iParameters]); iParameters++;
      int pamnass = atoi(args[iParameters]); iParameters++;
      int rtsnass = atoi(args[iParameters]); iParameters++;
      double alpha = atof(args[iParameters]); iParameters++;
      double shRadius = atof(args[iParameters]); iParameters++;
    
      if (alfaChar[0] == 'r'){
         double pf = atof(alfaChar + 1);
         ls = new BlgaSharing(itCalls, blgaPopSize, ff->getDim(), pf,numMates, 
                              pamnass, rtsnass, ff, random, alpha, shRadius);
      
         algName << lsName << "_itCalls" << itCalls << "_N" << blgaPopSize << 
                    "_pf" << pf << "_m" << numMates << "_pamn" << pamnass << 
                    "_rtsn" << rtsnass << "_alpha" << alpha << "_shRarius" << 
                    shRadius;
      }
      else{
         int alfa = atoi(alfaChar);
         ls = new BlgaSharing(itCalls, blgaPopSize, ff->getDim(), alfa, 
                              numMates, pamnass, rtsnass, ff, random, alpha, 
                              shRadius);
                           
         algName << lsName << "_itCalls" << itCalls <<"_N" << blgaPopSize << 
                 "_A" << alfa << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass << "_alpha" << alpha << "_shRarius" << 
                 shRadius;
      }
   } else if(0 == strcmp(lsName,"blgaClearing")) {
  
      if(nArgs < 9) {
         return NULL;
      }
    
      int itCalls = atoi(args[iParameters]); iParameters++;
      int blgaPopSize = atoi(args[iParameters]); iParameters++;
      char *alfaChar = args[iParameters]; iParameters++;
      int numMates = atoi(args[iParameters]); iParameters++;
      int pamnass = atoi(args[iParameters]); iParameters++;
      int rtsnass = atoi(args[iParameters]); iParameters++;
      int k = atoi(args[iParameters]); iParameters++;
      double clRadius = atof(args[iParameters]); iParameters++;
    
      if (alfaChar[0] == 'r'){
         double pf = atof(alfaChar + 1);
         ls = new BlgaClearing(itCalls, blgaPopSize, ff->getDim(), pf,numMates, 
                               pamnass, rtsnass, ff, random, k, clRadius);
      
         algName << lsName << "_itCalls" << itCalls <<"_N" << blgaPopSize << 
                    "_pf" << pf << "_m" << numMates << "_pamn" << pamnass << 
                    "_rtsn" << rtsnass << "_k" <<  k << "_clRarius" << clRadius;
      } else {
         int alfa = atoi(alfaChar);
         ls = new BlgaClearing(itCalls, blgaPopSize, ff->getDim(), alfa, 
                               numMates, pamnass, rtsnass, ff, random, k, 
                               clRadius);
                           
         algName << lsName << "_itCalls" << itCalls << "_N" << blgaPopSize << 
                 "_A" << alfa << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass << "_k" << k << "_clRarius" << clRadius;
      }
   } else if(0 == strcmp(lsName,"blgaClustering")) {
  
      if(nArgs < 10) {
         return NULL;
      }
      int itCalls = atoi(args[iParameters]); iParameters++;
      int blgaPopSize = atoi(args[iParameters]); iParameters++;
      char *alfaChar = args[iParameters]; iParameters++;
      int numMates = atoi(args[iParameters]); iParameters++;
      int pamnass = atoi(args[iParameters]); iParameters++;
      int rtsnass = atoi(args[iParameters]); iParameters++;
      int nCentroids = atoi(args[iParameters]); iParameters++;
      double dmin = atof(args[iParameters]); iParameters++;
      double dmax = atof(args[iParameters]); iParameters++;
      double alpha = atof(args[iParameters]); iParameters++;
 
      if (alfaChar[0] == 'r'){
         double pf = atof(alfaChar + 1);
         ls = new BlgaClustering(itCalls, blgaPopSize, ff->getDim(), pf,
                                 numMates, pamnass, rtsnass, ff, random, 
                                 nCentroids, dmin, dmax, alpha);
      
         algName << lsName << "_itCalls" << itCalls << "_N" << blgaPopSize << 
                 "_pf" << pf << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass << "_nCentroids" << nCentroids << "_dmin" 
                 << dmin << "_dmax" << dmax << "_alpha" << alpha;
      } else {
         int alfa = atoi(alfaChar);
         ls = new BlgaClustering(itCalls, blgaPopSize, ff->getDim(), alfa, 
                                 numMates, pamnass, rtsnass, ff, random, 
                                 nCentroids, dmin, dmax, alpha);
                           
         algName << lsName << "_itCalls" << itCalls <<"_N" << blgaPopSize << 
                 "_A" << alfa << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass << "_nCentroids" << nCentroids << "_dmin" <<
                  dmin << "_dmax" << dmax << "_alpha" << alpha;
      }
   } else if(0 == strcmp(lsName,"ClusteredClearing")) {
      if(nArgs < 8) {
         return NULL;
      }
    
      int nOff = atoi(args[iParameters]); iParameters++;
      int blgaPopSize = atoi(args[iParameters]); iParameters++;
      char *alfaChar = args[iParameters]; iParameters++;
      int numMates = atoi(args[iParameters]); iParameters++;
      int pamnass = atoi(args[iParameters]); iParameters++;
      int rtsnass = atoi(args[iParameters]); iParameters++;
      double clRadius = atof(args[iParameters]); iParameters++;
    
      if (alfaChar[0] == 'r'){
         double pf = atof(alfaChar + 1);
         ls = new ClusteredClearing(nOff, blgaPopSize,ff->getDim(), pf,
            numMates, pamnass, rtsnass, ff, random, clRadius);
      
         algName << lsName << "_nOff" << nOff <<"_N" << blgaPopSize << 
                    "_pf" << pf << "_m" << numMates << "_pamn" << pamnass << 
                    "_rtsn" << rtsnass << "_clRarius" << clRadius;
      } else {
         int alfa = atoi(alfaChar);
         ls = new ClusteredClearing(nOff, blgaPopSize, ff->getDim(), alfa, 
            numMates, pamnass, rtsnass, ff, random, clRadius);
                           
         algName << lsName << "_nOff" << nOff << "_N" << blgaPopSize << 
                 "_A" << alfa << "_m" << numMates << "_pamn" << pamnass << 
                 "_rtsn" << rtsnass << "_clRarius" << clRadius;
      }
   } else {
      cerr << "Búsqeda local no reconocida" << endl;
      exit(1);
   }
  
   return ls;
}

void LocalSearcher::showLSs(){
   cout << " Blga <itC> <blgaPopSize> (<alfa>|r<pf>) <numMates> <pamnass> "
      << "<rtsnass>" << endl;
  
   cout<< " BlgaSharing <itC> <blgaPopSize> (<alfa>|r<pf>) <numMates> <pamnass>"
       << " <rtsnass> <alpha> <shRadius>" << endl;
       
   cout << " BlgaClearing <itC> <blgaPopSize> (<alfa>|r<pf>) <numMates> " 
       << "<pamnass> <rtsnass> <k> <clRadius>" << endl;
       
   cout << " BlgaClustering <itC> <blgaPopSize> (<alfa>|r<pf>) <numMates> " <<
       "<pamnass> <rtsnass> <nCentroids> <dmin> <dmax> <alpha>" <<  endl;
       
   cout << " ClusterdClearing <nOff> <blgaPopSize> (<alfa>|r<pf>) <numMates> " 
       << "<pamnass> <rtsnass> <clRadius>" << endl;
}

void LocalSearcher::reset(char* lsName){

   if (0 == strcmp(lsName, "blga")){
      ((Blga*)this)->resetPopulation();
   } else if(0 == strcmp(lsName, "blgaSharing")) {
      ((BlgaSharing*)this)->resetPopulation();
   } else if(0 == strcmp(lsName, "blgaClearing")) {
      ((BlgaClearing*)this)->resetPopulation();
   } else if(0 == strcmp(lsName, "blgaClustering")) {
      ((BlgaClustering*)this)->resetPopulation();
   } else if(0 == strcmp(lsName, "ClusteredClearing")) {
      ((ClusteredClearing*)this)->resetPopulation();
  }
    
}
