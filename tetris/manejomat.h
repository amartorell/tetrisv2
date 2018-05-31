/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 /*
  * File:   manejomat.h
  * Author: ariel
  *
  * Created on December 12, 2017, 11:10 AM
  */

#ifndef MANEJOMAT_H
#define MANEJOMAT_H


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "constantes.h"






  /////////////////////////////////////////////////definicion de la estructura de la pieza///////////////////////////////////


typedef  struct
{
	uint16_t nombre;    //es un int como se usa un enum para crear la lista de piezas
	uint8_t mat_de_pieza[5][5];
	uint16_t x;
	uint16_t y;
	bool mov_vertical;  //flags que indican si se puede mover en alguno de los sentidos permitidos
	bool mov_der;
	bool mov_izq;


} bloque_t;


void mover_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza, char tipo_de_movimiento);
//funcion que se fija en las 3 direcciones en las que se mueve el bloque y si puede lo mueve

void rotar_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza);

void copiar_pieza(bloque_t * bloque, bloque_t * nextbloque);
//copia el contenido del nextbloque a bloque 

int clear_sector(uint8_t tablero[N + 5][N], uint16_t pos_x, uint16_t pos_y, uint16_t largo, uint16_t alto, uint8_t condicion);
//limpia un pequeño sector de la matriz principal del juego indicando el alto y el largo y se fija una condicion de que cosas borrar

int poner_pieza_en_tablero(bloque_t * pieza, uint8_t tablero[N + 5][N], uint16_t pos_inicial_x, uint16_t pos_inicial_y);

int rotate(bloque_t *pieza, uint8_t tablero[N + 5][N]);
//funcion que rota las piezas 

void estampar_bloque(bloque_t * pieza, uint8_t tablero[N + 5][N],uint16_t * puntaje);

void set_board(uint8_t tablero[N + 5][N]);
//setea el tablero antes de arrancar el juegp definiendo los bordes

void set_mat_in_block(uint8_t mat1[SIZE][SIZE], uint8_t mat[SIZE][SIZE]);
//setea la matriz de la estructura de la pieza en la matriz que le corresponde
//mat1 es la matriz de la estructura
//mat2 es la matriz fija donde se encuentra el bloque
//se lo usa en la creacion de la pieza


bloque_t * crear_pieza(void);
//funcion que crea una pieza aleatoriamente y devuelve un puntero a esta pieza



bool chequear_movimiento(bloque_t * p2pieza, uint8_t tablero[N + 5][N], bool movimiento, char tipo_de_movimiento);
//funcion que chequea que las piezas se puedan mover segun el sentido que se quiera averiguar y devuelve si se puede seguir moviendo


int chequear_lineas(uint8_t tablero[N + 5][N], uint16_t fila, uint16_t largo_fila, int condicion);
//funcion que se fija cuantos elementos con esa condicion hay en una fila y devuelve esa cantidad



int fijar_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza);
//funcion que deja fijo los bloques una vez que estos lleguen a destino


int can_rotate(uint8_t mat_pieza[SIZE][SIZE], uint8_t tablero[N + 5][N], uint16_t pos_x, uint16_t pos_y);
//funcion que controla si se puede realizar la rotacion en la posicion del tablero que se desea usar

void limpiar_matriz(uint8_t tablero[N + 5][N], uint16_t * puntaje);
//funcion que limpia las lineas una vez que se hayan completado y aumenta el puntaje

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////FUNCIONES TESTEO/////////////////////////////////////////////////////////////


void imprimir_mat(uint8_t mat[N + 5][N], int filas, int columnas);
//imprime el tablero de juego



void imprimir(uint8_t mat1[SIZE][SIZE]);
////////////////////////////////////////////////////////////////////////////////////////////////////////




#endif /* MANEJOMAT_H */

