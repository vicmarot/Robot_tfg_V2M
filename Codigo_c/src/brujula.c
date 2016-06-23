/*
 * File: main.c
 *
 * MATLAB Coder version            : 2.8
 * C/C++ source code generated on  : 18-May-2016 19:47:37
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/* To compile this program u need to do "gcc main. c -c"  and "gcc *.o   */
 /* -o main -lm -lwirinPi"                                               */
/*************************************************************************/
/* Include Files */
#include "rt_nonfinite.h"
#include "occupancy.h"
#include "main.h"
#include "occupancy_terminate.h"
#include "occupancy_initialize.h"

#include "unistd.h"
#include "stdio.h"
#include "Comunicacion.h"

#define MAXDATASIZE 64
#define NUM_POS 5
#define PI  3.14


int fd=200; //Unica variable global que sera el identificador del puerto
int Pos_Torreta[NUM_POS];
float Angl_Torreta[NUM_POS]; // la posicion inicial de la torre es mirando al frete

  


/* Function Declarations */
static void argInit_1x3_real_T(double result[3]);
static void argInit_500x500_real_T(double result[250000]);
static double argInit_real_T(void);
static void Init_Var_torre();
static void Emitir(double *Resultado_medida1, double *Resultado_medida2);
static void Com_Rasp_to_Ard(char *str[MAXDATASIZE]);
static void main_occupancy(void);
static void delay ();
static void Busca_Pared();
static void Acercamiento( double *d, double Angl_Rob, double Pos[3], unsigned int destinoX, unsigned int destinoY);
static void Exploracion(double Pos[3], double lold[250000], double lnew[250000], double Angl_Rob);
static void Giro (double alfa, double Angl_Rob, double d);
static void avanzar (double d);

/* Function Definitions */

/*
 * Arguments    : double result[3]
 * Return Type  : void
 */
static void argInit_1x3_real_T(double result[3])
{
  int b_j1;

  /* Loop over the array to initialize each element. */
  for (b_j1 = 0; b_j1 < 3; b_j1++) {
    /* Set the value of the array element.
       Change this value to the value that the application requires. */
    result[b_j1] = argInit_real_T();
  }
}

/*
 * Arguments    : double result[250000]
 * Return Type  : void
 */
static void argInit_500x500_real_T(double result[250000])
{
  int b_j0;
  int b_j1;

  /* Loop over the array to initialize each element. */
  for (b_j0 = 0; b_j0 < 500; b_j0++) {
    for (b_j1 = 0; b_j1 < 500; b_j1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result[b_j0 + 500 * b_j1] = 0.5;
    }
  }
}

/*
 * Arguments    : void
 * Return Type  : double
 */
static double argInit_real_T(void)
{
  return 0.0;
}


/* fuciones creadas por Maroto */
static void Init_Var_torre(){
  int jf1;
  Angl_Torreta[0]=0;
for (jf1=0;jf1<NUM_POS;jf1++){ // inicializacion de las posicoon y angulos de la torreta
  Pos_Torreta[jf1]=jf1+1;
  if (jf1!=0) // en la primera posicion no tiene que inicializarlo ya esta hecho
    Angl_Torreta[jf1]=Angl_Torreta[jf1-1]+PI/4;// Radianes
}
}

static void Emitir(double *Resultado_medida1, double *Resultado_medida2){ // en esta funcion debemos indicarle al arduino que comience a medir 
  //y obtener el resultado por comunicacion serie
  char str[MAXDATASIZE];
  char buf[MAXDATASIZE];
  double dato[2];
  int i=0;
  memset (str, '\0', MAXDATASIZE); // limpio str
  strcpy (str,"e*");
  writen (fd,str);
  memset (buf, '\0', MAXDATASIZE); // limpio buf
  serialport_read_until (fd, buf, 'K');
  //printf ("READ--> %s\n",buf );
  char * pch;
  pch = strtok (buf,"|K");
  while (pch != NULL)
  {
    dato[i]=atof(pch);
    pch = strtok (NULL, "|K");
    i++;
  }
  // Datos a devolver
  *Resultado_medida1=dato[0];
  *Resultado_medida2=dato[1];
}

static void Com_Rasp_to_Ard(char *str[MAXDATASIZE]){

  char buf[MAXDATASIZE];
  writen (fd,str);
  memset (buf, '\0', MAXDATASIZE); // limpio buf
  serialport_read_until (fd, buf, 'K');
  printf ("READ--> %s\n",buf );
  /*char * pch;
  pch = strtok (buf,"|K");
  while (pch != NULL)
  {
   // printf ("%s\n",pch);
    dato[i]=atof(pch);
    pch = strtok (NULL, "|K");
    i++;
  }
  printf("%s\n", );*/
}

