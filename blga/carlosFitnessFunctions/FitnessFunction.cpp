#include <cstdlib>
#include <cstring>

#include "FitnessFunction.h"
#include "Random.h"

#include "PeriodicEqual.h"
#include "PeriodicUnequal.h"
#include "HumpFunction.h"

FitnessFunction * FitnessFunction::ff = NULL;

// Función encargada de crear las todas las funciones de fitness incluidas
FitnessFunction* FitnessFunction::getFitnessFunction(int i, const char* path,
		int numRun){
    
   if (ff != NULL)
      delete ff;
    
   ff = NULL;
    
   switch (i){
      case 0: ff = new PeriodicEqual(P1_P2_dimension); break;
      case 1: ff = new PeriodicUnequal(P1_P2_dimension); break;
      case 2: ff = getHumpFunction(numRun, path); break;
      // Pafa HumpFunction usamos una función propia
   }
   return ff;
}

/* Función encargada de crear los objetos HumpFunction. Para ello abrirá el
   fichero de configuración que debe encontrarse junto al ejecutable */
FitnessFunction* FitnessFunction::getHumpFunction(int numRun,
		const char* path) {
	ifstream in;
	string ss(path);

	ss.append("humpFunctionConfig");
	in.open(ss.c_str() ,ifstream::in);

	if(in.fail()){
		cout << "Error while opening config file: "
				<< ss.c_str() << endl;
		return NULL;
	}
    
	char aux[250], filename[250];
    
	in >> filename;
    
	sprintf(aux,"%d",numRun);
    
	ss.clear();
	ss.append(path);
	ss.append(filename);
	ss.append(aux);
    
	int nvar, kpeaks, dimension;
	double radius, alpha, height;
	 
	// Obtención de parámetros
	in >> nvar;
	in >> kpeaks;
	in >> radius;
	in >> alpha;
	in >> height;
	in >> dimension;
   
    
	return new HumpFunction(ss.c_str(), nvar, kpeaks, radius, alpha,
			height, dimension);
}

// Paso de código gray a binario
void FitnessFunction::inverseGray(char *gray,char *binary) {
  binary[0] = gray[0];
  
  for(int i = 1; i < _dimension; i++) {
    binary[i]=(binary[i-1]+gray[i])%2;
  }
}


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
