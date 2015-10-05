#include<stdio.h>
#include<stdlib.h>

void * seacabo();

FILE * archivo = NULL;

int main(int argc, char *argv[])
{
	char * linea = NULL;
	size_t tamlinea = 0;
	ssize_t longcadena;

	// Verifica si se añadio un nombre de archivo al cual guardar
	archivo = argv[1]? fopen(argv[1], "w") : NULL;	
	signal(2, seacabo);

	while((longcadena = getline(&linea, &tamlinea, stdin)) != -1)
	{
		fwrite(linea, longcadena, 1, stdout);
		if(archivo != NULL){ fwrite(linea, longcadena, 1, archivo); }
	}
	fclose(archivo);

	return 0;
}

void * seacabo()
{
	fclose(archivo);
	exit(0);
}
