%{
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
#include <iostream>
#include <fstream>
#define abs(x) (((x)<0)?(-(x)):(x))

using namespace std;

/*********************************/
//MODIFICAR PARA P2
int numPicos=5;
double picos[]={0.1,0.3,0.5,0.7,0.9};
/************************************/

int primeralinea;
double precision;
int *picosObtenidos;
double sumaGlobal;
int numRuns;

ofstream out("archivo",ios::app);

%}

DIGIT [0-9]
INTEGER {DIGIT}+
NUMBER -?(({INTEGER}(("."{INTEGER})|".")?)|("."{INTEGER}))((e|E)(\+|-)?{INTEGER})?

EMPTYCELL \"\ \"
CELLNUMBER \"([^\"]*[ \t\'])?{NUMBER}+[^\"]*\"
CELL	\"[^\"]+\"

%%

^{NUMBER}	{}
{NUMBER}$	{
		double value = atof(yytext);
	
		for (int i = 0; i < numPicos; i++){
			
			if (abs(value - picos[i]) <= precision)
				picosObtenidos[i] = 1;
		}
}

Run" "{NUMBER}:{NUMBER}-+		{
			
			if (primeralinea == 0){
				int suma = 0;
		
				for (int i = 0; i < numPicos; i++)
					suma += picosObtenidos[i];
					
				out << endl << "\t" << suma << " picos obtenidos" << endl;
				sumaGlobal += suma;
				numRuns++;
				
				for (int i = 0; i < numPicos; i++)
					picosObtenidos[i]=0;
			}
			
			out << yytext;
			primeralinea = 0;
}

\n	{}
%%

main(int argc, char **argv){

	if (argc < 3){
		cout << argv[0] << " <precisión> <fichero>" << endl;
		return 0;
	}
	
	precision = atof(argv[1]);
	char *filename = argv[2];
	yyin = fopen(filename, "r");
	primeralinea = 1;
	sumaGlobal=0.;
	numRuns = 0;
	picosObtenidos = new int[numPicos];
	
	for (int i = 0; i < numPicos; i++)
		picosObtenidos[i]=0;

	yylex();
	
	//LECTURA DE LA ÚLTIMA EJECUCIÓN
	int suma = 0;
		
	for (int i = 0; i < numPicos; i++)
		suma += picosObtenidos[i];
					
	out << endl << "\t" << suma << " picos obtenidos" << endl;
	sumaGlobal += suma;
	numRuns++;

	
	out << endl << "Media de picos obtenidos: " << (sumaGlobal / numRuns) << endl;
	delete [] picosObtenidos;
	fclose(yyin);
  out.close();
	return 0;
}

int yywrap(){}
