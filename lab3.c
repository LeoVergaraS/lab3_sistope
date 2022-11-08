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

// Variables globales

pthread_t *tid;

int main(int argc, char *argv[]) {
    // Variables iniciales
    int option, i;
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

    // Se asigna memoria al arreglo de los id de las hebras
    tid = (pthread_t *)malloc(sizeof(pthread_t) * hebras);

    // Se abre el archivo de entrada
    archivoEntrada = fopen(nombreEntrada, "r");
    if(archivoEntrada == NULL){
        printf("Error: No se pudo abrir el archivo de entrada.\n");
        return 0;
    }

    // Se abre el lock
    if(pthread_mutex_init(&lock, NULL) != 0){
        printf("Error: No se pudo crear el mutex.\n");
        return 0;
    }

    // Se crean las hebras
    i = 0;
    while(!feof(archivoEntrada)){
        if(pthread_create(&(tid[i]), NULL, funcionHilo, (void*)&chunk) != 0){
            printf("Error: No se pudo crear la hebra.\n");
            return 0;
        }

    }

    
    // Se espera a que terminen las hebras
    for(i = 0; i < hebras; i++){
        if(pthread_join(tid[i], NULL) != 0){
            printf("Error: No se pudo unir la hebra.\n");
            return 0;
        }
    }

    // Se cierra el archivo de entrada
    fclose(archivoEntrada);

    // Se cierra el lock
    pthread_mutex_destroy(&lock);

	escribirArchivo(nombreSalida, anioInicio, precioMinimo, bandera);       

    // Se libera memoria
    free(tid);
    free(anios);
    return 0;
}