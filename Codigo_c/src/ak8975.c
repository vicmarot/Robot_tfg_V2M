
#include <wiringPiI2C.h>
#include  <stdio.h>
#include <math.h>
#include <errno.h>


static long ak8975_raw_to_gauss(short int data)
{
         return (((long)data + 128) * 3000) / 256;
}

float Brujula(){
	int devId=0x0c, fd;
	//unsigned char aux;
	//unsigned short int dato;
	char aux;
	short int dato;
	float angle, pi=3.14;
	int ver;

	/* configuro direccion*/
	fd=wiringPiI2CSetup (devId) ;
	/*escribo para que recoja dato*/
	if (ver=wiringPiI2CWriteReg8 (fd,0x0A, 0x1)==-1){
		fprintf (stdout, "Error en la escritura: %s\n", strerror (errno)) ;
        return 1 ;
	}
	while(dato=wiringPiI2CReadReg8 (fd, 0x02)!=1){
		printf("Esperando a dato\n"); 
	}

	/* Valor de X */
	aux=wiringPiI2CReadReg8 (fd, 0x04) ;
	dato=aux;
	dato=(dato<<8);
	aux=wiringPiI2CReadReg8(fd, 0x03);
	dato=dato | aux;
	#if (DEBUG)
	printf("%i\n", dato);
	#endif
	int x=-15;
//	x=ak8975_raw_to_gauss(dato);
	x=dato;
	printf("X=>%i\n",x  );
	/* Valor de Y*/
	aux=wiringPiI2CReadReg8 (fd, 0x06) ;
	dato=aux;
	dato=(dato<<8);
	aux=wiringPiI2CReadReg8(fd, 0x05);
	dato=dato | aux;
	#if (DEBUG)
	printf("%i\n", dato);
	#endif
	int y=-15;
//	y=ak8975_raw_to_gauss(dato);
	y=dato;
	printf("Y=>%i\n",y  );

	/* Calculo el angulo y lo devuelvo*/
	
	angle =atan2(y,x)*180/pi+180;
	while(angle<0)angle+=360;
	while(angle>360)angle-=360;
	printf("en grados=>%lf\n", angle);
	angle=angle*pi/180;
	printf("en rad=>%lf",angle);
	return angle;

}
