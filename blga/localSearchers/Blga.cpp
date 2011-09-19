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
#include "Blga.h"
#include <cstring>
//#include <sstream>

//#include "../carlosFitnessFunctions/PeriodicEqual.h"

//#define max(x, y) (((x)>(y))?(x):(y))

Blga::Blga( int itC, int popSize, int dimension, double probMux, int numMates, 
            int pamNass, int rtsNass, FitnessFunction * ff, Random * random )
: LocalSearcher() {
   this->repulse = false; //falso
   this->fValues = new double[ popSize ];
   this->population = new char * [ popSize ];
   
   // Inicializamos aleatoriamente la población y la evaluamos 
   for ( int i = 0; i < popSize; i++ ) {
    
      this->population[ i ] = new char[ dimension ];
    
      for ( int j = 0; j < dimension; j++ )
         this->population[ i ][ j ] = random->Randint( 0, 1 );
    
      this->fValues[ i ] = ff->fitness( population[i]);
   }
   
   // Tomamos los parámetros
   this->itC = itC;
   this->popSize = popSize;
   this->lastIndex = popSize - 1;
   this->ff = ff;
   this->offspring = new char[ dimension ];
   this->cl = NULL;
   this->mask = new char[ dimension ];
   this->auxStr = new char[ dimension ];
   this->random = random;
   this->dimension = dimension;
   this->numMates = numMates;
   this->pamNass = pamNass;
   this->rtsNass = rtsNass;
   
   if ( probMux > 0 )
      this->probMUX = probMux;
   else {
      this->repulse = true; //verdadero
      this->probMUX = -1.0 * probMux;
   }
  
   this->fCL = 0;
   this->numProtected = 0;
   this->alfa = false; 	// Mark it as not used
  
}

Blga::Blga( int itC, int popSize, int dimension, int alfa, int numMates, int pamNass, 
            int rtsNass, FitnessFunction *ff, Random *random )
: LocalSearcher() {
   this->repulse = false; //falso
   this->fValues = new double[ popSize ];
   this->population = new char * [ popSize ];

   // Inicializamos aleatoriamente la población y la evaluamos
   for ( int i = 0; i < popSize; i++ ) {
    
      this->population[ i ] = new char[ dimension ];
    
      for ( int j = 0; j < dimension; j++ )
         this->population[ i ][ j ] = random->Randint( 0, 1 );
   
    
      this->fValues[ i ] = ff->fitness( population[i]);
   }
   
   // Tomamos los parámetros
   this->itC = itC;
   this->popSize = popSize;
   this->lastIndex = popSize - 1;
   this->ff = ff;
   this->offspring = new char[ dimension ];
   this->cl = NULL;
   this->mask = new char[ dimension ];
   this->auxStr = new char[ dimension ];
   this->random = random;
   this->dimension = dimension;
   this->numMates = numMates;
   this->pamNass = pamNass;
   this->rtsNass = rtsNass;
   this->probMUX = ( ( double ) alfa ) / dimension;
  
   if ( alfa < 0 ) {
      this->repulse = true; //verdadero
      this->probMUX = -1.0 * this->probMUX;
   }
  
   this->fCL = 0;
   this->numProtected = 0;
   this->alfa = true;	// Mark it as used
}

Blga::~Blga() {
   delete [] fValues;
  
   for ( int i = 0; i < popSize; i++ )
      delete [] population[ i ];
  
   delete [] population;
   delete [] offspring;
   delete [] mask;
   delete [] auxStr;
   //fo.close();
}

/* Función encargada de realizar la selección PAM, por tanto dado un individuo
   cl, devolverá el que tenga una distancia haming entre nass individuos
   elegidos aleatoriamente. Si selected no esta vacío devolverá el de menor
   distancia a cl o a alguno de los individuos contenidos en selected */
