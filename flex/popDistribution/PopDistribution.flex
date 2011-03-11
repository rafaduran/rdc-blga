%{
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

int *individuos_totales;
double *distancia_total;


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
void cargaPicos(char *filename);

void resumen();

char MULTI_PEAKS_FILES = 1;

ofstream out;
%}

DIGIT [0-9]
INTEGER {DIGIT}+
NUMBER -?(({INTEGER}(("."{INTEGER})|".")?)|("."{INTEGER}))((e|E)(\+|-)?{INTEGER})?

EMPTYCELL \"\ \"
CELLNUMBER \"([^\"]*[ \t\'])?{NUMBER}+[^\"]*\"
CELL  \"[^\"]+\"

%%

{NUMBER}:{NUMBER} {
   out << yytext << endl;
   char *pchr;
   pchr=strchr(yytext,':');
   pchr='\0';
   currentRun = atoi(yytext);
   if(MULTI_PEAKS_FILES)
      cargaPicos(cabeceraFPicos, currentRun);
}

-+ {}

^{NUMBER}   {currentDimension=0;}
{NUMBER} {
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
         // Aquí imprimimos el pico más cercano y la distancia
         //out << imin << " " << dmin << endl;
         distancia_total[imin] += dmin;
         individuos_totales[imin]++;
      }
}

Run      {
         if (primeralinea == 0){
            int suma = 0;
            for (int i = 0; i < numPicos; i++)
               suma += picosObtenidos[i];

            sumaGlobal += suma;
            numRuns++;

            for (int i = 0; i < numPicos; i++)
               picosObtenidos[i]=0;

            // Mostramos individuos en cada pico y distancia media

            resumen();
            for(int i = 0; i < numPicos; i++) {
               individuos_totales[i] = 0;
               distancia_total[i] = 0;
            }
         }

         primeralinea = 0;
}

\n {}
%%

void cargaPicos(char *filename, int numRun){
   ostringstream aux;
   aux << filename << numRun;
   ifstream entrada(aux.str().data(), ios::in);

   for (int i = 0; i < numPicos; i++){

      for (int j = 0; j < dimension; j++){
         entrada >> picos[i][j];
      }
   }

   entrada.close();
}

void cargaPicos(char *filename){
   ifstream entrada(filename, ios::in);

   for (int i = 0; i < numPicos; i++){

      for (int j = 0; j < dimension; j++){
         entrada >> picos[i][j];
      }
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

void resumen(){
   out << "Resumen para ejecución " << currentRun << endl;
   for(int i = 0; i < numPicos; i++){
      out << i << " " << individuos_totales[i] << "\t";
      out << "Distancia media: " << distancia_total[i]/individuos_totales[i] << endl;
   }

}

main(int argc, char **argv){

   if (argc < 6 || argc > 8){
      cout << argv[0] << " <precisión> <numPicos> <dimension>" <<
         " <[-]cabeceraFicherosPicos> <ficheroResultados> [<fichero_salida>]" <<
         endl;
      cout << "\t<dimension> es el número de variables reales" << endl;
      cout << "\t<[-]cabeceraFicherosPicos> si la cabecera comienza por guión "
         << " se considerá un único fichero de picos para todas las ejecuciones"
         << ", sino se buscará\n\t un fichero de picos para cada ejecución."
         << endl;
         return 0;
   } else if(argc == 6){
      out.open("distribution.txt",ios::app);
   } else {
      out.open(argv[6],ios::app);
   }

   out.setf(ios::scientific,ios::floatfield);
   out.precision(15);


   precision = atof(argv[1]);
   numPicos = atoi(argv[2]);
   dimension = atoi(argv[3]);
   picos = new double*[numPicos];

   for (int i = 0; i < numPicos; i++)
      picos[i] = new double[dimension];

   individuos_totales = new int[numPicos];
   distancia_total = new double[numPicos];

   for(int i = 0; i < numPicos; i++){
      individuos_totales[i] = 0;
      distancia_total[i] = 0;
   }

   cabeceraFPicos = argv[4];

   if(cabeceraFPicos[0] == '-'){
      MULTI_PEAKS_FILES = 0;
      cargaPicos(cabeceraFPicos+1);
   }

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
   resumen();
   int suma = 0;

   for (int i = 0; i < numPicos; i++)
      suma += picosObtenidos[i];

   sumaGlobal += suma;
   numRuns++;

   for (int i = 0; i < numPicos; i++)
      delete [] picos[i];

   delete [] currentSol;
   delete [] picos;
   delete [] picosObtenidos;
   delete [] distancia_total;
   delete [] individuos_totales;

   fclose(yyin);
   out.close();
   return 0;
}

int yywrap(){}
