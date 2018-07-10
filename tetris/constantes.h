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
#define ALTURA_LIMITE   6


#define  ABAJO  'a'
#define  DERECHA 'd'
#define  IZQUIERDA 'i'

#define X_INICIAL ((N-4)/2) //posiciones iniciales donde arrancan cada pieza //PARA QUE ARRANCQUE JUSTO A LA MITAD
#define Y_INICIAL 0


enum piezas {PIEZA_L,PIEZA_J,PIEZA_O,PIEZA_S,PIEZA_Z,PIEZA_T,PIEZA_I}; //listado de las piezas 


#endif /* CONSTANTES_H */

