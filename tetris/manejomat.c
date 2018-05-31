/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "manejomat.h"

 //////////definicion de las matrices de las disntintas piezas///////////////////////

uint8_t piezaL[5][5] = { {0,0,0,0,0},
						  {0,0,1,0,0},
						  {0,0,1,0,0},
						  {0,0,1,1,0},
						  {0,0,0,0,0} };

uint8_t piezaJ[5][5] = { {0,0,0,0,0},
						  {0,0,1,0,0},
						  {0,0,1,0,0},
						  {0,1,1,0,0},
						  {0,0,0,0,0} };

uint8_t piezaO[5][5] = { {0,0,0,0,0},
						  {0,0,0,0,0},
						  {0,1,1,0,0},
						  {0,1,1,0,0},
						  {0,0,0,0,0} };

uint8_t piezaS[5][5] = { {0,0,0,0,0},
						  {0,0,1,1,0},
						  {0,1,1,0,0},
						  {0,0,0,0,0},
						  {0,0,0,0,0} };

uint8_t piezaZ[5][5] = { {0,0,0,0,0},
						  {0,1,1,0,0},
						  {0,0,1,1,0},
						  {0,0,0,0,0},
						  {0,0,0,0,0} };

uint8_t piezaT[5][5] = { {0,0,0,0,0},
						  {0,0,0,0,0},
						  {0,1,1,1,0},
						  {0,0,1,0,0},
						  {0,0,0,0,0} };


uint8_t piezaI[5][5] = { {0,0,1,0,0},
						  {0,0,1,0,0},
						  {0,0,1,0,0},
						  {0,0,1,0,0},
						  {0,0,0,0,0} };

////////////////////// termina la definicion de las piezas////////////////////////////////////////////////


























//////////funciones de testeo///////////////////////////////////////////////////////////////////////////////////////////////////////////


void imprimir_mat(uint8_t mat[N + 5][N], int filas, int columnas)
{
	int i;
	int j;
	for (i = 0; i < filas; i++)
	{
		for (j = 0; j < columnas; j++)
		{


			printf("%d ", mat[i][j]);


		}
		printf("\n");
	}

}





///////////////////////////////////////////////////// funciones manejo interno del tetris //////////////////////////////////////////////////////////////////////////////////////////////////







