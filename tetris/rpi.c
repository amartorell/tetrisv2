#include "rpi.h"

void rpi_init(int8_t joy_axis, int8_t display_axis, int8_t joy_dir_x, int8_t joy_dir_y)
{
	clrscr();

	joy_init();					
	display_init();				 

	set_joy_axis(joy_axis);

	set_display_axis(display_axis);

	set_joy_direction(joy_dir_x, joy_dir_y);
}

void rpi_start_game()
{
	int i, j;

	for (i = 0; i<16; i++)
		for (j = 0; j<16; j++)
			display_write(i, j, D_ON);		//encendemos todos los led

	display_update();			// Send display buffer to display 
	printf("presione el analogico del joystick para comenzar \n");
	

	while (joystick_get_switch_value() == J_NOPRESS)	//el juego comienza cuando se presione el analogico
		joystick_update();								// leemos del joystick para saber si fue presionada la tecla
	display_clear();									// limpiamos la pantalla
	display_update();
}

void rpi_display_game(int8_t mat[N + 5][N])
{
	int i, j;

	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
		{
			if (mat[i][j] != 0)					// si la posicion en la matris no figura como libre
				display_write(i, j, D_ON);		// se encendera el led
			else
				display_write(i, j, D_OFF);		// si esta libre el led del display permanece apagado
		}

	display_update();	// mostramos los cambio hechos en display
}

void rpi_end_game()
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
			display_write(i, j, D_ON);
		display_update();				// podriamos hacer que se dibuje END o algo en pantalla
	}
}