static void delay (){ 
 int c = 1, d = 1;

 for ( c = 1 ; c <= 32767/4 ; c++ )
     for ( d = 1 ; d <= 32767/4 ; d++ ){
    }
}

/*static void Scaneo (double Pos_Torreta[7], double Pos[2],double ang_robot, double Zt, double lold[250000], double lnew[250000]){
  int if1;
  for (if1=0;if1<7;if1++){
    Mueve_Torre(Pos_Torreta[if1]);// funcion que mueve la torreta
    Pos[2]=Pos_Torreta-ang_robot; // actulizo la posicion de la torreta respecto la del robot
    occupancy(Pos, Zt, lold, lnew); // calculo las casiilas ocupadas
  } // mientras haga este for solo muestrara no se movera
}*/

static void Busca_Pared(double lnew[250000], double Pos[3],unsigned int *destinoX, unsigned int *destinoY, double *distancia){
  int Yi,Xi,jf1,j=0,jf2,jf3;
  int PosDestinoX[250000],PosDestinoY[250000],tempi;
  double d[250000],tempd;
  for (jf1=0;jf1<250000;jf1++){
    PosDestinoX[jf1]=-1; // inicializo a -1 para despues descartar los que no escriba
    PosDestinoY[jf1]=-1;
    d[jf1]=9999; // distancia infinito
  }
  for(Xi=0;Xi<500;Xi++){
    for (Yi=0;Yi<500;Yi++){
      if(lnew[Yi+500*Xi]>0.5){
        PosDestinoX[j]=Xi;
        PosDestinoY[j]=Yi;
        j++; // j seran el numero de destino posibles totales
      }
    }
  }
  for (jf2=0;jf2<j;jf2++){
    d[jf2]=sqrt(pow((PosDestinoX[jf2]-Pos[0]),2)+pow((PosDestinoY[jf2]-Pos[1]),2));
   //printf("%lf\n",d[jf2] );
  }
  for (jf2=0;jf2<j;jf2++){ // los ordeno de menor a mayor
    for (jf3=0;jf3<j-1;jf3++){
      if(d[jf3]>d[jf3+1]){
         tempd = d[jf3];
         d[jf3] = d[jf3+1];
         d[jf3+1] = tempd;  
         tempi=PosDestinoX[jf3];
         PosDestinoX[jf3]=PosDestinoX[jf3+1];
         PosDestinoX[jf3+1]=tempi;
         tempi=PosDestinoY[jf3];
         PosDestinoY[jf3]=PosDestinoY[jf3+1];
         PosDestinoY[jf3+1]=tempi;
      }
    }
  }
  //printf("destino x:%i destino y:%i d:%lf\n",PosDestinoX[0],PosDestinoY[0],d[0] );
  *destinoX=PosDestinoX[0]; *destinoY=PosDestinoY[0]; *distancia=d[0];
}

static void Acercamiento( double *d, double Angl_Rob, double Pos[3], unsigned int destinoX, unsigned int destinoY){
  double alfa;
  *d=sqrt(pow((destinoX-Pos[0]),2)+pow((destinoY-Pos[1]),2));
  alfa=((destinoX-Pos[0])*cos(Angl_Rob)+(destinoY-Pos[1])*sin(Angl_Rob))/(*d);//calculamos alfa
  Angl_Rob=-acos(alfa)+Angl_Rob; //no tengo claros los signo ya que en la simulacion el eje y va hacia abajo y el z hacia la derecha para que fuera destrogiro habia que meter un menos
  if (destinoX-Pos[0]>0) // es positivo
    Pos[0]=Pos[0]+abs(destinoX-Pos[0])/4;
  else
    Pos[0]=Pos[0]-abs(destinoX-Pos[0])/4;
  if (destinoY-Pos[1]>0) // es positivo
    Pos[1]=Pos[1]+abs(destinoY-Pos[1])/4;
  else
    Pos[1]=Pos[1]-abs(destinoY-Pos[1])/4;
  printf("PDerecha=>%lf PIZQ=>%lf\n",Pos[0],Pos[1] );
}
/* Posiciones de la torreta respecto al robot. 3=>mirando al frente
                 90º pi/2
                    3
                   | |
      135º 3*pi/4 4   2 45º pi/4
                 |     |
    180º pi/4   5-------1 0º 0
                 |     |
                  |   |
                   | | 
                    |
*/
static void Exploracion(double Pos[3], double lold[250000], double lnew[250000], double Angl_Rob){
  int jf2,jf3;
  double Zt1, Zt2;
  for (jf2=0;jf2<NUM_POS;jf2++){ //sus 2 por NUM_POS
    Mover_Servo(Pos_Torreta[jf2]);
    Emitir(&Zt1,&Zt2); // manda una señal al arduino para que emita y este le devuleve los valores en cm por lo que al multiplicarlo por 100 para pasarlo a casillas me da el valor dado por el arduino
    Pos[2]=Angl_Torreta[jf2]-Angl_Rob; //radianes
    //Pos[0]=Pos[0]+25;
    //printf("angulo=>%lf\n",Angl_Torreta[jf2] );
    //occupancy(Pos, Zt1, lold, lnew); // pos sera la posicion del robot mas o menos la distancia del sensor dependiendo si es izquierda o derecha
    occupancy(Pos, Zt1, lnew, lnew); // pos sera la posicion del robot mas o menos la distancia del sensor dependiendo si es izquierda o derecha
    for (jf3=0;jf3<25000;jf3++){
    lold[jf3]=lnew[jf3];
    }
    //falta un occupancy para el segundo sensor y en el aterior restarle en la posicion correspondiente la difrencia de distancia en x e y
  }

}