int rotate(bloque_t *pieza, uint8_t tablero[N + 5][N])
{

	uint8_t maux[SIZE][SIZE];
	int i;
	int j;
	int validez = INVALIDO;

	if ((pieza->nombre) == PIEZA_O)
	{
		return VALIDO;     //la pieza O no realiza ninguna rotacion ya que es cuadrado asi que esta devuelve la misma pieza
	}
	else
	{

		for (i = 0; i < SIZE; i++)       //hago la rotacion de la pieza en una matriz auxiliar
		{
			for (j = 0; j < SIZE; j++)
			{
				maux[j][i] = pieza->mat_de_pieza[i][4 - j];
			}
		}


		//ahora con esa rotacion ya hecha paso a buscar donde poner en el tablero la pieza ya que esta al rotar se mueve

		if (can_rotate(maux, tablero, pieza->x, pieza->y))
		{
			validez = VALIDO;

		}
		else if (can_rotate(maux, tablero, (pieza->x) + 1, (pieza->y)))  //me corro un lugar a la derecha para ver si asi puedo rotarla
		{
			validez = VALIDO;
			pieza->x++; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
		else if (((pieza->x) > 0) && (can_rotate(maux, tablero, (pieza->x) - 1, (pieza->y))))  //me corro un lugar a la izquierda para ver si asi puedo rotarla
		{																			 //hace el chequeo de que sea mayor a 0 para que no haga overflow
			validez = VALIDO;
			pieza->x--; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
		else if (can_rotate(maux, tablero, (pieza->x), (pieza->y) - 1))  //me corro un lugar hacia arriba para ver si asi puedo rotarla
		{
			validez = VALIDO;
			pieza->y--; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
		//sino funciona corriendome un lugar me corro dos ahora

		else if (can_rotate(maux, tablero, (pieza->x) + 2, (pieza->y)))  //me corro 2 lugares a la derecha para ver si asi puedo rotarla
		{
			validez = VALIDO;
			pieza->x += 2; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
		else if (((pieza->x) > 1) && (can_rotate(maux, tablero, (pieza->x) - 2, (pieza->y))))  //me corro 2 lugares a la izquierda para ver si asi puedo rotarla
		{																				//hago ese cheque para no evitar overflow
			validez = VALIDO;
			pieza->x -= 2; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
		else if (can_rotate(maux, tablero, (pieza->x), (pieza->y) - 2))  //me corro 2 lugares a la derecha para ver si asi puedo rotarla
		{
			validez = VALIDO;
			pieza->y -= 2; //aumento como debo cambiar la coordenada si resulta que la pieza se mueve

		}
	}

	//ahora realizo la rotacion si es que se puede
	if (validez)
	{
		for (i = 0; i < SIZE; i++)
		{
			for (j = 0; j < SIZE; j++)
			{
				pieza->mat_de_pieza[i][j] = maux[i][j];
			}
		}
	}


	return validez;

}


int can_rotate(uint8_t mat_pieza[SIZE][SIZE], uint8_t tablero[N + 5][N], uint16_t pos_x, uint16_t pos_y)
{
	int i, j;
	int validez = VALIDO;

	for (i = 0; ((i < SIZE) && validez); i++)
	{
		for (j = 0; ((j < SIZE) && validez); j++)
		{
			if ((mat_pieza[i][j] == OCUPADO) && ((tablero[i + pos_y][j + pos_x] == FIJO) || (tablero[i + pos_y][j + pos_x] == BORDE)))
			{
				validez = INVALIDO;
			}
		}
	}
	return validez;
}





int poner_pieza_en_tablero(bloque_t * pieza, uint8_t tablero[N + 5][N], uint16_t pos_inicial_x, uint16_t pos_inicial_y)
{
	int valido = VALIDO;
	int i, j;
	for (i = 0; ((i < 5) && (valido)); i++)
	{
		for (j = 0; ((j < 5) && (valido)); j++)
		{
			if (((tablero[pos_inicial_y + i][pos_inicial_x + j] == VACANTE) && (pieza->mat_de_pieza[i][j] == OCUPADO)) && (tablero[pos_inicial_y + i][pos_inicial_x + j] != BORDE))
			{

				tablero[pos_inicial_y + i][pos_inicial_x + j] = pieza->mat_de_pieza[i][j];
			}

		}
	}


	return valido;
}


void mover_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza, char tipo_de_movimiento)
{
	if (tipo_de_movimiento == ABAJO)
	{

		(pieza->mov_vertical) = chequear_movimiento(pieza, tablero, pieza->mov_vertical, tipo_de_movimiento);
		if ((pieza->mov_vertical))
		{
			clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
			pieza->y++;
			poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);
		}
	}
	else if (tipo_de_movimiento == IZQUIERDA)
	{
		(pieza->mov_izq) = chequear_movimiento(pieza, tablero, pieza->mov_izq, IZQUIERDA);
		if (pieza->mov_izq)
		{
			clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
			pieza->x--;
			poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);

		}
	}
	else if (tipo_de_movimiento == DERECHA)
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

void rotar_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza)
{

	if (rotate(pieza, tablero))
	{
		clear_sector(tablero, pieza->x, pieza->y, SIZE + 1, SIZE + 1, OCUPADO);
		poner_pieza_en_tablero(pieza, tablero, pieza->x, pieza->y);


	}
}

void copiar_pieza(bloque_t * bloque, bloque_t * nextbloque)
{
	set_mat_in_block(bloque->mat_de_pieza, nextbloque->mat_de_pieza);
	(bloque->mov_der) = (nextbloque->mov_der);
	(bloque->mov_izq) = (nextbloque->mov_izq);
	(bloque->mov_vertical) = (nextbloque->mov_vertical);
	(bloque->nombre) = (nextbloque->nombre);
	(bloque->x) = (nextbloque->x);
	(bloque->y) = (nextbloque->y);
}

int clear_sector(uint8_t tablero[N + 5][N], uint16_t pos_x, uint16_t pos_y, uint16_t largo, uint16_t alto, uint8_t condicion)
{
	int i, j;
	if (condicion == OCUPADO)
	{
		if (pos_y > 0)
		{

			for (i = (-1); i <= alto; i++)    //ARRANCAN EN -1 EN ESTE CASO PARA LIMPIAR LA POSICION DE LA PIEZA QUE ESTABA ARRIBA DE ESTOS SI ES QUE EL ARREGLO ES MAS LARGO
			{
				for (j = (-1); j <= largo + 1; j++)
				{

					if (tablero[pos_y + i][pos_x + j] == condicion)	//limpia solo segun la condicion que se le da
					{												//para no borrar bloques que ya estan fijos por ejemplo					
						tablero[pos_y + i][pos_x + j] = VACANTE;		//o para borrar una fila que ya se completo
					}
				}
			}

		}
		else if (pos_y == 0)
		{
			for (i = (0); i <= alto; i++)    //ARRANCAN EN -1 EN ESTE CASO PARA LIMPIAR LA POSICION DE LA PIEZA QUE ESTABA ARRIBA DE ESTOS SI ES QUE EL ARREGLO ES MAS LARGO
			{
				for (j = (-1); j <= largo + 1; j++)
				{

					if (tablero[pos_y + i][pos_x + j] == condicion)	//limpia solo segun la condicion que se le da
					{												//para no borrar bloques que ya estan fijos por ejemplo					
						tablero[pos_y + i][pos_x + j] = VACANTE;		//o para borrar una fila que ya se completo
					}
				}
			}
		}

	}
	else
	{
		for (i = 0; i < alto; i++)
		{
			for (j = 0; j < largo; j++)
			{
				if (tablero[pos_y + i][pos_x + j] == condicion)	//limpia solo segun la condicion que se le da
				{												//para no borrar bloques que ya estan fijos por ejemplo					
					tablero[pos_y + i][pos_x + j] = VACANTE;		//o para borrar una fila que ya se completo
				}
			}
		}

	}

	return 0;
}





void estampar_bloque(bloque_t * pieza, uint8_t tablero[N + 5][N],  uint16_t * puntaje)
{
	fijar_bloque(tablero, pieza);
	limpiar_matriz(tablero, puntaje);
}

void set_board(uint8_t tablero[N + 5][N])
{
	int i, j;
	for (i = 0; i < N + 5; i++)
	{
		for (j = 0; j < N; j++)
		{
			if ((i == (N + 4)) || (j < 2) || (j > N - 3))
			{
				tablero[i][j] = BORDE;
			}
			else
			{
				tablero[i][j] = VACANTE;
			}
		}
	}
}



bloque_t * crear_pieza(void)
{
	uint16_t tipo_pieza = (rand() % 7);
	bloque_t * p2block;

	p2block = (bloque_t*)malloc(sizeof(bloque_t));

	switch (tipo_pieza) //para setear que arreglo va en cada pieza
	{
	case PIEZA_L:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaL);
		break;

	}
	case PIEZA_J:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaJ);
		break;
	}
	case PIEZA_O:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaO);
		break;
	}
	case PIEZA_S:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaS);
		break;
	}
	case PIEZA_Z:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaZ);
		break;
	}
	case PIEZA_T:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaT);
		break;
	}
	case PIEZA_I:
	{
		p2block->nombre = tipo_pieza;
		set_mat_in_block((p2block->mat_de_pieza), piezaI);
		break;
	}
	default:
	{
		p2block = NULL; //hubo algun error entonces devuelve NULL
	}

	}   //termina el switch

	p2block->x = X_INICIAL;   //pongo las coordenadas iniciales
	p2block->y = Y_INICIAL;

	//pongo como verdadero ya que la pieza va estar mas o menos centrada
	p2block->mov_vertical = TRUE;
	p2block->mov_der = TRUE;
	p2block->mov_izq = TRUE;

	return p2block;
}

