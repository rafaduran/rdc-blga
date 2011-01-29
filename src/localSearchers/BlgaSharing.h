//
// C++ Interface: BlgaSharing
//
// Description: 
//
//
// Author: rdc,,, <rdc_120@hotmail.com>, (C) 2008
//
// Copyright: See COPYING
#ifndef BLGA_SHARING
#define  BLGA_SHARING

#include "Blga.h"

class BlgaSharing : public Blga{
  public:
    void iterate();
    void fastIterate();
    BlgaSharing(int itC, int popSize, int dimension, int alfa, int numMates, 
                int pamNass, int rtsNass, FitnessFunction *ff, Random *random,
                double alpha, double shRadius);
    BlgaSharing(int itC, int popSize, int dimension, double probMux, 
                int numMates, int pamNass, int rtsNass, FitnessFunction *ff, 
                Random *random, double alpha, double shRadius);
  private:
    void sharing(); // Hace sharing a toda la población
    double sh(int index); // Función auxiliar para el sharing
    double sh(char *genes);
    int searchWorst(); // Búsqueda el peor
    
    double _alpha; // Constante usada para calcular el fitness modificado
    double _shRadius; // Radio de sharing
};

#endif
