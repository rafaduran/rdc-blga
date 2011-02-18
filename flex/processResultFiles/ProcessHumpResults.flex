%{
#include <stdio.h>
//#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#define abs(x) (((x)<0)?(-(x)):(x))

using namespace std;

int aux;
double suma = 0;
double sumaCuadrados = 0;
int cont = 0;

int primeralinea;
double precision;
int *picosObtenidos;
double sumaGlobal;
int numRuns;
double media = 0;
int iteracion;

//ofstream out("medias",ios::app);

%}
FIN --------------------------------------------------------------------
DIGIT [0-9]
INTEGER {DIGIT}+
NUMBER -?(({INTEGER}(("."{INTEGER})|".")?)|("."{INTEGER}))((e|E)(\+|-)?{INTEGER})?

EMPTYCELL \"\ \"
CELLNUMBER \"([^\"]*[ \t\'])?{NUMBER}+[^\"]*\"
CELL	\"[^\"]+\"

%%

:{NUMBER}	{
  //yytext[strlen(yytext)-1] = '\0';

  aux = atof(yytext+1);
  //cout << endl << aux << endl;
  //cout << endl << "LLEGA KKKKKKKKKKKKKKKKKKKKKK" << endl;
}

^{NUMBER} {}

{INTEGER} {
  //cout << endl << "LLEGA HHHHHHHHHHHHHHHHHHHHH" << endl;
  if(aux == iteracion){
    
    suma+=atof(yytext);
    sumaCuadrados+=pow(atof(yytext)-media,2);
    cont++;
    //cout << endl << atof(yytext) << "llega " << cont;
  }
}

"Media de picos obtenidos":\ {NUMBER} {}

" picos obtenidos" {}

\n	{}
%%

main(int argc, char **argv){

	if (argc < 3){
		cout << argv[0] << " <fichero> <iteracion>"<< endl;
		return 0;
	}
  iteracion = atoi(argv[2]);

  //cout << endl << "iteracion " << iteracion << endl;
  
  if(argc == 4)
    media = atof(argv[3]);

    cout.precision(15);

	yyin = fopen(argv[1], "r");
	primeralinea = 1;
	
	yylex();
	
	//LECTURA DE LA ÚLTIMA EJECUCIÓN
  media = suma / cont;

  double desviacion = sqrt(sumaCuadrados/double(cont-1));

  cout << sumaCuadrados << endl;

  cout << "Media de los " << cont << " elementos: " << media << endl;
  
  if(argc == 4)
    cout << "Desviación estandar: " << desviacion << endl;

  	

	fclose(yyin);
  //out.close();
	return 0;
}

int yywrap(){}
