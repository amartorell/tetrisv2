#include "manejomat.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>
#include "constantesAllegro.h"
#include "rpi.h"
#include "scoreManager.h"


int mainRpi(void)
{
	uint8_t tablero[N + 5][N];

	ALLEGRO_EVENT_QUEUE *event_queue = NULL;    //vamos a necesitar una cola de eventos para el timer
	ALLEGRO_TIMER *timer_pieza = NULL;          //timer del teclado
	ALLEGRO_TIMER *timer_de_caida = NULL;       //timer de caida 
	ALLEGRO_TIMER *ultimo_movimiento = NULL;     //timer de control de ultimo movimiento
	ALLEGRO_SAMPLE *sample = NULL;   //para cargar la musica

	bool do_exit = FALSE; //control de cerrar display
	bool key_pressed[4] = { FALSE, FALSE, FALSE, FALSE }; //Estado de teclas, true cuando esta apretada

	bool rotacion = TRUE;
	bool end_game = FALSE;        //bool que se utiliza para ver si se hizo gameover o no
	bool restart = FALSE;
	bool pause = FALSE;

			/*variables que conciernen al rpi*/
	jcoord_t joy_coordinates;			//leemos el analogico
	jswitch_t joy_switch = J_NOPRESS;	//leemos el estado del switch




	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	event_queue = al_create_event_queue();                       //cola de eventos creada
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
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

	sample = al_load_sample("music.ogg");
	if (!sample) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}


	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);



	al_register_event_source(event_queue, al_get_timer_event_source(timer_de_caida));
	al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
	al_register_event_source(event_queue, al_get_timer_event_source(ultimo_movimiento));
	al_register_event_source(event_queue, al_get_keyboard_event_source());				//hacemos que podamos leer ademas datos del teclado




	set_board(tablero);
	srand(time(NULL));
	bloque_t * pieza;
	bloque_t * next_pieza;
	uint16_t puntaje = 0;


	pieza = crear_pieza();
	next_pieza = crear_pieza();


	bool start_game = FALSE;


	uint16_t nivel = NIVEL_INICIAL;

	uint16_t highscore = get_highscore("highscore.txt");




	al_start_timer(timer_de_caida);
	al_start_timer(timer_pieza);


	rpi_init(JOY_ROTATE, NORMAL, J_INV_TRUE, J_INV_TRUE);	//aqui se inicia y se configura le rpi



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
			
			if (!timer_pieza) {
				fprintf(stderr, "failed to create timer!\n");
				return -1;
			}

			al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
			al_start_timer(timer_pieza);
		}

		if (puntaje >= (nivel*BARRERA_NIVEL))
		{
			al_stop_timer(timer_pieza);
			timer_pieza = al_create_timer(TIEMPO - (0.1*TIEMPO*nivel));

			if (!timer_pieza) {
				fprintf(stderr, "failed to create timer!\n");
				return -1;
			}

			al_register_event_source(event_queue, al_get_timer_event_source(timer_pieza));
			al_start_timer(timer_pieza);
			nivel++;

		}

		ALLEGRO_EVENT ev;
		if (!start_game)
		{
			rpi_start_game();
		}
		joystick_update();
		joy_switch = joystick_get_switch_value();		//vemos si el switch fue presionado
		joy_coordinates = joystick_get_coord();			//tomamos el estado del analogico
													
		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				if (ev.timer.source == timer_de_caida)
				{
					if (joy_coordinates.y > JOY_THRESHOLD)     //si la coordenada en y supera el threshold (analogico para arriba) -> rotar
					{
						rotar_bloque(tablero, pieza);
					}
					else if (joy_coordinates.y < -JOY_THRESHOLD) //si la coordenada en y esta por debajo del limite movemos para abajo
					{
						mover_bloque(tablero, pieza, ABAJO);
					}
					if (joy_coordinates.x < -JOY_THRESHOLD) //leemos la coordenada en x del joystick para mover (o no) lateralmente la pieza
					{
						mover_bloque(tablero, pieza, IZQUIERDA);
					}
					else if (joy_coordinates.x > JOY_THRESHOLD) 
					{
						mover_bloque(tablero, pieza, DERECHA);
					}


				}
				else if (ev.timer.source == timer_pieza) //la pieza cae por si sola
				{
					if ((pieza->y < (N + 2)) && chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{
						mover_bloque(tablero, pieza, ABAJO);

					}
					if (!chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{
						al_start_timer(ultimo_movimiento);
					}
				}

				else if (ev.timer.source == ultimo_movimiento)
				{
					if (!chequear_movimiento(pieza, tablero, pieza->mov_vertical, ABAJO))
					{
						estampar_bloque(pieza, tablero, &puntaje);

						copiar_pieza(pieza, next_pieza);
						free(next_pieza);
						next_pieza = crear_pieza();

					}
					al_stop_timer(ultimo_movimiento);
				}
			}

			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)      //control tecla presionada
				do_exit = TRUE;			//si el usuario presiona esc el juego termina


			else if (joy_switch == J_PRESS)    //si se presiona el switch...
			{
				pause = TRUE;
				while (joy_switch == J_PRESS)
				{
					joystick_update();
					joy_switch = joystick_get_switch_value();	//esperamos a que el usuario deje de presionar el switch para iniciar la pause
				}
				while (pause)
				{
					joystick_update();
					joy_switch = joystick_get_switch_value();
					if (joy_switch == J_PRESS)
						pause = FALSE;
				}
				while (joy_switch == J_PRESS)
				{
					joystick_update();
					joy_switch = joystick_get_switch_value();	//el juego reanuda cuando el usuario presiona y suelta el switch
				}
			}

			if ((nivel >= MAX_NIVEL) || (end_game = chequear_lineas(tablero, ALTURA_LIMITE, N, FIJO)))  //que pasa cuando se llega a game over
			{
				rpi_end_game();
				printf("puntaje: %d /n", puntaje);
				end_game = true;
			}
		}
	}


	//destruimos todo al finalizar el programa

	free(pieza);
	free(next_pieza);
	al_destroy_timer(timer_de_caida);
	al_destroy_timer(timer_pieza);
	al_destroy_timer(ultimo_movimiento);
	al_destroy_event_queue(event_queue);
	return 0;
}