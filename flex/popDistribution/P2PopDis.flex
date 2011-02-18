%{
//#include <stdio.h>
//#include <string.h>
//#include <math.h>
#include <iostream>
#include <fstream>
#define abs(x) (((x)<0)?(-(x)):(x))

using namespace std;

double aux;
double picos[]={0.929286856805789,0.678555051869995,0.449095488342517,0.246046176199754,0.0795972277772368};

int p0 = 0;
int p1 = 0;
int p2 = 0;
int p3 = 0;
int p4 = 0;

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

^{NUMBER}" "	{
}

{NUMBER}$	{
   double value = atof(yytext);
   
   int index = 0;
   double dis = abs(value - picos[0]);
   
   for(int i = 1; i < 5; i++){
      if(abs(value - picos[i]) < dis){
         index = i;
      }
   }
   
   switch(index){
      case 0: p0++;break;
      case 1: p1++;break;
      case 2: p2++;break;
      case 3: p3++;break;
      case 4: p4++;break;
   };
   
}

\n	{}
%%

main(int argc, char **argv){

	if (argc < 2){
		cout << argv[0] << " <fichero>" << endl;
		return 0;
	}
	char *filename = argv[1];
  	yyin = fopen(filename, "r");
	yylex();

   cout << p0 << endl << p1 << endl << p2 << endl << p3 << endl << p4 << endl;
	
	return 0;
}

int yywrap(){}
