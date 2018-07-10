#ifndef RPI_H
#define RPI_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>	
#include "termlib.h"
#include "joydrv.h"
#include "disdrv.h"
#include "constantes.h"


#define JOY_AXIS_INVERT J_INV_TRUE 

#define JOY_THRESHOLD 40	//con esto manejamos la sencibilidad del joystick

void rpi_init(int8_t joy_axis, int8_t display_axis, int8_t joy_dir_x, int8_t joy_dir_y);	//inicializa la rpi (display y joystick) y setea con los argumentos de entrada

void rpi_start_game();	//hace la secuencia de inicio del juego (encender todo y esperar a que el user presione el analogico)

void rpi_display_game(int8_t mat[N + 5][N]);	//recive la matriz con los datos del estado del juego y los manda a display

void rpi_end_game();		//secuencia de finalizacion del juego


#endif		//ACORDATE QUE EN EL MAIN TENES QUE CAMBIAR QUE SE CONTROLE CON EL TECLADO A QUE SE CONTROLE DESDE ANALOGICO