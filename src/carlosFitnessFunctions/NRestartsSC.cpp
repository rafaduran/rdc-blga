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
#include "NRestartsSC.h"

NRestartsSC::~NRestartsSC()
{
}

NRestartsSC::NRestartsSC( int numRestarts )
{
	this->numRestarts = numRestarts;
}

/* Función usada para comprobar si se ha alcanzado el creterio de parada,
   devolviendo cierto cuando se alcanza el número de reinicios de parada */ 
bool NRestartsSC::isTrue( StopCStruct & scs )
{
	if (scs.nRestarts >= this->numRestarts)
		return true;
	else
		return false;
}

// Devuelve el número máximo de evaluaciones
long NRestartsSC::getMaxEvaluations( )
{
	return 1000000;
}
