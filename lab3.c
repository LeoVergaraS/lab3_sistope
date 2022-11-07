// Lab 3 Sistemas operativos
// Integrantes: Leo Vergara Sepúlveda, Ian Rickmers Blamey
// Rut: 20.820.105-0, 20.284.545-2
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>
#include "funciones.h"
#include "otros.h"

int main(int argc, char *argv[]) {
    // Variables iniciales
    int option;
	char nombreEntrada[255];
	char nombreSalida[255];
	int anioInicio = 0;
	float precioMinimo = 0;
	int hebras = 1;
    int chunk = 1;
    int bandera = 0;
	int requeridoEntrada = 0;
	int requeridoSalida = 0;

    // getopt para leer las flags
	while ((option = getopt(argc, argv, ":i:o:d:p:n:c:b")) != -1)
	{
		switch (option)
		{
		//Nombre entrada
		case 'i':
			requeridoEntrada = 1;
			strcpy(nombreEntrada, optarg);
			break;

		//Nombre salida
		case 'o':
			requeridoSalida = 1;
			strcpy(nombreSalida, optarg);
			break;

		//Año inicio
		case 'd':
			if(atoi(optarg) < 0 || !verificarDecimal(optarg)){
				printf("Error: No se ingreso un año valido.\n");
				return 0;
			}
			anioInicio = atoi(optarg);
			break;

		//Precio minimo
		case 'p':
			if (atof(optarg) < 0 || !verificarDecimal(optarg)){
				printf("Error: No se ingreso un precio valido.\n");
				return 0;
			}
			precioMinimo = atof(optarg);
			break;

		//Cantidad de workers
		case 'n':
			if (atoi(optarg) < 1 || !verificarDecimal(optarg)){
				printf("Error: No se ingreso un numero de workers valido.\n");
				return 0;
			}
			hebras = atoi(optarg);
			break;

        case 'c':
			if (atoi(optarg) < 1 || !verificarDecimal(optarg)){
				printf("Error: No se ingreso un numero de chunks valido.\n");
				return 0;
			}
			chunk = atoi(optarg);
			break;

		//Bandera
		case 'b':
			bandera = 1;
			break;

		//Errores
		case '?':
			printf("Flag no reconocida\n");
			return 0;
		case ':':
			printf("Flag sin argumento\n");
			return 0;
		}
	}

	// Se verifican que el nombre de los archivos se hayan ingresado
	if(requeridoEntrada == 0 || requeridoSalida == 0){
		printf("Error: Falta ingresar el nombre de los archivos de entrada y/o salida.\n");
		return 0;
	}

    printf("%s %s %d %f %d %d %d\n", nombreEntrada, nombreSalida, anioInicio, precioMinimo, hebras, chunk, bandera);
    return 0;
}