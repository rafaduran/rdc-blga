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
/*double picos[] = {0.929286856805789, 0.678555051869995, 0.449095488342517,
                  0.246046176199754, 0.0795972277772368};*/
double picos[] = {0.1588652193402821, 0.3782075882955098, 
                  0.6576467790632494, 0.8860331603610117, 0.9895475847600835};
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
CELL  \"[^\"]+\"

%%

{NUMBER}$ {}
^{NUMBER} {
    double value = atof(yytext);
  
    for (int i = 0; i < numPicos; i++){
      
      if (abs(value - picos[i]) <= precision){
        picosObtenidos[i] = 1;
        //cout << value << endl;
      }
    }
}

Run" "{NUMBER}:{NUMBER}-+   {
      
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

\n  {}
%%

main(int argc, char **argv){

  if (argc < 3){
    cout << argv[0] << " <precisión> <fichero>" << endl;
    return 0;
  }
  cout.precision(15);
  
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
