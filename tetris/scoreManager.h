
#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H
#include <stdlib.h>
#include <stdio.h>

int set_highscore(char * highscore_file, int highscore);
//setea el highscore con el score deseado

int string_to_int(char *string);
//funcion que transforma un string en un int


int get_highscore(char * highscore_file);

#define VALIDO 1
#define INVALIDO 0
#endif