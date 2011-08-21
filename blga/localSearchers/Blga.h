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
#ifndef BLGA_H
#define BLGA_H

#include <fstream>
#include "LocalSearcher.h"
#include "FitnessFunction.h"
#include "Random.h"
#include "resultwriter.h"

/**
 @author Carlos
 */
class Blga : public LocalSearcher{
  protected:
    char **population;  // Población
    double *fValues;    // Vector de fitness de la población
    int popSize;        // Tamaño de la población
    int lastIndex;      // Variable auxiliar
    FitnessFunction *ff; // ff mantiene una referencia a la función de fitness
    char *offspring; // Aqui guardaremos los individuos creados
    char *cl;        // Cromosoma líder
    char *mask;      /* máscara que nos permite evitar explorar una solución más
                        de una vez */
    Random *random;
    int dimension;   // Longitud de las cadenas binarias
    int numMates;    // Número de individuos que participan en el cruce
    int pamNass;     // Número de individuos que compiten en PAM
    int rtsNass;     // Número de individuos que compiten en RTS
    double probMUX;  // Probabilidad usada en el cruce
    double fCL;      // Fitness del cromosoma líder
    char *auxStr;    // Variable auxiliar
    int numProtected;   // Número de bits protegidos por la máscara
    bool repulse;    // Valor usado en el cruce
    int itC;         /* Variable que indica la frecuencia con la que se llaman
                        iterate y fastIterate */
    ResultWriter<ofstream>* rw_;
    
    // Cruce multipadre uniforme
    virtual void crossMUX(double probM, char *cl, char **mates, int numMates, 
                          char *off, char *mask, int size);
    
    // Selección PAM
    int pamSelection(int nass, char *cl, int dimension, char** selected = 0, 
                     int numselected = 0);
    
    // Distancia Hamming                 
    int distance(char *s1, char *s2, int size, int maxDistance = -1);
    
    // Función que comprueba la convergencia
    bool hasConverged();
    
    public:
      Blga(int itC, int popSize, int dimension, double probMux, int numMates,int pamNass,
           int rtsNass, FitnessFunction *ff, Random *random);
      
      Blga(int itC, int popSize, int dimension, int alfa, int numMates, int pamNass, 
           int rtsNass, FitnessFunction *ff, Random *random);
      
      virtual ~Blga();
      
      // Función encargado de realizar una iteración
      virtual void iterate();
      
      /* Función encargada de realizar una iteración rápida, usada por las
         heredadas */
      virtual void fastIterate( );
      
      /* Función que es llamada para refinar una solución */    
      virtual int improve(char *s, double& fitness, int size,int maxEvaluations,
                          int iRuns, int current_nFEs, int fNumber,
                          const char *name);
      
      virtual int improve(char *s, double& fitness, int size);
      
      /* Genera una nueva población aelatoria, usada cuando se realizan varias
         ejecucicones con los mismos parámetros, permitiendo realizar sucesivas
         llamadas */
      virtual void resetPopulation();
      
      // Asocia una función de fitness
      virtual void setFF( FitnessFunction * ff );
      
      // Función auxiliar usada para guardar los resultados en un fichero                           
      void writeResults(int iRuns,int nFEs, bool is_last_run,
                        bool is_last_iteration); 
      
      void swap(int *a, int *b); // Función auxiliar de quick sort
      
      // Realiza quick sort, ordenando según el fitness de forma decreciente
      void quickSort(int arr[], int beg, int end);
         
};

#endif