static void Giro (double alfa, double Angl_Rob, double d){
  char str[MAXDATASIZE];
  char aux[MAXDATASIZE]; // cambia este tamaño
  float L_Giro=0, R_robot=14.5;//cm
  printf("el angulo de l robot es:%lf\n",Angl_Rob );
  L_Giro=abs(Angl_Rob*R_robot); // la distancia sera la misma para ambos casos
  if (alfa>0 && acos(alfa)!=0){
    sprintf(aux,"I%f*",L_Giro);
    printf("EL DATO ES==>%s\n",aux );
    strcpy(str, aux);
    Com_Rasp_to_Ard(&str[MAXDATASIZE]);
  }
  else if (alfa<0 && acos(alfa)!=0){
   sprintf(aux,"D%f*",L_Giro);
   printf("EL DATO ES==>%s\n",aux );
   strcpy(str, aux);
   Com_Rasp_to_Ard(&str[MAXDATASIZE]);
  }
  else {
    printf("No tengo que girar\n");
    avanzar(d);
  }
}

static void avanzar (double d){
  char str[MAXDATASIZE];
  char aux[MAXDATASIZE]; // cambia este tamaño
  sprintf(aux,"I%lf*",d);
  printf("EL DATO ES==>%s\n",aux );
  strcpy(str, aux);
  Com_Rasp_to_Ard(&str[MAXDATASIZE]);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;


  int if3,jf3;
  unsigned int destinoX=0, destinoY=0, Dist_Seg=20;  
  double d=0;
  int jf2,jf1;
  double Pos[3],Angl_Rob=0, Zt1, Zt2, alfa, Dd, Di; // pos[0]=>posicion x del robot. pos[1]=> posicion y del robot. pos[2]=> beta-angulo del robot
  static double lold[250000];
  static double lnew[250000];
  float angle_i2c;
  Init_Var_torre();
  Init_WiringPi();
  /* inicializacion de variables y posiciones del robot */
  fd=Open_port (fd); // inicializo la comunicacion entre el arduino y la raspberry pi
  argInit_1x3_real_T(Pos); // inicializacion de variables a 0
  argInit_500x500_real_T(lold);
  argInit_500x500_real_T(lnew);
  occupancy_initialize();
  Mover_Servo(Pos_Torreta[3]); //inicializamos torreta a la posicion 3 (la torre se encuentra mirando al frente).




  FILE *fichero;

  fichero=fopen("Datos_l.m", "w+");
  if( fichero )
      printf( "creado (ABIERTO)\n" );
   else
   {
      printf( "Error (NO ABIERTO)\n" );
      return 1;
   }


  //primer reconocimiento del entorno para ello nos vamos a un lateral y escaneamos
  Pos[0]=400;
  Pos[1]=250; // debo establecer el punto incial en el centro del mapa?
  Pos[2]=0;//3.14/4;
  angle_i2c=Brujula();
  //Pos[2]=angle_i2c;
  
  /*

  Exploracion(Pos, lold, lnew, Angl_Rob);

  // Busqueda de destino
  Busca_Pared(lnew, Pos, &destinoX, &destinoY, &d); //^
  printf("destino x:%i destino y:%i distancia:%lf\n",destinoX,destinoY,d);
  // una vez encontrada la pared mas cercana nos acercamos a ella con un margen de seguridad
  // calculamos d que lo hicimos en Busca_pared
  printf("Angulo del robot pirmer vistazo: %lf\n",Angl_Rob );
  alfa=((destinoX-Pos[0])*cos(Angl_Rob)+(destinoY-Pos[1])*sin(Angl_Rob))/d;//calculamos coseno de alfa
  printf("alfa es =>%lf y acos(alfa) es =>%lf\n",alfa,acos(alfa) );
  Angl_Rob=-acos(alfa)+Angl_Rob; // acos da el angulo en rad o en grados? radianes??
  printf("Angulo del robot segundo vistazo: %lf\n",Angl_Rob );
  printf("Cominenza el giro\n");
  Giro(alfa, Angl_Rob,  d);
  
  */


  //14.5 cm de radio desde el centor del robot

  /* MUY IMPORTANTE!!!!!!
    En este instaten tengo que decirle al coche cuanto girar para ello 
    empleo la formula del arco de circunferencia que es (2*pi*radiodelrobot*Angl_Rob)/360
    el resultado de esto sale en en grados, tienes que revisar las unidades de todos 
    los angulos es posible que alguno este mal
  */  


  /*while(d>Dist_Seg){
  Acercamiento(&d, Angl_Rob, Pos, destinoX, destinoY);
  Exploracion(Pos, lold, lnew, Angl_Rob);
  Dd=abs(destinoX-Pos[0]);Di=abs(destinoY-Pos[1]); // calculo las distancias que tengo que mandar y que el arduino calculara cuantas vueltas de encoder
  printf("Distancia Der %lf, Distancia Izq %lf\n",Dd,Di);
  }*/
  
  fprintf(fichero, "a=[\n"); 
  for (if3 = 0; if3 <500; ++if3)
  {
    for (jf3 = 0; jf3 < 500; ++jf3)
    {
      //printf("%i ==>",jf3+500*if3);
      //printf("%.1lf\n",lnew[jf3+500*if3] ); 
      fprintf(fichero, "%.3g\t", lnew[500*jf3+if3]); 
    }  // para ver las matrices por columnas  
    fprintf(fichero, ";\n"); 
  }
  fprintf(fichero, "];\n"); 

  


  if( !fclose(fichero) )
      printf( "\nFichero cerrado\n" );
   else
   {
      printf( "\nError: fichero NO CERRADO\n" );
      return 1;
   }




	// falta el seguir la pared y ya.
  occupancy_terminate(); 
  Close_port(fd);
  return 0;
}

