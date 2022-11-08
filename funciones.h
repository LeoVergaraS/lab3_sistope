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

anio *agregarAniosFinales(anio anioAgregado);
int transformarStringABool(char *cadena);
int transformarStringABoolLinus(char *cadena);
juego lineaToJuego(char *linea);
int juegoInAnios(juego juego);
anio crearAnioVacio(juego juego);
anio crearAnio(juego juego);
void juntarAnios(juego juego);
void *funcionHilo(void *arg);
int buscarIndice(int anioInicio);
void escribirArchivo(char * nombreSalida, int anioInicio, int bandera);
void merge(anio* aniosFinal,int inicio,int medio,int fin);
void mergeSort(anio* aniosFinal, int ini, int fin);