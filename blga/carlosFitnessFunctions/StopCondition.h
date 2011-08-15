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
#ifndef STOPCONDITION_H
#define STOPCONDITION_H

#include "FitnessFunction.h"

typedef struct{
	double fitness;
	int nFEs;
	int nRestarts;
} StopCStruct;

/**
@author Carlos García Martínez
*/

/* Clase base usada para implementar los diferentes criterios de parada que
   pueden ser usados */
class StopCondition{
public:
    StopCondition();

    ~StopCondition();

	 virtual bool isTrue(StopCStruct &scs) = 0;
	 // Método usado para comprobar si se ha alcanzado el criterio de parada
	 
	 static StopCondition * getSC(char *cadena, FitnessFunction *ff);
	 /* Método usado para la creación de todos los objetos que implementan los
	    diferentes criterios de parada incluidos */
	 
	 virtual long getMaxEvaluations() = 0;
    // Devuelve el número máximo de evaluaciones
};

#endif
