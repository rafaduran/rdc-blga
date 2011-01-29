#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <Random.h>
#include <math.h>

int semillas[] = {
 12345678, 23456781, 34567812, 45678123, 56781234, 67812345, 78123456, 81234567,
 12435678, 24356781, 43567812, 35678124, 56781243, 67812435, 78124356, 81243567,
 18435672, 84356721, 43567218, 35672184, 56721843, 67218435, 72184356, 21843567,
 18437652, 84376521, 43765218, 37652184, 76521843, 65218437, 52184376, 21843765,
 18473652, 84736521, 47365218, 73652184, 36521847, 65218473, 52184736, 21847365,
 15473682, 54736821, 47368215, 73682154, 36821547, 68215473, 82154736, 21547368,
 15472683, 54726831, 47268315, 72683154, 26831547, 68315472, 83154726, 31547268,
 65472183, 54721836, 47218365, 72183654, 21836547, 18365472, 83654721, 36547218,
 35472186, 54721863, 47218635, 72186354, 21863547, 18635472, 86354721, 63547218,
 35427186, 54271863, 42718635, 27186354, 71863542, 18635427, 86354271, 63542718,
 36427185, 64271853, 42718536, 27185364, 71853642, 18536427, 85364271, 53642718,
 36428175, 64281753, 42817536, 28175364, 81753642, 17536428, 75364281, 53642817,
 36528174, 65281743, 52817436, 28174365};

void newPeakFile(char *filename,int run, int nVariables, int nPeaks, double
   radius, double height, double alpha);
   
double distance(double *peak1, double *peak2, int nVariables);

using namespace std;

int main(int argc, char **argv) {
    if(argc < 8){
       cout << "Wrong arguments: main filename numRuns nVariables nPeaks radius"
            << " height alpha" << endl;
       return 0;
    }

    int nVariables, nPeaks, dimension, numRuns;
    double radius, height, alpha;
    
    char *filename;
    
    filename = argv[1];
    numRuns = atoi(argv[2]);
    nVariables = atoi(argv[3]);
    nPeaks = atoi(argv[4]);
    radius = atof(argv[5]);
    height = atof(argv[6]);
    alpha = atof(argv[7]);
    //dimension = atoi(argv[8]);
    
    for(int i = 0; i < numRuns; i++){
      newPeakFile(filename, i, nVariables, nPeaks, radius, height, alpha);
    }
}

void newPeakFile(char *filename,int run, int nVariables, int nPeaks, double
   radius, double height, double alpha){
   char *str = new char[strlen(filename)+2];
   double **peaks = new double * [nPeaks];
   Random *random = new Random();
   
   random->cambiaSemilla(semillas[run]);
   
   for(int i = 0; i < nPeaks; i++)
      peaks[i] = new double[nVariables];

   sprintf(str,"%s%d",filename,run);
   
   // Genero nPeaks picos aleatorios
   for (int i = 0; i < nPeaks; i++ ) {
      for (int j = 0; j < nVariables; j++ ) {
         peaks[ i ][ j ] = random->Rand();
      }
   }

   // Compruebo que ningún pico este dentro del radio de otro
   for (int i = 0; i < nPeaks-1; i++ ) {
      for (int j = i + 1; j < nPeaks; j++ ) {
         if (distance( peaks[ i ], peaks[ j ], nVariables ) < 1.5 * radius ) {
         /*En caso que el pico j este dentro del radio
           del pico i, reubicamos el pico j hasta que este fuera*/
            int OK = 0;

            while ( !OK ) {
               for (int l = 0; l < nVariables; l++ ) {
                  peaks[ j ][ l ] = random->Rand();
               }
               OK = 1;
               for(int k = 0; k <= i; k++){
                  if (distance( peaks[ k ], peaks[ j ], nVariables) < 1.5 *   
                        radius ) {
                     OK = 0;
                     break;
                  }
               }
            }
         }
      }
   }
   
   //Guardamos los resultados en un fichero
   ofstream fs( str );
    
   fs.precision(15);  
   fs.setf(ios::scientific,ios::floatfield);
    
   for (int i = 0; i < nPeaks; i++ ) {
      for (int j = 0; j < nVariables; j++ ) {
         fs << peaks[ i ][ j ] << endl;
      }   
   }
  
  fs.close();
}

double distance(double *peak1, double *peak2, int nVariables){
  double contador = 0;
  
  for ( int i = 0; i < nVariables; i++ ) {
    contador += (( peak1[ i ] - peak2[ i ]) * ( peak1[ i ] - peak2[ i ]));
  }

  return sqrt( contador );
}