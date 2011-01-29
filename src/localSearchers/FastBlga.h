// 
// File:   fastblga.h
// Author: carlos
//
// Created on 6 de marzo de 2007, 12:01
//

#ifndef _fastblga_H
#define	_fastblga_H

#include "blga.h"
#include "../../carlosFitnessFunctions/FitnessFunction.h"
#include "../../carlosFitnessFunctions/Random.h"
#include <vector>

using namespace std;


class FastBlga : public BLga{
    
    vector<int> mates;
    vector<int> distMates;
    int indexFarestMate;
    int indexNearestMate;
    double* meanMates;
    
    //-----------------------
    
    int isAMate(int index);
    void initMates(char *cl, int size, int numMates, Random *random);
    void updateIndexNearestFarest();
    //int indexNearestMate();
    virtual void crossMUX(double probM, char *cl, vector<int> &mates, int numMates,
        double* meanMates, char *off, char *mask, int size);
    void resetMeanMates();
    void addMateToMean(char *mate, int size, int numMates);
    void removeMateFromMean(char *mate, int size, int numMates);
    void updateDistMates(char *offspring, char *cl, int size);

    public:
        FastBlga(int popSize, int dimension, double probMux, int numMates, int rtsNass, FitnessFunction *ff, Random *random);
        FastBlga(int popSize, int dimension, int alfa, int numMates, int rtsNass, FitnessFunction *ff, Random *random);
        
        ~FastBlga();
        
        virtual int improve(char *s, double &fitness, int size, int maxEvaluations);
        virtual int improve(char *s, double &fitness, int size);

        virtual void iterate();
//        virtual void setFF( FitnessFunction * ff );
};

#endif	/* _fastblga_H */

