#include "allegro_fun.h"
#include"constantes.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


void dibujar_tablero(ALLEGRO_BITMAP * bitmap, ALLEGRO_DISPLAY * display, uint8_t mat[N + 5][N],uint16_t color)
{
	char * colores[] = { "midnightblue","mediumspringgreen","goldenrod","maroon","deeppink","orange","magenta" };
	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_color_name("black"));



	int i, j;
	for (i = (ALTURA_LIMITE - 2); i < N + 5; i++)
	{
		for (j = 1; j < N-1; j++)
		{

			if ((mat[i][j] == OCUPADO) /*|| (mat[i][j] == FIJO)*/) 
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name(colores[color]));
				al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}
			else if (mat[i][j] == FIJO)
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("cornsilk"));
				al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}
			//else if (mat[i][j] == VACANTE)
			//{
			//	//al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			//}
			else if ((mat[i][j] == BORDE))
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("red"));
				al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}
			if ((i == (ALTURA_LIMITE - 2)))
			{
				al_draw_filled_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("red"));
				al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}

			//al_draw_rectangle(ANCHO_BLOQUE*j, ALTURA_BLOQUE*i, ANCHO_BLOQUE + ANCHO_BLOQUE * j, ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
		}

	}
	

	al_set_target_bitmap(al_get_backbuffer(display));
	al_draw_bitmap(bitmap, 240, 50, 0);
	al_flip_display();
}

void start_game_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev, ALLEGRO_BITMAP * start_image, ALLEGRO_DISPLAY * display, bool * start_game, bool * do_exit)
{
	while (!(*start_game))
	{
		al_draw_bitmap(start_image, 0, 0, 0); //cargo el menu de game over
		al_flip_display();

		if (al_get_next_event(event_queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_KEY_UP)    //control tecla levantada
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ENTER:
				{
					*start_game = TRUE;
					break;
				}

				case ALLEGRO_KEY_ESCAPE:
				{
					*start_game = TRUE;
					*do_exit = TRUE;
					break;
				}
				}
			}

			al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
			al_clear_to_color(al_color_name("black"));

		}
	}
}




void end_game_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev,ALLEGRO_TIMER* timer_pieza, ALLEGRO_TIMER *ultimo_movimiento, ALLEGRO_DISPLAY *display, ALLEGRO_FONT * tipo_letra ,ALLEGRO_FONT * tipo_letra_pausa_1, uint16_t puntaje,uint16_t highscore,bool *restart, bool * do_exit)
{
	bool end_game = TRUE;

	al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
	al_clear_to_color(al_color_name("black")); //para actualizar el score y que no se superponga


	

	al_stop_samples();
	al_stop_timer(timer_pieza); //paro los dos timer 
	al_stop_timer(ultimo_movimiento);

	

	al_draw_text(tipo_letra_pausa_1, al_color_name("royalblue"), 130, 135, 0, "GAME OVER");

	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), ESC_X, ESC_Y+OFFSET*4, 0, "PRESS ESC TO EXIT");
	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), RESTART_X, RESTART_Y + OFFSET*6, 0, "PRESS R TO RESTART");
	if (puntaje > highscore)
	{
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), HSCORE_X, SCORE_Y + OFFSET * 2, 0, "NEW HIGHSCORE: %d",puntaje);
	}
	else
	{
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), SCORE_X, SCORE_Y+ OFFSET * 2, 0, "SCORE : %d", puntaje);
	}
	
	

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
					*do_exit = TRUE;
					end_game = FALSE;
					break;
				}
				case ALLEGRO_KEY_R:     //caso reinicio
				{
					*restart = TRUE;       //pongo como true asi puedo reiniciar el juego
					end_game = FALSE;
					break;
				}

				}

			}
		}
	}
	
	al_start_timer(timer_pieza);    //vuelvo a iniciar los timers
	al_start_timer(ultimo_movimiento);

	al_flush_event_queue(event_queue);
	al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
	al_clear_to_color(al_color_name("black"));
}

void pause_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display,ALLEGRO_BITMAP * pause_image, ALLEGRO_TIMER * timer_pieza, ALLEGRO_TIMER * ultimo_movimiento, ALLEGRO_SAMPLE * sample,bool * do_exit)
{
	bool pausa = FALSE;
	al_stop_timer(timer_pieza); //paro los dos timers 
	al_stop_timer(ultimo_movimiento);
	al_stop_samples();


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
				{
					pausa = TRUE;
					al_play_sample(sample, 1.0, 1.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					break;
				}
				case ALLEGRO_KEY_ESCAPE:    //si se aprieta escape en medio de la pausa cierro todo
				{
					pausa = TRUE;
					*do_exit = TRUE;
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
	al_flush_event_queue(event_queue);

	al_flip_display();
}

void print_score(ALLEGRO_FONT *tipo_letra, uint16_t puntaje,uint16_t highscore, uint16_t nivel, ALLEGRO_DISPLAY *display,ALLEGRO_BITMAP *board, uint8_t tablero[N + 5][N],bloque_t * next_bloque,uint16_t color_pieza)
{
	char * colores[] = { "midnightblue","mediumspringgreen","goldenrod","maroon","deeppink","orange","magenta" };
	al_set_target_bitmap(al_get_backbuffer(display)); //vuelvo a pintar de negro el bitmap
	al_clear_to_color(al_color_name("black")); //para actualizar el score y que no se superponga

	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), POS_X - 70, POS_Y, 0, "HIGH SCORE:");
	if (puntaje > highscore)
	{
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), POS_X + OFFSET * 3, POS_Y + OFFSET, 0, "%d", puntaje);
	}
	else
	{
		al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), POS_X + OFFSET * 3, POS_Y + OFFSET, 0, "%d", highscore);
	}
	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), POS_X, POS_Y + OFFSET* 5, 0, "SCORE:");
	al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), POS_X+ OFFSET * 3, POS_Y + OFFSET * 6, 0, "%d", puntaje);

	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), POS_X+9, POS_Y + OFFSET * 10, 0, "LEVEL:");
	al_draw_textf(tipo_letra, al_color_name("lightsteelblue"), POS_X + OFFSET * 3, POS_Y + OFFSET * 11, 0, "%d", nivel);

	al_draw_text(tipo_letra, al_color_name("lightsteelblue"), POS_X +NEXTTXT_X, POS_Y, 0, "NEXT:");

	int i, j;
	for (i = 0; i < SIZE; i++)	//para la pieza que sigue
	{
		for (j = 0; j < SIZE; j++)
		{
			if ((next_bloque->mat_de_pieza[i][j]) == OCUPADO)
			{
				al_draw_filled_rectangle(NEXT_X + ANCHO_BLOQUE * j, OFFSET + POS_Y +ALTURA_BLOQUE*i, NEXT_X + ANCHO_BLOQUE + ANCHO_BLOQUE * j, OFFSET + POS_Y + ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name(colores[(next_bloque->nombre)]));
				al_draw_rectangle(NEXT_X + ANCHO_BLOQUE * j, OFFSET + POS_Y+ALTURA_BLOQUE*i, NEXT_X + ANCHO_BLOQUE + ANCHO_BLOQUE * j, OFFSET + POS_Y+ANCHO_BLOQUE + ALTURA_BLOQUE * i, al_color_name("purple"), 1);
			}
		}
	}

	dibujar_tablero(board, display, tablero,color_pieza);  //siempre dibujo el tablero

}


