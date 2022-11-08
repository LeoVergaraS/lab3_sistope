#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Variables globales
int *anios;
int fin = 0;
int n = 0;
pthread_mutex_t lock;
FILE *archivoEntrada;

int *agregarAnios(int *anios, int anio, int *n);
void *funcionHilo(void *arg);
void escribirArchivo(char * nombreSalida, int anioInicio, int precioMinimo, int bandera);