int Blga::pamSelection( int nass, char *cl, int dimension, char** selected, 
                        int numSelected ) {
   int * indexes = new int[ nass ];
   int minDistance;
   int indexMin;
  
   // Escoge nass individuos aleatoriamente de la población
   for ( int i = 0; i < nass; i++ )
      indexes[ i ] = random->Randint( 0, popSize - 1 );
  
   /* Calcula distancia mínima entre el primer elemento y cl (y los individuos
      en selected si numSelected > 0) */
   int k = indexes[ 0 ];
   indexMin = indexes[ 0 ];
   minDistance = distance( cl, population[ k ], dimension );
  
   for (int j = 0; j < numSelected; j++){
      int newDistance2 = distance( selected[j], population[k], dimension, 
                                   minDistance);
    
      if (newDistance2 < minDistance)
         minDistance = newDistance2;
   }
  
   // Comprueba si hay otro individuo con menor distancia hamming
   for ( int i = 1; i < nass; i++ ) {
      k = indexes[ i ];
      int newDistance = distance( cl, population[ k ], dimension, minDistance);
    
      for (int j = 0; j < numSelected; j++){
         int newDistance2 = distance( selected[j], population[k], dimension, 
                                      minDistance);
      
      if (newDistance2 < newDistance)
	      newDistance = newDistance2;
      }
    
      if ( newDistance < minDistance ) {
         minDistance = newDistance;
         indexMin = k;
      }
   }
  
   delete [] indexes;
   return indexMin; // Devuelve el indice del individuo con menor distancia
}

/* Calcula la distancia hamming, si maxDistance = -1 será ignorada, sino si se
   alcanza dicha distancia se deja de comparar bits y se devuelve dicho valor */
int Blga::distance( char * s1, char * s2, int size, int maxDistance ) {
  
   int cDist = 0;
  
   if ( maxDistance == -1 ) {
    
      for ( int i = 0; i < size; i++ ) {
      
         if ( s1[ i ] != s2[ i ] )
	         cDist++;
      }
    
      return cDist;
   }
   else{
    
      for ( int i = 0; i < size && cDist <= maxDistance; i++ ) {
      
         if ( s1[ i ] != s2[ i ] )
	         cDist++;
      }
    
      return cDist;
   }
}

/* Cruce multipadre uniforme. Incialmente offspring es igual a cl, luego se va
   recorriendo offspring y para cada bit si un número generado aleatoriamente
   es menor que la probabilidad probM, se cambia dicho bit por el bit 
   correspondiente de un indiviudo en numMates elegido aleatoriamente. En esta 
   implementación dicho proceso se realiza mediante el uso de probabilidades */
void Blga::crossMUX( double probM, char * cl, char ** mates, int numMates, 
   char * off, char * mask, int size ) {
   int numDiferences = 0;
   char *oldMask = new char[ size ];
   memcpy( oldMask, mask, size * sizeof( char ) );
   memcpy( offspring, cl, size * sizeof( char ) );
   
   // Recorreremos el vector aleatoriamente, para ello usamos perm
   int *perm = new int[ size ];
   random->RandPerm( perm, size );
   
   /* Si repulse es falso probM será la probabilidad de coger un bit de un
      padre aleatorio, en caso contrario será la probabilidad de no cogerlo (por 
      tanto se mantiene el bit de cl) */
   if ( !repulse ) {
    
      for ( int i = 0; i < size; i++ ) {
      
         int cIndex = perm[ i ];
         
         // Si la oldMask = 0 no hacemos nada ya que ese bit esta protegido
         if ( oldMask[ cIndex ] == 1 ) {
	         /* Calculamos la probabilidad de obtener 1 cogiendo el bit cIndex 
	            de un padre aleatorio */
	         double probs1 = 0;
	         double probChange = 0;
	
	         for ( int j = 0; j < numMates; j++ ) {
	            probs1 += mates[ j ][ cIndex ];
	         }
	
	         probs1 = probs1 / numMates;
            /* La probabilidad de cambiar el bit cIndex será 1-probabilidad de 
               obtner 1, si offspring[cIndex] = 1, probabilidad de obtener 1 en 
               caso de que offsrping[cIndex] = 0 */
	         if ( offspring[ cIndex ] == 1 )
	            probChange = 1.0 - probs1;
	         else
	            probChange = probs1;
	         
	         /* Finalmente cambiaremos un bit si un la probabilidad probM 
	            combinada con la probabilidad de que cambie dicho bit
	            es mayor que un generado aleatoriamente */
	         if ( random->Rand() < ( probChange * probM ) ) {
	            offspring[ cIndex ] = 1 - offspring[ cIndex ];
	            numDiferences++;
	            this->numProtected++;
	            mask[ cIndex ] = 0;
	         }
         }
      }
   } else { // Ahora el proceso es el mismo, sólo que repulse = true
    
      for ( int i = 0; i < size; i++ ) {
      
         int cIndex = perm[ i ];
      
         if ( oldMask[ cIndex ] == 1 ) {
	
	         double probs1 = 0;
	
	         double probChange = 0;
	
	         for ( int j = 0; j < numMates; j++ ) {
	            probs1 += mates[ j ][ cIndex ];

	         }
	
	         probs1 = probs1 / numMates;
	
	         if ( offspring[ cIndex ] == 0 ) // repulsa
	            probChange = 1.0 - probs1;
	       else
	            probChange = probs1;
	
	         if ( random->Rand() < ( probChange * probM ) ) {
	            offspring[ cIndex ] = 1 - offspring[ cIndex ];
	            numDiferences++;
	            this->numProtected++;
	            mask[ cIndex ] = 0;
	         }
         }
      }
   }
   
   /* Si tras recorrer offspring no cambiamos ningún bit, cambiamos uno 
      aleatoriamente que no este protegido */
   if ( numDiferences == 0 ) {
    
      for ( int i = 0; i < size; i++ ) {
      
         if ( oldMask[ perm[ i ] ] == 1 ) {
	         offspring[ perm[ i ] ] = 1 - offspring[ perm[ i ] ];
	         numDiferences++;
	         this->numProtected++;
	         mask[ perm[ i ] ] = 0;
	         break;
         }
      }
   }
  
   delete [] perm;
   delete [] oldMask;
}

