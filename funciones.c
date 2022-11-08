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

// Función que transforma un string Yes, No, True o False a un entero 0 o 1
// Entrada: string
// Salida: entero
int transformarStringABool(char *cadena)
{
    // Si es Yes o True, retorna 1
    if (strcmp(cadena, "Yes") == 0 || strcmp(cadena, "True") == 0)
    {
        return 1;
    }
    // Si es No o False, retorna 0
    else 
    {
        return 0;
    }

}

//Función que verifica si es que el primer elemento es Y, representando un Yes
//Entrada: un String
//Salida: un booleano
int transformarStringABoolLinus(char *cadena)
{
    // Si es Yes o True, retorna 1
    if (cadena[0]=='Y')
    {
        return 1;
    }
    // Si es No o False, retorna 0
    else 
    {
        return 0;
    }

}

juego lineaToJuego(char *linea){
    // Variables iniciales
    juego juego;
    char prox[255], gratis[255], win[255], mac[255], lin[255];

    // Separar la linea
    sscanf(linea, "%d,%[^,],%d,%f,%[^,],%d,%[^,],%[^,],%[^,],%[^,]", 
    &juego.id, juego.nombre, &juego.restriccion, &juego.precio, prox, &juego.fecha,
    gratis, win, mac, lin);

    // Convertir los strings a int
    juego.proximamente = transformarStringABool(prox);
    juego.gratis = transformarStringABool(gratis);
    juego.win = transformarStringABool(win);
    juego.mac = transformarStringABool(mac);
    juego.lin = transformarStringABoolLinus(lin);    
    return juego;
}

//Descripción: Función que junta lun año calculado por un worker y los guarda en un array con los datos combinados
//Entrada: arreglo de años, año, lista de posibles años, lista de si el año ya está dentro del array de años, cantidad de años
//Salida: arreglo de años con el año agregado
anio* juntarAnios(anio* aniosFinal, anio anio, int* anios, int* anioDentro, int cantAnios){
    for(int i=0;i<cantAnios;++i){

        //Si el año no está dentro del array de años solo se agregan los datos
        if(anios[i]==anio.anio && anioDentro[i]==0){
            aniosFinal[i] = anio;
            anioDentro[i] = 1;
            return aniosFinal;
        }
        //Si el año ya está dentro del array, se mezclan los datos para que concuerden con lo solicitado
        if(anios[i]==anio.anio && anioDentro[i]==1){
            aniosFinal[i].cantidadJuegos += anio.cantidadJuegos;
            aniosFinal[i].sumaPrecios += anio.sumaPrecios;
            aniosFinal[i].cantidadWindows += anio.cantidadWindows;
            aniosFinal[i].cantidadMac += anio.cantidadMac;
            aniosFinal[i].cantidadLinux += anio.cantidadLinux;

            //Los juegos gratis se van sumando
            if(aniosFinal[i].cantidadJuegosGratis == 0 && anio.cantidadJuegosGratis > 0){
                strcpy(aniosFinal[i].juegosGratis, anio.juegosGratis);
                aniosFinal[i].cantidadJuegosGratis += anio.cantidadJuegosGratis;
            }else if(aniosFinal[i].cantidadJuegosGratis > 0 && anio.cantidadJuegosGratis > 0){
                strcat(aniosFinal[i].juegosGratis, anio.juegosGratis);
                aniosFinal[i].cantidadJuegosGratis += anio.cantidadJuegosGratis;
            }

            //Se verifica el juego más caro del año
            if(anio.caro > aniosFinal[i].caro){
                aniosFinal[i].caro = anio.caro;
                strcpy(aniosFinal[i].nombreCaro, anio.nombreCaro);
            }

            //Se verifica el juego más barato del año
            if((anio.barato < aniosFinal[i].barato && anio.barato != -1) || aniosFinal[i].barato == -1){
                aniosFinal[i].barato = anio.barato;
                strcpy(aniosFinal[i].nombreBarato, anio.nombreBarato);
            }
            return aniosFinal;
        }
    }
    return aniosFinal;
}

void *funcionHilo(void *arg) {
    pthread_mutex_lock(&lock);
    int *chunk = (int *)arg;
    int i = 0;
    char linea[500];
    juego juego;
    while(fgets(linea, 500, archivoEntrada) != NULL && i < *chunk){
        printf("%s", linea);
        juego = lineaToJuego(linea);
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