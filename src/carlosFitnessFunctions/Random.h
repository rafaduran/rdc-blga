/*******************************************************************/
/*  Rand genera un numero real pseudoaleatorio entre 0 y 1,        */
/*  excluyendo el 1.			                                        */
/*  Randint genera un numero entero entre low y high, ambos 	    */
/*  incluidos.	   						                               */
/*  Randfloat genera un numero real entre low y high, incluido low */
/*  y no incluido high.                                            */
/*  RandPerm genera un permutación aleatoria de un vector entero,  */
/*  de tamaño n.                                                   */
/*******************************************************************/

#ifndef __RANDOM_H__
#define __RANDOM_H__

class Random {
    
    const static long MASK = 2147483647;
    const static long PRIME = 65539;
    const static double SCALE;
    long Seed;
    
    public:

		//! Constructor
    Random(){
        Seed = 123456789;
		//double SCALE = 0.4656612875e-9;
	}
    
     void cambiaSemilla(double semillaNueva){
        Seed = (long)semillaNueva;
    }
    
     long getSeed(){
        return Seed;
    }
    
     double Rand(){
		 Seed = (Seed * PRIME) & MASK;
		 double value = Random::SCALE * ((double) Seed);
		 return ( value );
    }
    
     int Randint(int low, int high){
        return ( (int) (low + (high-(low)+1) * Rand()));
    }
    
     long RandULong(long low, long high){
        return ( (long) (low + (high-(low)+1) * Rand()));
    }
    
     double Randfloat(double low, double high){
        return ( (low + (high-(low))*Rand()));
    }
    
     int* RandPerm(int *perm, int n){
        
        int i, top;
        top = n - 1;
        
        for (i = 0; i < n; i++)
            perm[i] = i;
        
        for (i = 0; i < n; i++){
            int change = this->Randint(0, top);
            int aux = perm[change];
            perm[change] = perm[i];
            perm[i] = aux;
        }
        
        return (perm);
    }
};

#endif
