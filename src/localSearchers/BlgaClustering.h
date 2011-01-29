/***************************************************************************
 *   Copyright (C) 2008 by rdc                                             *
 *   rdc_120@hotmail.com                                                   *
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
 
#ifndef BLGA_CLUSTERING
#define BLGA_CLUSTERING

#include "Blga.h"
#include <vector>

class BlgaClustering : public Blga{
  public:
    void iterate();
    void fastIterate();
    BlgaClustering(int itC, int popSize, int dimension, int alfa, int numMates, 
                   int pamNass, int rtsNass, FitnessFunction *ff, 
                   Random *random, int k, double dmin, double dmax,
                   double alpha);
    BlgaClustering(int itC, int popSize, int dimension, double probMux, 
                   int numMates, int pamNass, int rtsNass, FitnessFunction *ff, 
                   Random *random, int k, double dmin, double dmax, 
                   double alpha);
                    
    ~BlgaClustering();
  private:
    void clustering(); // Calcula el fitness modificado según clustering
    void clustering(char *gen,double &fitness); /* Esta función hace clustering
                                                   para un sólo individuo, usada
                                                   para offspring */
    void populateCentroids(); /* Busca los centroides y añade el resto de 
                                 individuos a los clusters formados */
    int searchCentroid(int index); /* Busca el centroide más cercano, devuelve
                                      -1 si la distancia al centroide más 
                                      cercano es mayor que _dmax */
    int searchCentroid(char *gen);
    int searchWorstClus(); /* Búsqueda el peor, evitando los individuos que son
                              centroides */
    double _dmin; // Distancia minima entre centroides
    double _dmax; // Distancia maxima entre un individuo y su centroide
    double _alpha;// Constante usada para calcular el fitness modificado
    int _nCentroids; // Número de centroides
    int _nCaux;      // Variable auxiliar para mantener el número de centroides
    vector<int> _centroids; // Vector usado para mantener los centroides
    int *_cluster; /* _cluster[i] == -1 -> el elemento i no tiene _cluster
                                      asignado
                      _cluster[i] == -2 -> el elemento i es centroide
                      _cluster[i] == n, 0 <= n < popSize -> n es el centroide
                                     el individuo i */ 
 
};

#endif

