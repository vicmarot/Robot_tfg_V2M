/*************************************************/
/* Fichero de cabecera para la comunicacion      */
/*  Autor Victor MAnuel MAroto Ortega            */
/*************************************************/

int Open_port (int fd);

int serialport_read_until(int fd, char* buf, char until);

int writen(int fd, char* str);

int Close_port (int fd);