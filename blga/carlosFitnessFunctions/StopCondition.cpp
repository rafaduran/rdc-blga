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
#include <cstdlib>

#include "StopCondition.h"
#include "FitnessStopC.h"
#include "NFEsStopC.h"
#include "NRestartsSC.h"

StopCondition::StopCondition()
{
}


StopCondition::~StopCondition()
{
}

/* Método usado para la creación de todos los objetos que implementan los
   diferentes criterios de parada incluidos */
StopCondition * StopCondition::getSC( char *cadena, FitnessFunction * ff )
{
	StopCondition *sc;

	if (cadena[0] == 'f')
		sc = new FitnessStopC(atof(cadena + 1), 1000000, ff);
	else if (cadena[0] == 'n')
		sc = new NFEsStopC(atoi(cadena + 1));
	else if (cadena[0] == 'r')
		sc = new NRestartsSC(atoi(cadena + 1));
	else{
		cerr << "Condición de parada no entendida: " << cadena << endl;
		exit(1);
	}
	
	return sc;
}


