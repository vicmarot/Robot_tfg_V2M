#Programas
CC = gcc
     
#Directorios
INCLUDE = ./include
BIN = ./bin
LIB = ./lib
OBJ = ./obj
SRC = ./src
SdK = ./include/SDK
     
#Flags y miscelanea
CPPFLAGS = -lm -lpthread -lwiringPi
CPOFLAGS = -I$(INCLUDE) 


all: objetos
 #$(BIN)
	@echo "Creado ejecutable"
	$(CC) $(OBJ)/*.o -o $(BIN)/Robot $(CPPFLAGS)  # -L$(LIB) -lformas



objetos:
	@echo ""
	@echo "Compilando archivos fuente"
	$(CC) -c -o $(OBJ)/ak8975.o $(CPOFLAGS) $(SRC)/ak8975.cpp 
	$(CC) -c -o $(OBJ)/brujula.o $(CPOFLAGS) $(SRC)/brujula.cpp 
	$(CC) -c -o $(OBJ)/occupancy.o $(CPOFLAGS) $(SRC)/occupancy.cpp 
	$(CC) -c -o $(OBJ)/occupancy_initialize.o $(CPOFLAGS) $(SRC)/occupancy_initialize.cpp
	$(CC) -c -o $(OBJ)/occupancy_terminate.o $(CPOFLAGS) $(SRC)/occupancy_terminate.cpp
	$(CC) -c -o $(OBJ)/rtGetInf.o $(CPOFLAGS) $(SRC)/rtGetInf.cpp
	$(CC) -c -o $(OBJ)/rtGetNaN.o $(CPOFLAGS) $(SRC)/rtGetNaN.cpp
	$(CC) -c -o $(OBJ)/rt_nonfinite.o $(CPOFLAGS) $(SRC)/rt_nonfinite.cpp
	$(CC) -c -o $(OBJ)/Servo.o $(CPOFLAGS) $(SRC)/Servo.cpp
	$(CC) -c -o $(OBJ)/main.o $(CPOFLAGS) $(SRC)/main.cpp

clean:
	rm -f $(OBJ)/*.o
	@echo "Borrado de ficheros objeto realizado con exito"
     
    #Mrproper
mrproper:clean
	@echo "Limpiando"
	rm -f $(BIN)/localizacion
	rm -f $(LIB)/*.a
	@echo "Limpieza completada"