/* Cada vez que se llama a iterate se realizará una iteración del algoritmo
   AGBL */
void Blga::iterate( ) {
   //SELECCIÓN PAM
   char **mates = new char * [ numMates ];
  
   for ( int i = 0; i < numMates; i++ ) {
      int selected = this->pamSelection( this->pamNass, this->cl, 
                                         this->dimension);
      mates[ i ] = this->population[ selected ];
   }
  
   //CRUCE MULTIPADRE UNIFORME
   this->crossMUX( this->probMUX, this->cl, mates,
                   numMates, this->offspring, this->mask, this->dimension );
  
   //EVALUACION DE OFFSPRING
   double fOffspring = this->ff->fitness( offspring);
  
   //ACTUALIZACIÓN DE CL
   if ( ff->compare( fOffspring, fCL ) > 0 ) {
      memcpy( auxStr, offspring, sizeof( char ) * dimension );
      memcpy( offspring, cl, sizeof( char ) * dimension );
      memcpy( cl, auxStr, sizeof( char ) * dimension );
      double auxF = fOffspring;
      fOffspring = fCL;
      fCL = auxF;
    
      //RESETEAMOS LA MASCARA CUANDO CAMBIE CL
      this->numProtected = 0;
    
      for ( int i = 0; i < dimension; i++ )
         this->mask[ i ] = 1;
   }
  
   //REEMPLAZAMOS MEDIANTE RTS
   int nearest = this->pamSelection( this->rtsNass, this->offspring, 
                                     this->dimension );
  
   if ( ff->compare( fOffspring, fValues[ nearest ] ) > 0 ) {
      memcpy( population[ nearest ], offspring, sizeof( char ) * dimension );
      fValues[ nearest ] = fOffspring;
   }
  
   delete [] mates;
}

/* Cada vez que es llamado realiza una iteración rápida. Este método es usado
   por las clases heredadas y para Blga iterate = fastIterate. Por tanto el
   valor de itC es ignorado */
void Blga::fastIterate( ) {
  this->iterate();
}

/* Reciba una solución y se encarga de refinarla para obtner el máximo global, 
   para ello realiza iteraciones hasta que se llegue al número máximo de
   evaluaciones o haya convergencia. Se devuelve el número de evaluaciones
   realizadas */
int Blga::improve(char * s, double & fitness, int size, int maxEvaluations,
                  int iRuns, int current_nFEs, int fNumber, const char *name ) {
	int numEvaluations = 0;
	this->cl = s;
	this->fCL = fitness;
	this->dimension = size;
	this->numProtected = 0;
   
	if(this->rw_ == NULL){
		this->rw_ = ResultWriter<ofstream>::getResultWriter(0, name,
				this->ff->getNvariables());
		this->rw_->writeParams(this->get_params());
	}

   if(current_nFEs==1){
      this->rw_->startRun(iRuns);
   }
   
   for ( int i = 0; i < size; i++ )
	   this->mask[ i ] = 1;
   /* La primera vez se llama siempre a iterate, luego se va llamando a iterate
      o fastIterate según itC */
   while ( numEvaluations < maxEvaluations && !this->hasConverged() ) {
	   if((current_nFEs + numEvaluations + 1) == 2)
		   this->iterate();
	   else if((current_nFEs + numEvaluations + 1) % itC)
		   this->fastIterate();
	   else
		   this->iterate();

	   numEvaluations++;
    
      /*if((current_nFEs + numEvaluations) % 1000 == 0){
         this->writeResults(iRuns, current_nFEs + numEvaluations, false, false);
      }*/
   }
  
   fitness = this->fCL; // Guardamos el fitness obtenido
   return numEvaluations;  // Devolvemos el número de evaluaciones realizadas
}