/*
 * File trailer for main.c
 *
 * [EOF]
 */
  /*for (if2 = 0; if2 <500; ++if2)
  {
    for (jf2 = 0; jf2 < 500; ++jf2)
    {
      if(lnew[jf2+500*if2]!=0.5){
      printf("%i ==>",jf2+500*if2);
      printf("%.1lf\n",lnew[jf2+500*if2] ); 
      }  // para ver las matrices por columnas  
    }
    //printf("\n");
  }
  printf("NADA\n");*/

  /*Emitir(&Zt1,&Zt2); // manda una señal al arduino para que emita y este le devuleve los valores
  Zt1=Zt1*100;Zt2=Zt2*100; // pasamos de cm a casillas 1 cm son 100 casillas
  printf("Zt1=%lf Zt2=%lf\n",Zt1,Zt2 );
  occupancy(Pos,Zt1, lold, lnew); // pos sera la posicion del robot mas o menos la distancia del sensor dependiendo si es izquierda o derecha*/
  

    /*for (jf2=0;jf2<NUM_POS;jf2++){
   // Mover_Servo(Pos_Torreta[jf2]);
    Emitir(&Zt1,&Zt2); // manda una señal al arduino para que emita y este le devuleve los valores
    Zt1=Zt1*100;Zt2=*Zt2*100; // pasamos de cm a casillas 1 cm son 100 casillas
    //printf("Zt1=%lf Zt2=%lf\n",Zt1,Zt2 );
    Pos[2]=Angl_Torreta[jf2]-Angl_Rob;
    occupancy(Pos,50, lold, lnew); // pos sera la posicion del robot mas o menos la distancia del sensor dependiendo si es izquierda o derecha
    //falta un occupancy para el segundo sensor y en el aterior restarle en la posicion correspondiente la difrencia de distancia en x e y
  }*/
