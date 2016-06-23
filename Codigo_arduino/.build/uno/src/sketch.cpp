#include <Arduino.h>
#include <FSM.h>
#include "myStates.h"
#include "Ultrasonic.h"
void setup();
void loop();
void lectura(void);
void ReadEvents(void);
void cambio_encD();
void cambio_encI();
void MDerecho(void);
void MIzquierdo(void);
void Emitir(void);
#line 1 "src/sketch.ino"

// -------------------------------------------------------------------------------------
// 
//    ROBOT
//
//  Utiliza libreria FSM
// Buscar hotspot parar generar wifi con las rasp adafruit
//
// Pines ocupados por los motores : 4 y 5 para el motor derecho, 12 y 13 para el motor izquierdo
// pines 9 y 11 para la pwm derecha e izquierda respectivamente 
//
// V2.3 actualizacion de velocidades y mando de posicion en todo momento 29/2/16
//
// Las velocidades se deciden a traves de las teclas qwe y asf
// 
//  q=>Ri vBaja   a=>Rd Vbaja
//  w=>Ri vmedia  s=>Rd Vmedia
//  e=>Ri valta   f=>Rd Valta
//
// V2.31 introduccion de servo para la torreta utilizando el pin 12 para su control este pin es provisional.
// No puedo utilizar el servo con el arduino ya que la libreria servo me consume 3 de las 4 pwm, lo voy a mover con la rasp
// V2.32 introducimos una funcion de parada
// -------------------------------------------------------------------------------------

//#include <TimerOne.h>
//#include <FSM.h>
//#include <Servo.h>
//#include "myStates.h"
//#include "Ultrasonic.h"

unsigned char myEvent;
char myByteRec;
unsigned char MyNewEvent [1000]; //[900];
int Wr=0, Rd=0;
boolean Proc_EvD=false, Proc_EvI=false;
double  UI=0, UD=0;
const int VBAJA =180, VMEDIA=217, VALTA=255;

// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 pines digitales usados
//     x x x x x x x x x  x  x  x
// estos pines vienen dados por el hardware que estoy utilizando y la necesidad de utilizar interrupciones
int pinMotorDireccionD1=4; 
int pinMotorDireccionD2=5;

int pinMotorDireccionI1=12;
int pinMotorDireccionI2=13;

int pinMotorPWMD=9;
int pinMotorPWMI=11;

// los pines que len las vueltas
//int pinAencoderD=7;
//int pinBencoderI=8;
int SensEcho1=6; //Extremo izquierdo
int SensEcho2=7; //Extremo derecho
int SensTriger1=8;
int SensTriger2=10;
// variables GLOBALES
// encoders (variables globales que llevan la cuenta)
volatile long encoderD = 0;
volatile long encoderI = 0;

// Variables fisicas

int r=3;//cm
int Nt=81;//360;//35; // giro de una vuelta de encoder
double pi=3.14;

int Nd=0, Ni=0;

Ultrasonic Sens1(SensTriger1,SensEcho1);
Ultrasonic Sens2(SensTriger2,SensEcho2);

const byte interruptPin=2;

void setup()
{
  // Timer1.disablePwm(9);
  //interrupcion temporal inicializacion
   
   //Timer1.initialize(150); //0.15ms
   //Timer1.pwm(10, 512);
 // Timer1.disablePwm(9);
  pinMode(pinMotorDireccionD1, OUTPUT);
  pinMode(pinMotorDireccionD2, OUTPUT);
  pinMode(pinMotorDireccionI1, OUTPUT);
  pinMode(pinMotorDireccionI2, OUTPUT);
  pinMode(pinMotorPWMD, OUTPUT);
  pinMode(pinMotorPWMI, OUTPUT);
  //miServo.attach(12);
  
  //mensaje por el puerto serie
  Serial.begin(9600);
  Serial.println("Esperando comandos \n"); 

  // Configuracion de velocidad, la velocidad esta configurada inicialmente como baja
  analogWrite(pinMotorPWMD,VMEDIA+10);//217
  analogWrite(pinMotorPWMI,VMEDIA);

  // Interrupciones encoder
 pinMode(0,INPUT_PULLUP);
 pinMode(1,INPUT_PULLUP);
 attachInterrupt(0,cambio_encD,RISING);
 attachInterrupt(1,cambio_encI,FALLING); //CHANGE
 
 // attachInterrupt(digitalPinToInterrupt(interruptPin),cambio_encD,CHANGE);
//  attachInterrupt(digitalPinToInterrupt(1),cambio_encI,CHANGE);

  //Timer1.attachInterrupt(lectura);
  digitalWrite(pinMotorDireccionD1,HIGH);
  digitalWrite(pinMotorDireccionD2,LOW);
  digitalWrite(pinMotorDireccionI1,LOW);
  digitalWrite(pinMotorDireccionI2,HIGH);
 // miServo.write(90);
  delay(100);
  digitalWrite(pinMotorDireccionD1,LOW);
  digitalWrite(pinMotorDireccionD2,LOW);
  digitalWrite(pinMotorDireccionI1,LOW);
  digitalWrite(pinMotorDireccionI2,LOW);
  //miServo.write(-90);
  //Serial.end();
  
   FSM.begin(FSM_NextState,nStateMachine,FSM_State,nStateFcn,STATE1);

  }

  void loop()
{
  //PASOS DE LA MAQUINA DE ESTADOS
  //  Primer paso:  LEO EVENTOS EXTERNOS
  //  Segundo paso: ACTUALIZO ESTADOS
  //  Tercer paso:  EJECUTO FUNCION CORRESPONDIENTE A DICHO ESTADO
  //  Cuaro paso:   ACTUALIZO ESTADOS DEPENDIENDO DE LOS EVENTOS INTERNOS GENERADOS
  lectura();
  FSM.Update();
 }


