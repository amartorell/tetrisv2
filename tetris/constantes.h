/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   constantes.h
 * Author: ariel
 *
 * Created on December 12, 2017, 11:12 AM
 */

#ifndef CONSTANTES_H
#define CONSTANTES_H


//definicion de bool/////////////////////

typedef int bool;
#define TRUE 1
#define FALSE 0

////////////////////////////////////////


#define N 16		//n tiene que ser multiplo de cuatro
#define VALIDO 1
#define INVALIDO 0

#define SIZE 5  //tamaño de la matriz donde se ubica el bloque

#define VACANTE 0
#define OCUPADO 1
#define FIJO 2	
#define BORDE 3
#define ALTURA_LIMITE   3


#define  ABAJO  'a'
#define  DERECHA 'd'
#define  IZQUIERDA 'i'

#define X_INICIAL ((N-4)/2) //posiciones iniciales donde arrancan cada pieza //PARA QUE ARRANCQUE JUSTO A LA MITAD
#define Y_INICIAL 1



////////////////////////////////////////constastes allegro///////////////////////////////////////////////////

#define SCREEN_W		800
#define SCREEN_H		600
#define ALTURA_BLOQUE           20
#define ANCHO_BLOQUE            20
#define TIEMPO_CAIDA            10
#define TIEMPO_ESPERA_ULTIMO_MOV           0.500
#define TIEMPO                  0.500   

#define BARRERA_NIVEL           1
#define NIVEL_INICIAL           1
#define PUNTAJE_INICIAL         0   

#define MINIMO_X                0
#define MAXIMO_X                N

#define MAXIMO_Y                (N+5) 
#define MAX_NIVEL               3

#define POS_X 100
#define POS_Y 150
#define OFFSET 24

enum MYKEYS{KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT};

enum piezas {PIEZA_L,PIEZA_J,PIEZA_O,PIEZA_S,PIEZA_Z,PIEZA_T,PIEZA_I}; //listado de las piezas 


#endif /* CONSTANTES_H */

