/*
    ClusterdClearing
    Copyright (C) 2011  rafadurancastaneda@gmail.com

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

#ifndef CLUSTEREDCLEARING_H
#define CLUSTEREDCLEARING_H

#include <Blga.h>
#include <vector>
#include <string.h>

struct Off{
   char *genotype;
   double fitness;
};

struct Cluster{
  int cIndex;  // Centroid index
  int size;    // Number of individuals
  string mask;  // Mask used to not repeat new solutions
  int numProtected;  // Number of bits protecteds
};

class ClusteredClearing : public Blga {
   public:
      ClusteredClearing(int nOff, int popSize, int dimension, double probMux,
         int numMates, int pamNass, int rtsNass, FitnessFunction* ff, Random*
         random, double clRadius);
      
      ClusteredClearing(int nOff, int popSize, int dimension, int alfa, int
         numMates, int pamNass, int rtsNass, FitnessFunction *ff, Random
         *random, double clRadius);
         
      virtual ~ClusteredClearing();
      
      virtual void iterate();
      
      virtual int improve(char* s, double& fitness, int size, int
         maxEvaluations, int iRuns, int current_nFEs, int fNumber, const
         char* name);
  
  protected: 
      virtual bool hasConverged();
            virtual void crossMUX(double probM, Cluster &clus, char **mates, int
         numMates, char *off, int size);
      
   private:
      int _numEval;  // Number evaluations made in last iteration
      vector <Cluster> _cs;  // Holds clusters information
      vector <Off> _offsprings; // Holds offspring information
      double _clRadius;   // Clearing radius
      int *_cluster;   /* Cluster index:
                        _cluster[i] = n -> 0 <= n <= number of clusters
                                           i belongs to cluster n 
                        _cluster[i] = -1 -> not assigned
                        _cluster[i] = -2 -> centroid */
      void clearing(); // Clearing method
      
      void clusteredClearingReplacement(); /* Replacement method used by CC */
      
      int searchReplaced();   // Search individual to be replaced
      
      int searchCluster(char *gen); // Return index of closer cluster to gen
      
      int searchWorst(int cluster); /* Return index of worst individual from 
                                       specific cluster */
};

#endif // CLUSTEREDCLEARING_H