void lectura(void)
{
  ReadEvents(); 
  if(Proc_EvD){
    Proc_EvD=false;
    MyNewEvent[Wr]=EV_D;
    Wr++;
  }
  else if (Proc_EvI){
    Proc_EvI=false;
    MyNewEvent[Wr]=EV_I;
    Wr++;
  }
  if(Wr!=Rd){
   // Serial.print("posicion es Rd");
//    Serial.print("Rd=");//rueda derecha
    Serial.print(encoderD);
    Serial.print("	");
//    Serial.print("Ri=");//rueda izquierda
    Serial.println(encoderI);    
    FSM.AddEvent(MyNewEvent[Rd]); 
    Rd++;
  }
  else{
    Wr=0;
    Rd=0;
  }
  FSM.Update();
}


void ReadEvents(void) // buena pinta
{
  
   myEvent=0;
   if (Serial.available()>0)
   {
     myByteRec=Serial.read();
  while (myByteRec!='*') 
  { 
    switch (myByteRec)
      {
        case 'i':case'I':
        UI=Serial.parseFloat();
        Ni=(UI*Nt)/(2*pi*r);
        MyNewEvent[Wr]=EV_I;      //Evento rueda Izquierda
        Wr++;
	encoderI=0;
        break;
        
        case 'd':case'D':
        UD=Serial.parseFloat(); 
        Nd=(UD*Nt)/(2*pi*r);
        Serial.print(Nd);
        MyNewEvent[Wr]=EV_D;      //Evento rueda Derecha
        Wr++;
	encoderD=0;
        break; 

        case 'p':case 'P':
        digitalWrite(pinMotorDireccionI1,LOW);
        digitalWrite(pinMotorDireccionI2,LOW);
        digitalWrite(pinMotorDireccionD1,LOW);
        digitalWrite(pinMotorDireccionD2,LOW);
        Serial.print("parada");
        delay(10);
        for (int i=0;i<900;i++){
          MyNewEvent[i]=0;
        } 
        Wr=0;
        Rd=0;
        Proc_EvD=false;
        Proc_EvI=false;
        FSM.AddEvent(EV_LEC); 
        encoderD=0;
        encoderI=0;
        break;
        
        case 'r':case 'R':
        Serial.print("VIB");
        analogWrite(pinMotorPWMI,VBAJA);
        break;
        
        case 't':case 'T':
        Serial.print("VIM");
        analogWrite(pinMotorPWMI,VMEDIA);
        break;
        
        case 'y':case 'Y':
        Serial.print("VIA");
        analogWrite(pinMotorPWMI,VALTA);
        break;
        
        case 'a':case 'A':
        Serial.print("VDB");
        analogWrite(pinMotorPWMD,VBAJA);
        break;
        
        case 's':case 'S':
        Serial.print("VDM");
        analogWrite(pinMotorPWMD,VMEDIA);
        break;
        
        case 'f':case 'F':
        Serial.print("VDA");
        analogWrite(pinMotorPWMD,VALTA);
        break;
        
        case 'e': case 'E':
        //Serial.println("Emito");
        Emitir();    //Evento rueda Derecha
        break;
        
        case ' ':
        break;
      }
       myByteRec=Serial.read();
    }
  }
}

// lectura del encoder Derecho
void cambio_encD()
{
  encoderD++;
}
// lectura del encoder izquierdo
void cambio_encI()
{
  encoderI++;
}
void MDerecho(void){ 
 if (encoderD<Nd){
    Proc_EvD=true;
    digitalWrite(pinMotorDireccionD1,HIGH);
    digitalWrite(pinMotorDireccionD2,LOW);
      }
  else{
    Proc_EvD=false;
    Serial.flush();
    //Serial.println("K");
    encoderD=0;
    digitalWrite(pinMotorDireccionD1,LOW);
    digitalWrite(pinMotorDireccionD2,LOW); 
  }
}
void MIzquierdo(void){  
 if (encoderI<Ni){
    Proc_EvI=true;
    digitalWrite(pinMotorDireccionI1,LOW);
    digitalWrite(pinMotorDireccionI2,HIGH);
      }
  else{
    Proc_EvI=false;
    Serial.flush();
    Serial.println("K");
    //Serial.end();
    encoderI=0;
    digitalWrite(pinMotorDireccionI1,LOW);
    digitalWrite(pinMotorDireccionI2,LOW);
    
  }
}
void Emitir(void){
 // Serial.print("HEY");
  int dist1=0, dist2=0;
  dist1=Sens1.Ranging(CM); //funcion especifica del sensor que devuelve el valor en Cm
  dist2=Sens2.Ranging(CM); //
  //Serial.print("distancia1:");
  Serial.print(dist1);
  Serial.print("|");
  Serial.print(dist2);
  Serial.println("K");
}