// En este caso improve realiza iteraciones hasta convergir
int Blga::improve( char * s, double & fitness, int size ) {
  int numEvaluations = 0;
  this->cl = s;
  this->fCL = fitness;
  this->dimension = size;
  this->numProtected = 0;
  
  for ( int i = 0; i < size; i++ )
    this->mask[ i ] = 1;
  
  while ( !this->hasConverged() ) {
    this->iterate();
    numEvaluations++;
  }
  
  fitness = this->fCL;
  return numEvaluations;
}

/* Si el número de bit protegidos es igual al número de bit total no podemos 
   seguir explorando dicha solución, por tanto se considera que ha convergido */
bool Blga::hasConverged( ) {
  return ( dimension == this->numProtected );
}

// Generamos una nueva población aleatoria
void Blga::resetPopulation( ) {
  for ( int i = 0; i < popSize; i++ ) {
    
    for ( int j = 0; j < dimension; j++ )
      this->population[ i ][ j ] = random->Randint( 0, 1 );
      
       
    this->fValues[ i ] = ff->fitness( population[i]);
  }
}

// Asociamos nuestro algoritmo a una función de fitness
void Blga::setFF( FitnessFunction * ff ) {
  this->ff = ff;
}

// Función auxiliar para almacenar los resultados
void Blga::writeResults(int iRuns,int nFEs, bool is_last_run, 
                        bool is_last_iteration) {
    int fNum = this->ff->getFunctionNumber();
    rw_->startIteration(nFEs);
    for(int i = 0; i < popSize; i++) {
        
        double *vector = new double[ff->getNvariables()];
        double fitness;
        char binary[dimension];

        if( fNum != 2) {
            ff->inverseGray(population[i],binary);
            vector[0] = ff->binaryToDouble(binary);
            fitness = ff->fitness(population[i]);
            this->rw_->write(vector, fitness, (i==popSize-1)?true:false);
        } else {
            vector = new double[ff->getNvariables()];
            ff->inverseGrayVector(population[i],binary);
            ff->binaryToDoubleVector(binary,vector);
            fitness = ff->fitness(population[i]);
            this->rw_->write(vector, fitness, (i==popSize-1)?true:false);
        }
    }
    rw_->endIteration(is_last_iteration);
  
    if(is_last_iteration){
        rw_->endRun(is_last_run);
    }
}

/* Función auxiliar usada por quickSort */
void Blga::swap(int *a, int *b){
   int t=*a; *a=*b; *b=t;
}

/* Ordenación quickSort, inicialmente recibe un vector con los indice de 0 a 
   popSize-1 y devuelve dicho vector de indices ordenados según el fitness de
   los individuos correspondientes a dichos indices */
void Blga::quickSort(int arr[], int beg, int end) {
   if (end > beg + 1) {
      double piv = fValues[arr[beg]];
      int l = beg + 1, r = end;
    
      while (l < r) {
         if (fValues[arr[l]] > piv)
            l++;
         else
         swap(&arr[l], &arr[--r]);
      }
      swap(&arr[--l], &arr[beg]);
      quickSort(arr, beg, l);
      quickSort(arr, r, end);
   }
}

vector<Param> Blga::get_params()
{
	vector<Param> params;

	Param p;
	p.name = "itC";
	p.ivalue = this->itC;
	p.is_int = true;
	params.push_back(p);

	p.name = "N";
	p.ivalue = this->popSize;
	p.is_int = true;
	params.push_back(p);

	p.name = "m";
	p.ivalue = this->numMates;
	p.is_int = true;
	params.push_back(p);

	p.name = "pamn";
	p.ivalue = this->pamNass;
	p.is_int = true;
	params.push_back(p);

	p.name = "rtsn";
	p.ivalue = this->rtsNass;
	params.push_back(p);
	p.is_int = true;

	if(this->alfa){
		p.name = "a";
		p.ivalue = int(this->probMUX*dimension);
		p.is_int = true;
		params.push_back(p);
	} else {
		p.name = "pf";
		p.dvalue = this->probMUX;
		p.is_int = false;
		params.push_back(p);
	}
	p.name = "Blga";
	params.push_back(p);
	return params;
}
