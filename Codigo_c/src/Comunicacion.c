/*************************************************/
/* Funcionaes para apertura escritura y lectura  */
/* del puerto serie      						 */
/*  Autor Victor Manuel Maroto Ortega            */
/*************************************************/

#include "Comunicacion.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>


int Open_port (int fd){

  char buf[64];
  struct termios toptions;
	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  if (fd==-1){
    fprintf(stderr,"Error=>Al abrir el puerto\n");
    exit(EXIT_FAILURE);
  }
  printf("fd opened as %i\n", fd);

  usleep(3500000);
  
  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions, B9600);
  cfsetospeed(&toptions, B9600);
  /* 8 bits, no parity, no stop bits */
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);
  printf("Primera lect\n");
  read(fd,buf,64); // pirmera lectura
  //printf("%s y numero de datos %i\n",buf,n); // lee esperando comandos
  printf("read: %s\n",buf);
  return fd;
}

int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do { 
        int n = read(fd, b, 1);  // read a char at a time
//        if( n==-1) return -1;    // couldn't read
	  if (n==-1){
		fprintf(stderr,"Error=> Lectura\n");
	        exit(EXIT_FAILURE);
	}
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );

    buf[i] = 0;  // null terminate the string
    return 0;
}
int writen(int fd, char* str)
{
  int len;
  //strcpy(str,"i12*");
  len=strlen(str);
  write(fd,(void*)str,len);
  return 0;
}
int Close_port (int fd)
{
	 if(close(fd)==-1)
    fprintf(stderr,"Error=> Al cerrar el puerto\n");
    exit(EXIT_FAILURE);
}
