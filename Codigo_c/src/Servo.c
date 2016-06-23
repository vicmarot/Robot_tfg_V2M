#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>
#include "Servo.h"
#include <wiringPi.h>
#include </home/rasp/wiringpi/wiringPi-b0a60c3/wiringPi/softServo.c>
#include <softPwm.h>

Mover_Servo(int posicion){

	int comp=0;
    double pos[5];
    pos[0]=4.5;pos[1]=8.5;pos[2]=12.5;pos[3]=16.5;pos[4]=20.5;
//    printf("la posicion es %lf\n",pos[posicion-1]);
    pinMode(7,OUTPUT);
    digitalWrite(7,LOW);
    softPwmCreate(7,0,200);
    softPwmWrite(7,pos[posicion-1]);
    delay(200); //1000
    softPwmWrite(7,0);
}


Init_WiringPi(){
   if (wiringPiSetup () == -1)
    {
        fprintf (stdout, "oops: %s\n", strerror (errno)) ;
        return 1 ;
    }
}
