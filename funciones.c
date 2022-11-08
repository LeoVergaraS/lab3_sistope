#include "funciones.h"

int *agregarAnios(int *anios, int anio, int *n){
    int *aux = (int *)malloc(sizeof(int)*(*n+1));
    for(int i = 0; i < *n; i++){
        aux[i] = anios[i];
    }
    aux[*n] = anio;
    *n = *n + 1;
    free(anios);
    return aux;
}

void *funcionHilo(void *arg) {
    pthread_mutex_lock(&lock);
    int *chunk = (int *)arg;
    int i = 0;
    char linea[500];
    while(fgets(linea, 500, archivoEntrada) != NULL && i < *chunk){
        printf("%s", linea);
        i++;
    }
    pthread_mutex_unlock(&lock);    
    return NULL;
}

void escribirArchivo(char * nombreSalida, int anioInicio, int precioMinimo, int bandera){
    FILE *archivoSalida;
    archivoSalida = fopen(nombreSalida, "w");
    if(archivoSalida == NULL){
        printf("Error: No se pudo abrir el archivo de salida.\n");
        return;
    }
    if(bandera == 1){
        printf("El año con el precio mínimo es %d con un precio de %d.\n", anioInicio, precioMinimo);
    }
    fprintf(archivoSalida, "El año con el precio mínimo es %d con un precio de %d.\n", anioInicio, precioMinimo);
    fprintf(archivoSalida, "No se encontró un año con el precio mínimo.\n");
    fclose(archivoSalida);
}