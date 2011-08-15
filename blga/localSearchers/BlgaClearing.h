//
// C++ Interface: BlgaClearing
//
// Description: 
//
//
// Author: rdc,,,rafadurancastaneda@gmail.com (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef BLGA_CLEARING
#define BLGA_CLEARING

#include "Blga.h"

class BlgaClearing : public Blga{
  public:
    void iterate();
    void fastIterate();
    BlgaClearing(int itC, int popSize, int dimension, int alfa, int numMates, 
                 int pamNass, int rtsNass, FitnessFunction *ff, Random *random,
                 int k, double clRadius);
    BlgaClearing(int itC, int popSize, int dimension, double probMux,
                 int numMates, int pamNass, int rtsNass, FitnessFunction *ff, 
                 Random *random, int k, double clRadius);
    ~BlgaClearing();
  private:
    void clearing();
    int pamSelectionCl( int nass, char *cl, int dimension,char cleared, char**
                        selected = 0, int numSelected = 0);
         /* Realiza PAM modificado según el valor de cleared:
            cleared = 1 -> sólo coge individuos con _cleared = 1 (cleared)
            cleared = 0 -> sólo coge individuos con _cleared = 0 (no cleared) */
    int searchWorstCl();  // Búsqueda el peor

    double _clRadius;   // Radio de clearing
    int _k;             // Número de individios que no son cleared en cada nicho
    double *fCopy;      // Para mantener el fitness de la población y evitar
                        // reevaluar la población
    char *_cleared;     /* Cleared[n]: 0 si el individuo n no esta cleared
                                       1 si el individuo n está cleared */
};

#endif
