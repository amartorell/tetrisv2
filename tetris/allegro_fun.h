

#ifndef ALLEGRO_FUN_H
#define ALLEGRO_FUN_H
#include "constantes.h"
#include"manejomat.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> 
#include <allegro5/allegro_primitives.h> 
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h> 
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "constantesAllegro.h"


void print_score(ALLEGRO_FONT *tipo_letra, uint16_t puntaje, uint16_t highscore, uint16_t nivel, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *board, uint8_t tablero[N + 5][N],bloque_t * next_bloque,uint16_t color_pieza);


void dibujar_tablero(ALLEGRO_BITMAP * bitmap, ALLEGRO_DISPLAY * display, uint8_t mat[N + 5][N], uint16_t color);
//funcion que dibuja en el display como va el tablero de juego mostrando sus piezas 

void start_game_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev, ALLEGRO_BITMAP * start_image, ALLEGRO_DISPLAY * display, bool * start_game, bool * do_exit);


void end_game_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev, ALLEGRO_TIMER* timer_pieza, ALLEGRO_TIMER *ultimo_movimiento, ALLEGRO_DISPLAY *display, ALLEGRO_FONT * tipo_letra, ALLEGRO_FONT * tipo_letra_pausa_1, uint16_t puntaje, uint16_t highscore, bool *restart, bool * do_exit);

void pause_scenario(ALLEGRO_EVENT_QUEUE * event_queue, ALLEGRO_EVENT ev,ALLEGRO_DISPLAY * display, ALLEGRO_BITMAP * pause_image, ALLEGRO_TIMER * timer_pieza, ALLEGRO_TIMER * ultimo_movimiento, ALLEGRO_SAMPLE * sample,bool * do_exit);


#endif /* ALLEGRO_FUN_H */