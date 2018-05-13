/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
	uint8_t tablero[N+5][N]; //correlo de aca que es solo testeo
				//tiene 5 filas de mas que no se ven
int main (void)
{

    srand(time(NULL));
    
 set_board(tablero);
 

            
 
 bloque_t * pieza;
 
pieza=crear_pieza();                    ****************************************************TESTEO GENERAL*******************************************************
 
 
 printf("%d\n",pieza->nombre);
 
		poner_pieza_en_tablero(pieza,tablero,pieza->x,pieza->y);

               

		
                     imprimir_mat(tablero,N+5,N);
           
/*
		clear_sector(tablero,pieza->x,pieza->y,5,5,OCUPADO);

                printf("incremento \n");
		pieza->x++;





		poner_pieza_en_tablero(pieza,tablero,pieza->x,pieza->y);;

		imprimir_mat(tablero,N+5,N);

		printf("ahora rotacion\n");

//testeando la rotacion
                clear_sector(tablero,pieza->x,pieza->y,5,5,OCUPADO);
		rotate(pieza->mat_de_pieza,tablero,pieza->x,pieza->y);


		int valido;

		valido=poner_pieza_en_tablero(pieza,tablero,pieza->x,pieza->y);


//control para las piezas
		
		if (valido)
			imprimir_mat(tablero,N+5,N);

		else
			printf("te olvidaste de borrar wacho\n");

	 

                     
 //testeo fijar
                     printf("fiajdo \n");
                     fijar_bloque(tablero,pieza);
                     imprimir_mat(tablero,N+5,N);
                     
                     int cont=chequear_lineas(tablero,4+2,N); //acordate que las filas visibles arrancan en 4
                     
                     printf("contador %d",cont);
        */             
 //testeando flags
                     
/*    
                    
                     bool jose;
                     pieza->mov_izq=0;
                     jose=chequear_movimiento(pieza,tablero,(pieza->mov_izq),IZQUIERDA);
                     printf("el bool %d", jose);
                     
                     
    free(pieza);
    return 0;
}
*/