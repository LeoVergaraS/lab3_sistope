#ifndef ANIO_H
#define ANIO_H

typedef struct year{
    int anio;
    int cantidadJuegos;
    char nombreCaro[255];
    float caro;
    char nombreBarato[255];
    float barato;
    float sumaPrecios;
    int cantidadWindows;
    int cantidadMac;
    int cantidadLinux;
    char juegosGratis[2000];
    int cantidadJuegosGratis;
} anio;

#endif