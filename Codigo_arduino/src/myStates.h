//-----------------------------------------------------
//ESTE ES EL ÚNICO FICHERO QUE HAY QUE MODIFICAR
//-----------------------------------------------------

#ifndef myStates_H
#define myStates_H


//Declaracion de las funciones
extern void lectura(void); // Espera 
extern void MDerecho(void); // Derecha
extern void MIzquierdo(void); // IZQUIERDA




//Declaracion del nombre de ESTADOS y de EVENTOS
#define STATE1  				0x01
#define STATE2  				0x02
#define STATE3 	                                0x03


#define EV_LEC                                   0x41
#define EV_D     				 0x43
#define EV_I   	                                 0x45	
#define EV_ERROR    	                         0x44	



// Estructuras descriptivas de mi diagrama de flujo
const FSMClass::FSM_State_t FSM_State[] PROGMEM= {
// STATE,STATE_FUNC
{STATE1,lectura},
{STATE2,MDerecho},// motor derecho
{STATE3,MIzquierdo}, // motor izquierdo
};

const FSMClass::FSM_NextState_t FSM_NextState[] PROGMEM= {
// STATE,EVENT,NEXT_STATE


//   ESTADO 1: LECTURA    //
{STATE1,EV_D,STATE2},
{STATE1,EV_I,STATE3},

//     ESTADO 2: MDERECHO   //
{STATE2,EV_LEC,STATE1},
{STATE2,EV_ERROR,STATE1},
{STATE2,EV_I,STATE3},
//    ESTADO 3: MIZQUIERDO    //
{STATE3,EV_LEC,STATE1},
{STATE3,EV_ERROR,STATE1},
{STATE3,EV_D,STATE2},
//    ESTADO 5    //
//{STATE5,EV_NULL,STATE1},
};

//Macros para el cálculo de los tamaños de las estructuras
//NO MODIFICAR
#define nStateFcn		sizeof(FSM_State)/sizeof(FSMClass::FSM_State_t)
#define nStateMachine		sizeof(FSM_NextState)/sizeof(FSMClass::FSM_NextState_t)

#endif


