///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
// /*
//  * File:   main.c
//  * Author: ariel
//  *
//  * Created on December 9, 2017, 11:19 AM
//  *
//  */
//



//#include "manejomat.h"
//#include "constantes.h"
//#include "constantesAllegro.h"
//#include <allegro5/allegro.h>
//
//
//
//char c;	//////////SACA ESTO DESPUES////////////////
//
//int main(void)
//{
//	uint8_t tablero[N + 5][N];
//	
//	rand(time(NULL));
//	set_board(tablero);
//
//
//	uint16_t puntaje = PUNTAJE_INICIAL;
//
//	bloque_t *pieza;
//	pieza = crear_pieza();
//	bool start_game = TRUE;
//	bool do_exit = FALSE;
//	bool restart = FALSE;
//	bool end_game = FALSE;
//	uint16_t nivel = NIVEL_INICIAL;
//	uint16_t    highscore;
//
//	while (!do_exit)
//	{
//		if (restart == TRUE)
//		{
//			puntaje = PUNTAJE_INICIAL;
//			nivel = NIVEL_INICIAL;
//			//yo del futuro reinicia los timer &0
//			restart = FALSE;
//
//		}
//
//		if (start_game)
//		{
//			//mandale una intro &0
//			start_game = FALSE;
//		}
//
//		while (!do_exit)
//		{
//			imprimir_mat(tablero, N+5, N);
//			while ((c = getchar()) != '\n')
//			{
//				if (c == 's') //para abajo
//				{
//					mover_bloque(tablero, pieza, ABAJO);
//				}
//				else if (c == 'd')
//				{
//					mover_bloque(tablero, pieza, DERECHA);
//				}
//				else if (c == 'a')
//				{
//					mover_bloque(tablero, pieza, IZQUIERDA);
//				}
//				else if (c == 'w')
//				{
//					rotar_bloque(tablero, pieza);
//				}
//			}
//			/*METER LAS BOLUDECES DE TIMER &0*/
//			if (!chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
//			{
//				estampar_bloque(pieza, tablero, &puntaje);
//				free(pieza);
//				pieza = crear_pieza();
//			}
//
//			if (end_game = chequear_lineas(tablero, ALTURA_LIMITE, N, FIJO))
//			{
//				do_exit = TRUE;
//			}
//		}
//
//	}
//
//}


#include "manejomat.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro_fun.h"
#include "constantesAllegro.h"
#include"scoreManager.h"






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
	
	ALLEGRO_SAMPLE *sample = NULL;   //para cargar la musica

	ALLEGRO_FONT * tipo_letra = NULL;
	ALLEGRO_FONT * tipo_letra_pausa_1 = NULL;
	ALLEGRO_FONT * tipo_letra_pausa_2 = NULL;


	ALLEGRO_BITMAP * icon = NULL;

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
	icon = al_load_bitmap("logo.jpg");
	al_set_display_icon(display, icon);
	if (!(pause_image = al_load_bitmap("pause.bmp")))
	{
		fprintf(stderr, "failed to load image !\n");
	}

	if (!(start_image = al_load_bitmap("start_menu.bmp")))
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
	tipo_letra_pausa_1 = al_load_font("Minercraftory.ttf", 76, 0);
	if (!tipo_letra_pausa_1) {
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
	bloque_t * next_pieza;
	uint16_t puntaje = 0;


	pieza = crear_pieza();
	next_pieza = crear_pieza();


	bool start_game = FALSE;


	uint16_t nivel = NIVEL_INICIAL;     //establezco nivel inicial

	uint16_t highscore=get_highscore("highscore.txt");




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
			al_stop_timer(timer_pieza); //para el timer de tiempo
			al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

			timer_pieza = al_create_timer(TIEMPO);
			//proteccion por si hay error
			if (!timer_pieza) {
				fprintf(stderr, "failed to create timer!\n");
				return -1;
			}

			al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
			al_start_timer(timer_pieza);
		}

		if (puntaje >= (nivel*BARRERA_NIVEL))
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

		ALLEGRO_EVENT ev;
		if (!start_game)
		{
			start_game_scenario(event_queue, ev, start_image, display, &start_game, &do_exit);
		}
		print_score(tipo_letra, puntaje, highscore,nivel, display, board, tablero, next_pieza->mat_de_pieza);
		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				if (ev.timer.source == timer_de_caida)
				{
					if (key_pressed[KEY_UP])     //poner condiciones del rotar
					{
						rotar_bloque(tablero, pieza);
					}
					if ((key_pressed[KEY_DOWN])) //me fijo en el bool si me puedo seguir moviendo para abajo
					{
						mover_bloque(tablero, pieza, ABAJO);
					}
					if ((key_pressed[KEY_LEFT])) //me fijo si la pieza esta presionada y si puedo moverme a la izquierda 
					{
						mover_bloque(tablero, pieza, IZQUIERDA);
					}
					if ((key_pressed[KEY_RIGHT])) ////me fijo si la pieza esta presionada y si puedo moverme a la derecha
					{
						mover_bloque(tablero, pieza, DERECHA);
					}


				}
				else if (ev.timer.source == timer_pieza) //tiempo que pasa para que caiga la pieza
				{
					if ((pieza->y < (N + 2)) && chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{
						mover_bloque(tablero, pieza, ABAJO);

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
						estampar_bloque(pieza, tablero, &puntaje);
						dibujar_tablero(board, display, tablero, next_pieza->mat_de_pieza);

						copiar_pieza(pieza, next_pieza);
						free(next_pieza);
						next_pieza = crear_pieza();

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
					pause_scenario(event_queue, ev, display, pause_image, timer_pieza, ultimo_movimiento, sample, &do_exit);
					break;
				}

				}
			}

			if ((nivel >= MAX_NIVEL) || (end_game = chequear_lineas(tablero, ALTURA_LIMITE, N, FIJO)))  //que pasa cuando se llega a game over
			{
				end_game_scenario(event_queue, ev, timer_pieza, ultimo_movimiento, display, tipo_letra, tipo_letra_pausa_1, puntaje,highscore, &restart, &do_exit);
				if (puntaje > highscore)
				{
					set_highscore("highscore.txt", puntaje);
				}
			}

		}



	}


	//destruimos todo al finalizar el programa

	free(pieza);
	free(next_pieza);
	al_destroy_bitmap(board);
	al_destroy_display(display);
	al_shutdown_image_addon();
	al_destroy_bitmap(start_image);
	al_destroy_bitmap(pause_image);
	al_destroy_timer(timer_de_caida);
	al_destroy_timer(timer_pieza);
	al_destroy_timer(ultimo_movimiento);
	al_destroy_event_queue(event_queue);
	return 0;
}