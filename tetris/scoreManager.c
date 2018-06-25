#include "scoreManager.h"
int get_highscore(char * highscore_file)
{
	FILE * document;
	char test[50];
	int i = 0;
	char c = 0;
	int numero = 0;
	document = fopen(highscore_file, "r");
	if (document == NULL)
	{
		numero = INVALIDO;
		fclose(document);
	}
	else
	{
		while (!(feof(document)))
		{
			c = fgetc(document);
			if ((c <= '9') && (c >= '0'))
			{
				test[i] = c;
				i++;
			}
		}
		test[i] = '\0';


		fclose(document);


		numero = string_to_int(test);
	}
	
	return numero;
}

int string_to_int(char *string)
{
	int i = 0;
	int numero = 0;
	int aux;
	for (i = 0; ((string[i]) != '\0'); i++)
	{
		if ((string[i] <= '9') && (string[i] >= '0'))
		{
			aux = (string[i] - '0');
			numero = (numero * 10) + aux;

		}
	}
	return numero;
}

int set_highscore(char * highscore_file, int highscore)
{
	FILE * document;
	int i = 0;
	char c = 0;
	int numero = 0;
	document = fopen(highscore_file, "w+");
	if (document == NULL)
	{
		numero = INVALIDO;
	}
	else {

		fprintf(document, "%d", highscore);
		numero = VALIDO;
	}
	fclose(document);
	return numero;
}