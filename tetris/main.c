/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   main.c
  * Author: ariel
  *
  * Created on December 9, 2017, 11:19 AM
  *
  */

#include "manejomat.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>





void dibujar_tablero(ALLEGRO_BITMAP * bitmap, ALLEGRO_DISPLAY * display, uint8_t mat[N + 5][N]);
//funcion que dibuja en el display como va el tablero de juego mostrando sus piezas 


//constantes allegro

#define SCREEN_W		800
#define SCREEN_H		600
#define ALTURA_BLOQUE           20
#define ANCHO_BLOQUE            20
#define TIEMPO_CAIDA            10
#define TIEMPO_ESPERA_ULTIMO_MOV           0.500
#define TIEMPO                  0.500   

#define BARRERA_NIVEL           5
#define NIVEL_INICIAL           1
#define PUNTAJE_INICIAL         0   

#define MINIMO_X                0
#define MAXIMO_X                N

#define MAXIMO_Y                (N+5) 
#define MAX_NIVEL               10  


enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT //flechitas teclado
};



/**/

int main(void)
{

	uint8_t tablero[N + 5][N];

	ALLEGRO_DISPLAY *display = NULL;    //display

	ALLEGRO_BITMAP * board = NULL;      //bitmap del tablero
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;    //cola de evento
	ALLEGRO_TIMER *timer_pieza = NULL;          //timer del teclado
	ALLEGRO_TIMER *timer_de_caida = NULL;       //timer de caida 
	ALLEGRO_TIMER *ultimo_movimiento = NULL;     //timer de control de ultimo movimiento

	ALLEGRO_BITMAP * pause_image;       //para las distintas imagenes de los distintos menues
	ALLEGRO_BITMAP * start_image;
	ALLEGRO_BITMAP * game_over_image;

	ALLEGRO_SAMPLE *sample = NULL;   //para cargar la musica

	ALLEGRO_FONT * tipo_letra = NULL;



	bool do_exit = FALSE; //control de cerrar display
	bool key_pressed[4] = { FALSE, FALSE, FALSE, FALSE }; //Estado de teclas, true cuando esta apretada

	bool rotacion = TRUE;
	bool end_game = FALSE;        //bool que se utiliza para ver si se hizo gameover o no
	bool restart = FALSE;




	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H); // Intenta crear display de 640x480 
	if (!display) {
		fprintf(stderr, "failed to create display!\n");

		return -1;
	}
	al_set_window_title(display, "TETRIS");
	
	if (!al_install_keyboard()) {                                                   //instalo el teclado
		fprintf(stderr, "failed to initialize the keyboard!\n");
		return -1;
	}
	board = al_create_bitmap(ANCHO_BLOQUE*(N), ALTURA_BLOQUE*(N + 5));
	if (!board) {
		fprintf(stderr, "failed to create bloque bitmap!\n");
		return -1;
	}

	pause_image = al_create_bitmap(SCREEN_W, SCREEN_H);
	if (!pause_image) {
		fprintf(stderr, "failed to create bloque bitmap!\n");
		return -1;
	}

	start_image = al_create_bitmap(SCREEN_W, SCREEN_H);
	if (!start_image) {
		fprintf(stderr, "failed to create bloque bitmap!\n");
		return -1;
	}

	game_over_image = al_create_bitmap(SCREEN_W, SCREEN_H);
	if (!game_over_image) {
		fprintf(stderr, "failed to create bloque bitmap!\n");
		return -1;
	}



	if (!al_init_primitives_addon()) {                                        //inicio las primitivas
		fprintf(stderr, "failed to initialize primitives!\n");
		return -1;
	}

	event_queue = al_create_event_queue();                       //cola de eventos creada
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);

		return -1;
	}



	timer_de_caida = al_create_timer(1.0 / TIEMPO_CAIDA);
	if (!timer_de_caida) {
		fprintf(stderr, "failed to create timer_caida!\n");
		return -1;
	}

	timer_pieza = al_create_timer(TIEMPO);
	if (!timer_pieza) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	ultimo_movimiento = al_create_timer(.500);
	if (!ultimo_movimiento) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	//imagenes
	if (!al_init_image_addon())
	{
		fprintf(stderr, "Unable to start image addon \n"); //Igual que printf pero imprime al error std 
		al_uninstall_system();
		return -1;
	}
	ALLEGRO_BITMAP * icon = al_load_bitmap("logo.jpg");
	al_set_display_icon(display, icon);
	if (!(pause_image = al_load_bitmap("pause.bmp")))
	{
		fprintf(stderr, "failed to load image !\n");
	}

	if (!(start_image = al_load_bitmap("start_menu.bmp")))
	{
		fprintf(stderr, "failed to load image !\n");
	}

	if (!(game_over_image = al_load_bitmap("game_over.bmp")))
	{
		fprintf(stderr, "failed to load image !\n");
	}

	// creo lo que compete con la musica


	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(1)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}

	al_init_font_addon();
	al_init_ttf_addon();

	// inicio las letras   



	tipo_letra = al_load_font("text.ttf", 24, 0);
	if (!tipo_letra) {
		fprintf(stderr, "failed to create font!\n");
		return -1;
	}


	sample = al_load_sample("music.ogg");
	if (!sample) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}


	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);



	al_register_event_source(event_queue, al_get_display_event_source(display));            //registro los posibles eventos que pueden ocurrir
	al_register_event_source(event_queue, al_get_timer_event_source(timer_de_caida));
	al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
	al_register_event_source(event_queue, al_get_timer_event_source(ultimo_movimiento));
	al_register_event_source(event_queue, al_get_keyboard_event_source());




	set_board(tablero); //para setear el tablero
	srand(time(NULL));  //para iniciar la semilla del random
	bloque_t * pieza;
	uint16_t puntaje = 0;


	pieza = crear_pieza();



	bool start_game = FALSE;


	uint16_t   nivel = 1;     //establezco nivel inicial

	uint16_t    highscore;




	al_start_timer(timer_de_caida); //inicializar siempre los timers
	al_start_timer(timer_pieza);    //timer de caida






	while (!do_exit)
	{

		if (restart == TRUE)
		{
			set_board(tablero);
			puntaje = PUNTAJE_INICIAL;
			nivel = NIVEL_INICIAL;
			restart = FALSE;
		}

		if (puntaje == (nivel*BARRERA_NIVEL))
		{
			al_stop_timer(timer_pieza); //para el timer de tiempo
			timer_pieza = al_create_timer(TIEMPO - (0.1*TIEMPO*nivel));

			//proteccion por si hay error
			if (!timer_pieza) {
				fprintf(stderr, "failed to create timer!\n");
				return -1;
			}
			////////////////////////////

			al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
			al_start_timer(timer_pieza);
			nivel++;

		}

		while (!start_game)
		{
			al_draw_bitmap(start_image, 0, 0, 0); //cargo el menu de game over
			al_flip_display();


			ALLEGRO_EVENT ev;
			if (al_get_next_event(event_queue, &ev))
			{
				if (ev.type == ALLEGRO_EVENT_KEY_UP)    //control tecla levantada
				{
					switch (ev.keyboard.keycode)
					{
					case ALLEGRO_KEY_ENTER:
					{
						start_game = TRUE;
						break;
					}

					case ALLEGRO_KEY_ESCAPE:
					{
						start_game = TRUE;
						do_exit = TRUE;
						break;
					}
					}
				}

				al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
				al_clear_to_color(al_color_name("black"));

			}
		}

		al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
		al_clear_to_color(al_color_name("black")); //para actualizar el score y que no se superponga

		al_draw_text(tipo_letra, al_color_name("lightsteelblue"), 100, 150, 0, "SCORE:");
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), 100 + 24 * 3, 150 + 24, 0, "%d", puntaje);

		al_draw_text(tipo_letra, al_color_name("lightsteelblue"), 100, 150 + 24 * 5, 0, "LEVEL:");
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), 100 + 24 * 3, 150 + 24 * 6, 0, "%d", nivel);

		dibujar_tablero(board, display, tablero);  //siempre dibujo el tablero

		ALLEGRO_EVENT ev;
		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				if (ev.timer.source == timer_de_caida)
				{
					if (key_pressed[KEY_UP])     //poner condiciones del rotar
					{
						rotacion = rotate(pieza, tablero);
						if (rotacion)
						{
							clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
							poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);


						}

					}
					if ((key_pressed[KEY_DOWN])) //me fijo en el bool si me puedo seguir moviendo para abajo
					{

						(pieza->mov_vertical) = chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO);
						if ((pieza->mov_vertical))
						{
							clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
							pieza->y++;
							poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);


						}
					}
					if ((key_pressed[KEY_LEFT])) //me fijo si la pieza esta presionada y si puedo moverme a la izquierda 
					{
						(pieza->mov_izq) = chequear_movimiento(pieza, tablero, pieza->mov_izq, IZQUIERDA);
						if (pieza->mov_izq)
						{
							clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
							pieza->x--;
							poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);

						}
					}
					if ((key_pressed[KEY_RIGHT])) ////me fijo si la pieza esta presionada y si puedo moverme a la derecha
					{
						(pieza->mov_der) = chequear_movimiento(pieza, tablero, pieza->mov_der, DERECHA);

						if (pieza->mov_der)
						{
							clear_sector(tablero, pieza->x, pieza->y, SIZE, SIZE, OCUPADO);
							pieza->x++;
							poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);


						}
					}


				}
				else if (ev.timer.source == timer_pieza) //tiempo que pasa para que caiga la pieza
				{
					if ((pieza->y < (N + 2)) && chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{

						clear_sector(tablero, pieza->x, pieza->y, SIZE, SIZE, OCUPADO);
						pieza->y++;
						poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);

					}
					if (!chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{
						al_start_timer(ultimo_movimiento); //arranco el timer para dejar hacer unos ultimos movimientos laterales
					}
				}

				else if (ev.timer.source == ultimo_movimiento)
				{

					if (!chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{

						fijar_bloque(tablero, pieza);
						limpiar_matriz(tablero, &puntaje);
						dibujar_tablero(board, display, tablero);
						pieza = crear_pieza();

						if ((nivel == MAX_NIVEL) || (end_game = chequear_lineas(tablero, ALTURA_LIMITE, N, FIJO)))  //que pasa cuando se llega a game over
						{

							if (!(game_over_image = al_load_bitmap("game_over.bmp")))
							{
								fprintf(stderr, "failed to load image !\n");
							}


							al_stop_timer(timer_pieza); //paro los dos timer 
							al_stop_timer(ultimo_movimiento);

							al_draw_bitmap(game_over_image, 0, 0, 0); //cargo el menu de game over
							al_flip_display();


							while (end_game)
							{
								if (al_get_next_event(event_queue, &ev)) //busco un evento donde se presione devuelta la tecla
								{


									if (ev.type == ALLEGRO_EVENT_KEY_UP)
									{
										switch (ev.keyboard.keycode)
										{
										case ALLEGRO_KEY_ESCAPE:    //CASO TERMINO EL JUEGO
										{
											do_exit = TRUE;
											end_game = FALSE;
											break;
										}
										case ALLEGRO_KEY_R:     //caso reinicio
										{
											restart = TRUE;       //pongo como true asi puedo reiniciar el juego
											end_game = FALSE;
											break;
										}

										}

									}
								}
							}

							al_start_timer(timer_pieza);    //vuelvo a iniciar los timers
							al_start_timer(ultimo_movimiento);

							al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
							al_clear_to_color(al_color_name("black"));

						}
					}


					al_stop_timer(ultimo_movimiento);
				}



			}
			else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //en caso que se cierre el display
				do_exit = TRUE;


			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)      //control tecla presionada
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					key_pressed[KEY_UP] = true;
					break;

				case ALLEGRO_KEY_DOWN:
					key_pressed[KEY_DOWN] = true;
					break;

				case ALLEGRO_KEY_LEFT:
					key_pressed[KEY_LEFT] = true;
					break;

				case ALLEGRO_KEY_RIGHT:
					key_pressed[KEY_RIGHT] = true;
					break;
				}
			}

			else if (ev.type == ALLEGRO_EVENT_KEY_UP)    //control tecla levantada
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_UP:
					key_pressed[KEY_UP] = false;
					break;

				case ALLEGRO_KEY_DOWN:
					key_pressed[KEY_DOWN] = false;
					break;

				case ALLEGRO_KEY_LEFT:
					key_pressed[KEY_LEFT] = false;
					break;

				case ALLEGRO_KEY_RIGHT:
					key_pressed[KEY_RIGHT] = false;
					break;

				case ALLEGRO_KEY_ESCAPE:    //cierra el programa
					do_exit = true;
					break;

				case ALLEGRO_KEY_P:
				{
					al_stop_timer(timer_pieza); //paro los dos timers 
					al_stop_timer(ultimo_movimiento);
					bool pausa = FALSE;

					if (!(pause_image = al_load_bitmap("pause.bmp")))  //cargo la imagen de pausa
					{
						fprintf(stderr, "failed to load image !\n");
					}


					al_draw_bitmap(pause_image, 0, 0, 0);
					al_flip_display();



					while (!pausa)
					{
						if (al_get_next_event(event_queue, &ev)) //busco un evento donde se presione devuelta la tecla
						{


							if (ev.type == ALLEGRO_EVENT_KEY_UP)
							{
								switch (ev.keyboard.keycode)
								{
								case ALLEGRO_KEY_P:
									pausa = TRUE;
									break;

								case ALLEGRO_KEY_ESCAPE:    //si se aprieta escape en medio de la pausa cierro todo
								{
									pausa = TRUE;
									do_exit = TRUE;
									break;
								}

								default:
									pausa = FALSE;
									break;
								}
							}
						}
					}
					al_start_timer(timer_pieza);    //vuelvo a iniciar los timers
					al_start_timer(ultimo_movimiento);


					al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
					al_clear_to_color(al_color_name("black"));


					al_flip_display();

					break;
				}

				}
			}

		}



	}


	//destruimos todo al finalizar el programa

	free(pieza);
	al_destroy_bitmap(board);
	al_destroy_display(display);
	al_shutdown_image_addon();
	al_destroy_bitmap(start_image);
	al_destroy_bitmap(game_over_image);
	al_destroy_bitmap(pause_image);
	al_destroy_timer(timer_de_caida);
	al_destroy_timer(timer_pieza);
	al_destroy_timer(ultimo_movimiento);
	al_destroy_event_queue(event_queue);
	return 0;
}
















////////////////////////////////FUNCIONES DE IMPRESION EN EL DISPLAY/////////////////////////////////////////////////////


void dibujar_tablero(ALLEGRO_BITMAP * bitmap, ALLEGRO_DISPLAY * display, uint8_t mat[N + 5][N])
{

	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_color_name("black"));



	int i, j;
	for (i = (ALTURA_LIMITE - 1); i < N + 5; i++)
	{
		for (j = 0; j < N; j++)
		{

			if ((i == (ALTURA_LIMITE - 1)))
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("red"));
			}
			if (mat[i][j] == OCUPADO)
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("goldenrod"));
			}
			else if (mat[i][j] == FIJO)
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("lightsteelblue"));
			}
			else if (mat[i][j] == VACANTE)
			{
				al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}
			else if ((mat[i][j] == BORDE))
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("red"));
			}

			al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
		}

	}
	al_set_target_bitmap(al_get_backbuffer(display));
	al_draw_bitmap(bitmap, 240, 50, 0);
	al_flip_display();
}

