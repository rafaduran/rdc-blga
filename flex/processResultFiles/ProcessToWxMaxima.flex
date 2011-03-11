%{
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
#include <iostream>
#include <fstream>
#define abs(x) (((x)<0)?(-(x)):(x))

using namespace std;

double aux;

int primeralinea;
double precision;
int *picosObtenidos;
double sumaGlobal;
int numRuns;

ofstream out("salida",ios::app);

%}

DIGIT [0-9]
INTEGER {DIGIT}+
NUMBER -?(({INTEGER}(("."{INTEGER})|".")?)|("."{INTEGER}))((e|E)(\+|-)?{INTEGER})?

EMPTYCELL \"\ \"
CELLNUMBER \"([^\"]*[ \t\'])?{NUMBER}+[^\"]*\"
CELL	\"[^\"]+\"

%%

^{NUMBER}	{
  aux = atof(yytext);
}

{NUMBER}$	{
  double value = atof(yytext);

  out << "[" << value << "," << aux << "],";
}

Run" "{NUMBER}:{NUMBER}-+		{
  out << "]" << endl;

  out << yytext << endl;

  out << "xy:[";
}

\n	{}
%%

main(int argc, char **argv){

	if (argc < 2){
		cout << argv[0] << " <fichero>" << endl;
		return 0;
	}

  out.precision(15);
	
	char *filename = argv[1];
	yyin = fopen(filename, "r");
	primeralinea = 1;
	
	yylex();
	
	//LECTURA DE LA ÚLTIMA EJECUCIÓN
  out << "]" << endl;	

	fclose(yyin);
  out.close();
	return 0;
}

int yywrap(){}
