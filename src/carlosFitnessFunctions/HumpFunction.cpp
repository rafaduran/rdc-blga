//
// C++ Implementation: HumpFunction
//
// Description:
//
//
// Author: rdc,,, <rdc_120@hotmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "HumpFunction.h"
#include <cstdlib>

/* Función encargada de obtener los picos generados aleatoriamente. Para ello
   se utiliza un fichero de configuración llamado humpFunctionConfig, en el que
   se guarda información sobre si los ficheros deben crearse o simplemente
   leerse y todos los parámetros necesarios. Junto con el código fuente se 
   suministra un fichero de configuración ejemplo y en Leeme.txt se explican
   todos los parámetros usados.
*/
void HumpFunction::getPeaks( char *filename) {
   int i, j, l, OK;
  
   ifstream in;
    
   in.open( filename, ifstream::in );
   
   if(in.fail()){
      cout << "Error al abrir fichero picos" << endl;
      exit(1);
   }
    
   for ( i = 0; i < _kPeaks; i++ ) {
      for ( j = 0; j < _nVariables; j++ ) {
         in >> _peaks[ i ][ j ];
      }
   }
   in.close();
}

/* Constructor usado cuando se utiliza el tipo de configuración 0 */
HumpFunction::HumpFunction(char *filename, int nVariables, int kpeaks, double
   radius, double alpha, double height, int dimension) {
  this->_radius = radius;
  this->_alpha = alpha;
  this->_height = height;
  this->_nVariables = nVariables;
  this->_kPeaks = kpeaks;
  this->_dimension = dimension;

  _peaks = new double * [ _kPeaks ];
        
  for(int i = 0; i < _kPeaks; i++) {
    _peaks[i] = new double[_nVariables];
  }

  getPeaks( filename);
}

/* Destructor de la función, liberamos la memoria donde se guardan los picos */
HumpFunction::~HumpFunction() {
  for ( int i = 0; i < _kPeaks; i++ )
    delete [] _peaks[ i ];

  delete [] _peaks;
}

/* Función usada para el paso de binario a real, teniendo en cuenta que hay 
   varias variables */
void HumpFunction::binaryToDoubleVector( char *binary, double *vector ) {
  double contador;

  for ( int i = 0; i < _nVariables; i++) {
    contador = 0.0;
    
    for ( int j = 0; j < _dimension; j++) {
      contador += binary[(i*_dimension)+j] * pow((double)2, _dimension-j-1);
    }
   
    vector[i] = contador / (pow((double) 2, _dimension)-1);
  }
}

/* Función de fitness. Como estamos trabajando en código gray lo primero que
   haremos es pasar de gray a binario, luego pasaremos a real, con dicho valor
   buscaremos el pico más cercano y calcularemos el fitness */
double HumpFunction::fitness( char *genes) {
  double vector[_nVariables];
  char binary[_dimension*_nVariables];
   
  this->inverseGrayVector(genes,binary);

  binaryToDoubleVector( binary, vector );
        
  double aux;
  int closest = searchClosestPeak( vector, aux);

  if(aux > _radius) {
    return 0;
  } else {
    double fitness = _height * ( 1 - pow( aux / _radius, _alpha ) );
    return fitness;
  }
}

/* Función que busca el pico más cercano para un individuo, necesaria para el
   calculo de fitness */
int HumpFunction::searchClosestPeak( double *vector, double &difference ) {
  int index = 0;
  double aux;
        
  difference = distance( vector, _peaks[ 0 ] );

  for ( int i = 1; i < _kPeaks; i++ ) {
    aux = distance( vector, _peaks[ i ] );
    if ( aux < difference ) {
      difference = aux;
      index = i;
    }
  }
  
  return index;
}

/* Función usada para comparar dos fitness, debe devolver > 0 si f1 es mayor,
   < 0 si f2 es mayor. En este caso valdrá simplemente con restar los valores de
   fitness. */
double HumpFunction::compare( double f1, double f2 ) {
  return f1-f2;
}

/* Función usada para obtner el nombre de la función de fitness. Dicho nombre
   se usará para crear el archivo donde se guarden los resultados */
stringstream & HumpFunction::getName() {

  _ss << "Hump function";

  return this->_ss;
}

/* Función que devuelve la longitud total de la cadena binaria a usar, en este
   caso será la suma total de la longitud de todas las variables */
int HumpFunction::getDim() {
  return _nVariables * _dimension;
}

/* Función que devuelve el número de variables usadas */
int HumpFunction::getNvariables() {
  return this->_nVariables;
}

/* Función que calcula la distancia entre dos individuos dado su valor real */
double HumpFunction::distance( double *firstPeak, double *secondPeak ) {
  double contador = 0;
  
  for ( int i = 0; i < _nVariables; i++ ) {
    contador += pow( firstPeak[ i ] - secondPeak[ i ],
                   ( double ) 2 );
  }

  return sqrt( contador );
}

/* Función que calcula la distancia entre dos individuos dadas sus cadenas 
   binarias */
double HumpFunction::distance( char *firstVector, char *secondVector ) {
  double difference = 0;
  char fbinary[_dimension * _nVariables],sbinary[_dimension*_nVariables];
  
  this->inverseGrayVector(firstVector,fbinary);
  this->inverseGrayVector(secondVector,sbinary);
  
  double vector1[_nVariables];
  binaryToDoubleVector(fbinary, vector1);
  
  double vector2[_nVariables];
  binaryToDoubleVector(sbinary, vector2);

  return this->distance(vector1, vector2);
}

/* Función usada para pasar de código gray a binario, teniendo en cuenta que
   estamos trabajando con más de una variable */
void HumpFunction::inverseGrayVector(char *gray, char *binary){

   for(int i = 0; i < _nVariables;i++){
   
   binary[i*_dimension] = gray[i*_dimension];
    
    for(int j = 1; j < _dimension; j++){
      binary[(i*_dimension)+j] =
        (binary[((i*_dimension)+j)-1] +
         gray[(i*_dimension)+j])%2;
    }
  }
}
