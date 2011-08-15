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
#ifndef NFESSTOPC_H
#define NFESSTOPC_H

#include "StopCondition.h"

/**
@author Carlos García Martínez
*/

/* Con esta clase se implementa el criterio de parada basado en el número de 
   evaluaciones */
class NFEsStopC : public StopCondition{

		int numFEs;
		
public:
    NFEsStopC(int numFEs);

    ~NFEsStopC();

	 bool isTrue(StopCStruct &scs);
	 /* Función usada para comprobar si se ha alcanzado el creterio de parada,
	    devolviendo verdadero cuando llega al número de evaluaciones de parada */


	long getMaxEvaluations();
   // Devuelve el número máximo de evaluaciones
};

#endif
