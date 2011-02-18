%{
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#define abs(x) (((x)<0)?(-(x)):(x))

using namespace std;

int numPicos;
int dimension;
double **picos;
int *picosObtenidos;

double *currentSol;
int currentDimension;

int primeralinea;
double precision;
double sumaGlobal;
int numRuns;
int currentRun;
char *cabeceraFPicos;

double distancia(double *p1, double *p2);
void cargaPicos(char *filename, int numRun);

//ofstream out("archivo",ios::app);
ofstream nichos("nichos.txt",ios::app);
%}

DIGIT [0-9]
INTEGER {DIGIT}+
NUMBER -?(({INTEGER}(("."{INTEGER})|".")?)|("."{INTEGER}))((e|E)(\+|-)?{INTEGER})?

EMPTYCELL \"\ \"
CELLNUMBER \"([^\"]*[ \t\'])?{NUMBER}+[^\"]*\"
CELL	\"[^\"]+\"

%%

{NUMBER}:{NUMBER}	{
	//out << yytext;
	nichos << yytext << endl;
	char *pchr;
	pchr=strchr(yytext,':');
	pchr='\0';
	currentRun = atoi(yytext);
  //cout << currentRun << endl;
	cargaPicos(cabeceraFPicos, currentRun);
}

-+ {}
		
^{NUMBER}	{currentDimension=0;}
{NUMBER}	{
    //cout << "numero" << endl;
		currentSol[currentDimension] = atof(yytext);
		currentDimension++;
		
		if(currentDimension == dimension){
			double daux, dmin = distancia(currentSol, picos[0]);
	      int imin = 0;
			for (int i = 1; i < numPicos; i++){
				daux=distancia(currentSol, picos[i]);
				if (daux < dmin){
					imin = i;
					dmin = daux;
                }
			}
			nichos << imin << " " << dmin << endl;
		}
}

Run		{
			
			if (primeralinea == 0){
				int suma = 0;
		
				for (int i = 0; i < numPicos; i++)
					suma += picosObtenidos[i];
					
				//out << endl << "\t" << suma << " picos obtenidos" << endl;
				sumaGlobal += suma;
				numRuns++;
				
				for (int i = 0; i < numPicos; i++)
					picosObtenidos[i]=0;
			}
			
			//cout << "Run" << endl;
			primeralinea = 0;
}

\n	{}
%%

void cargaPicos(char *filename, int numRun){
	ostringstream aux;
	aux << filename << numRun;
	ifstream entrada(aux.str().data(), ios::in);
	
	for (int i = 0; i < numPicos; i++){

		for (int j = 0; j < dimension; j++){
			entrada >> picos[i][j];
			//cout << picos[i][j] << ",";
		}
		
		//cout << endl;
	}
	
	entrada.close();
}

double distancia(double *p1, double *p2){
	
	double sumaCoordenadas = 0.;
	
	for (int i = 0; i < dimension; i++){
		double unaCoordenada = p1[i] - p2[i];
		sumaCoordenadas += (unaCoordenada * unaCoordenada);
	}
	
	return (sqrt(sumaCoordenadas));
}

main(int argc, char **argv){

	if (argc < 6){
		cout << argv[0] << " <precisión> <numPicos> <dimension> <cabeceraFicherosPicos> <ficheroResultados>" << endl;
		cout << "\t<dimension> es el número de variables reales" << endl;
		return 0;
	}

  //cout << "algo" << endl;
	
  

	precision = atof(argv[1]);
	numPicos = atoi(argv[2]);
	dimension = atoi(argv[3]);
	picos = new double*[numPicos];
	
	for (int i = 0; i < numPicos; i++)
		picos[i] = new double[dimension];
	
	cabeceraFPicos = argv[4];
	char *fileResults = argv[5];
	yyin = fopen(fileResults, "r");
	primeralinea = 1;
	sumaGlobal=0.;
	numRuns = 0;
	currentSol = new double[dimension];
	picosObtenidos = new int[numPicos];
	
	for (int i = 0; i < numPicos; i++)
		picosObtenidos[i]=0;

	yylex();
	
	//LECTURA DE LA ÚLTIMA EJECUCIÓN
	int suma = 0;
		
	for (int i = 0; i < numPicos; i++)
		suma += picosObtenidos[i];
					
	//out << endl << suma << " picos obtenidos" << endl;
	sumaGlobal += suma;
	numRuns++;

	//out << endl << "Media de picos obtenidos: " << (sumaGlobal / numRuns) << endl;
	
	for (int i = 0; i < numPicos; i++)
		delete [] picos[i];
	
	delete [] currentSol;
	delete [] picos;
	delete [] picosObtenidos;
	fclose(yyin);
        //out.close();
        nichos.close();
	return 0;
}

int yywrap(){}
