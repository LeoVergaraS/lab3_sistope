#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <pthread.h>
#include "anio.h"
#include "juego.h"

int *agregarAnios(int *anios, int anio, int *n);
void *funcionHilo(void *arg);
void escribirArchivo(char * nombreSalida, int anioInicio, int precioMinimo, int bandera);
int transformarStringABool(char *cadena);
int transformarStringABoolLinus(char *cadena);
juego lineaToJuego(char *linea);
void merge(anio* aniosFinal,int inicio,int medio,int fin);
void mergeSort(anio* aniosFinal, int ini, int fin);