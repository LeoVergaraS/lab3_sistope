#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char linea[500];
    char err[500];
    FILE *archivoEntrada = fopen("entrada.txt", "r");
    while( err != NULL){
        strcpy(err,fgets(linea, 500, archivoEntrada));
        printf("%s", linea);
        printf("%s\n", err);
        //sleep(1);
    }
    return 0;
}