void set_mat_in_block(uint8_t mat1[SIZE][SIZE], uint8_t mat2[SIZE][SIZE])
{
	int i, j;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			mat1[i][j] = mat2[i][j];
		}
	}
}


/*  funcion minimalista de prueba BORRAR DESPUES

	void imprimir(uint8_t mat1[SIZE][SIZE])
{
	int i,j;

	for (i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
		   printf("%d",mat1[i][j]);
		}
		printf("\n");
	}
}
*/




bool chequear_movimiento(bloque_t * p2pieza, uint8_t tablero[N + 5][N], bool movimiento, char tipo_de_movimiento)
{
	int i, j;
	uint16_t x = (p2pieza->x);
	uint16_t y = (p2pieza->y);

	bool control = TRUE;    //siempre en 1 para poder chequear el caso en que antes no me podia mover y ahora si

	if ((tipo_de_movimiento == ABAJO) && (control == TRUE))
	{
		for (i = 0; (i < SIZE) && (i < (N + 5) && control); i++)
		{
			for (j = 0; ((j < SIZE) && control); j++)
			{
				if (((p2pieza->mat_de_pieza[i][j]) == OCUPADO) && ((tablero[i + y + control][j + x] == FIJO) || (tablero[i + y + control][j + x] == BORDE))) //me fijo si adelantandome un paso
				{                                                                                                                                       //llego a un borde o a dponde ya habian bloques
					control = FALSE;                                                                                                        //ESTO SE LLEVA ACABO SUMANDOLE EL CONTROL A LAS POSICIONES
				}
			}
		}

	}

	else if ((tipo_de_movimiento == IZQUIERDA) && (control == TRUE))
	{
		for (i = 0; (i < SIZE) && (i < (N + 5) && control); i++)
		{
			for (j = 0; ((j < SIZE) && control); j++)
			{
				if (((p2pieza->mat_de_pieza[i][j]) == OCUPADO) && ((tablero[i + y][j + x - control] == FIJO) || (tablero[i + y][j + x - control] == BORDE))) //me fijo si adelantandome un paso
				{                                                                                                                                       //llego a un borde o a dponde ya habian bloques
					control = FALSE;
				}
			}
		}

	}

	else if ((tipo_de_movimiento == DERECHA) && (control == TRUE))
	{
		for (i = 0; (i < SIZE) && (i < (N + 5) && control); i++)
		{
			for (j = 0; ((j < SIZE) && control); j++)
			{
				if (((p2pieza->mat_de_pieza[i][j]) == OCUPADO) && ((tablero[i + y][j + x + (control)] == FIJO) || (tablero[i + y][j + x + (control)] == BORDE))) //me fijo si adelantandome un paso
				{                                                                                                                                       //llego a un borde o a dponde ya habian bloques
					control = FALSE;
				}
			}
		}

	}

	return control;
}





