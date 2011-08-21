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
#ifndef LOCALSEARCHER_H
#define LOCALSEARCHER_H

#include "../carlosFitnessFunctions/FitnessFunction.h"
#include "../carlosFitnessFunctions/Random.h"

#include <iostream>
#include <sstream>

#define ITERATE_CALLS 50


using namespace std;

/**
@author Carlos
*/
class LocalSearcher{

public:
   LocalSearcher();

   virtual ~LocalSearcher();
   
   virtual void setFF(FitnessFunction *ff) = 0;
   
   /* Función usada para refinar una solución hasta convergir, devuelve el 
      número de evaluaciones realizadas */
   virtual int improve(char *s, double& fitness, int size) = 0;
	 
   /* En este caso realiza evaluaciones hata convergir o haya alcando el máximo
      de evaluaciones */
   virtual int improve(char *s, double& fitness, int size, int maxEvaluations,
                       int iRuns, int current_nFEs, int fNumber, 
                       const char *name) = 0;
                       
	/* Función estática que permite la creación de todos algoritmos de búsqueda 
	   local incluidos */
   static LocalSearcher * getLS(int nArgs, char *args[], 
                                FitnessFunction *ff, Random *random,
                                ostringstream &algName, int maxEvaluations);
	
	// Muestra los parámetros de cada algoritmo 
   static void showLSs();
	
	/* Resetea un algoritmo de búsqueda local, usado para poder hacer ejecuciones
	   sucesivas sin tener que crear nuevos objetos */
   void reset(char *lsName);
   
   // Función auxiliar para almacenar los resultados 
   virtual void writeResults(int iRuns,int nFEs, bool is_last_run,
                        bool is_last_iteration){};
};

#endif