int chequear_lineas(uint8_t tablero[N + 5][N], uint16_t fila, uint16_t largo_fila, int condicion)
{
	int i;
	int contador = 0;

	if (fila <= N + 3)   //porque solo puede chequear hasta una antes del borde
	{

		for (i = 0; i < largo_fila; i++)
		{
			if (tablero[fila][i] == condicion)
			{
				contador++;
			}
		}

	}

	return contador;

}

int fijar_bloque(uint8_t tablero[N + 5][N], bloque_t * pieza)
{
	int i, j;
	uint16_t x = (pieza->x);
	uint16_t y = (pieza->y);

	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if ((pieza->mat_de_pieza[i][j]) == OCUPADO)
			{
				tablero[y + i][x + j] = FIJO;
			}
		}
	}
	//free(pieza);
	return 0;
}


void limpiar_matriz(uint8_t tablero[N + 5][N], uint16_t * puntaje)
{
	int i, j, k;

	int control;
	int contador = 0; //contador de cuantas veces se cumpleto una linea




	for (i = N + 4; i >= 0; i--)
	{
		if ((control = (chequear_lineas(tablero, i, N, FIJO))) == (N - 4))	//me fijo si la fila esta llena de espacios fijos
		{
			contador++;
			for (k = i; k > 0; k--)	//si la fila esta llena de espacios fijos 
			{					//desplazo para abajo
				for (j = 0; j < N; j++)
				{
					tablero[k][j] = tablero[k - 1][j];
					i++;
				}
			}

		}
	}

	(*puntaje) += contador;
}

