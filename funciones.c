#include "funciones.h"
extern int n;
extern pthread_mutex_t lock;
extern FILE *archivoEntrada;
extern int contador;
extern anio *aniosStruct;
//extern anio aniosStruct[25];
extern int cantAnios;
extern int contadorAnios;
//extern char linea[500];

void funPrint(){
    printf("n: %d\n", n);
    n=n+1;
}


anio *agregarAniosFinales( anio anioAgregado){
    anio* aux = (anio*)malloc(sizeof(anio)*(cantAnios+1));
    for(int i = 0; i <cantAnios; i++){
        aux[i] = aniosStruct[i];
    }
    aux[cantAnios] = anioAgregado;
    cantAnios++;
    //Si se cae puede ser acá
    free(aniosStruct);
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
    //printf("%s", linea);
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


int juegoInAnios(juego juego){
    for(int i = 0; i < cantAnios; i++){
        if(juego.fecha == aniosStruct[i].anio){
            return i;
        }
    }
    return -1;
}

anio crearAnio(juego juego){
    anio nuevoAnio;
    nuevoAnio.anio = juego.fecha;
    nuevoAnio.cantidadJuegos = 1;
    strcpy(nuevoAnio.nombreCaro,juego.nombre);
    nuevoAnio.caro = juego.precio;
    strcpy(nuevoAnio.nombreBarato,juego.nombre);
    nuevoAnio.barato = juego.precio;
    nuevoAnio.sumaPrecios = juego.precio;
    nuevoAnio.cantidadWindows = juego.win;
    nuevoAnio.cantidadMac = juego.mac;
    nuevoAnio.cantidadLinux = juego.lin;
    if(juego.gratis == 1){
        nuevoAnio.cantidadJuegosGratis=1;
        strcpy(nuevoAnio.juegosGratis, juego.nombre);
        strcat(nuevoAnio.juegosGratis, "\n");
        return nuevoAnio;
    }
    strcpy(nuevoAnio.juegosGratis, "");
    nuevoAnio.cantidadJuegosGratis = 0;
    return nuevoAnio;
}

//Descripción: Función que junta lun año calculado por un worker y los guarda en un array con los datos combinados
//Entrada: arreglo de años, año, lista de posibles años, lista de si el año ya está dentro del array de años, cantidad de años
//Salida: arreglo de años con el año agregado
void juntarAnios(juego juego){
    int k = juegoInAnios(juego);
    //printf("k: %d", k);
    if(k == -1){
        anio anioAgregar = crearAnio(juego);
        aniosStruct = agregarAniosFinales(anioAgregar);
        //aniosStruct[cantAnios] = anioAgregar;
        //cantAnios++;
    }else{
        aniosStruct[k].cantidadJuegos++;
        aniosStruct[k].sumaPrecios += juego.precio;
        aniosStruct[k].cantidadWindows += juego.win;
        aniosStruct[k].cantidadMac += juego.mac;
        aniosStruct[k].cantidadLinux += juego.lin;
        if(juego.precio > aniosStruct[k].caro){
            aniosStruct[k].caro = juego.precio;
            strcpy(aniosStruct[k].nombreCaro, juego.nombre);
        }
        if(juego.precio < aniosStruct[k].barato){
            aniosStruct[k].barato = juego.precio;
            strcpy(aniosStruct[k].nombreBarato, juego.nombre);
        }
        if(juego.gratis == 1){
            aniosStruct[k].cantidadJuegosGratis++;
            strcat(aniosStruct[k].juegosGratis, juego.nombre);
            strcat(aniosStruct[k].juegosGratis, "\n");
        }
    }
    /* //Si el año ya está dentro del array, se mezclan los datos para que concuerden con lo solicitado
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
return aniosFinal;*/
}

//Posible error en la función por printf de n
void *funcionHilo(void *arg) {
    pthread_mutex_lock(&lock);
    int *chunk = (int *)arg;
    contador++;
    int i = 0;
    char linea[500];
    juego juego;
    while(i<*chunk && fgets(linea, 500, archivoEntrada) != NULL){
        /* if(i == *chunk){
            break;
        } */
        //printf("%s", linea);
        contadorAnios++;
        juego = lineaToJuego(linea);
        juntarAnios(juego);
        /* printf("%d %s %d %f %d %d %d %d %d %d\n", juego.id, juego.nombre,
        juego.restriccion, juego.precio, juego.proximamente, juego.fecha,
        juego.gratis, juego.win, juego.mac, juego.lin);  */
        i++; 
    }
    //printf("i: %d, contador: %d\n", i,contador);
    //funPrint();
    pthread_mutex_unlock(&lock);    
    return NULL;
}

int buscarIndice(int anioInicio){
    if(anioInicio <= aniosStruct[0].anio){return 0;}
    for(int i=0;i<cantAnios;i++){
        if(anioInicio == aniosStruct[i].anio){
            return i;
        }
    }
    return -1;
}

void escribirArchivo(char * nombreSalida, int anioInicio, int bandera){
    char lineasAEscribir[100000]="";
    char temporal[5000];
    FILE *archivoSalida = fopen(nombreSalida, "w");
    if(archivoSalida == NULL){
        printf("Error: No se pudo abrir el archivo de salida.\n");
        return;
    }
    int indice = buscarIndice(anioInicio);
    if(indice == -1){
        printf("No se encontro el anio %d en el archivo", anioInicio);
        return;
    }
    for(int i=indice;i<cantAnios;i++){
        sprintf(temporal, "Año: %d\nJuego más caro: %s\nJuego más barato: %s\nPromedio de precios: %f\nWindows: %d Mac: %d Linux: %d\nJuegos gratis:\n %s\n\n",
        aniosStruct[i].anio, aniosStruct[i].nombreCaro, aniosStruct[i].nombreBarato, aniosStruct[i].sumaPrecios/aniosStruct[i].cantidadJuegos,
        (aniosStruct[i].cantidadWindows*100)/aniosStruct[i].cantidadJuegos, (aniosStruct[i].cantidadMac*100)/aniosStruct[i].cantidadJuegos, 
        (aniosStruct[i].cantidadLinux*100)/aniosStruct[i].cantidadJuegos, aniosStruct[i].juegosGratis);
        strcat(lineasAEscribir, temporal);
    }
    if(bandera == 1){
        printf("%s", lineasAEscribir);
    }

    fprintf(archivoSalida,"%s",lineasAEscribir);
    fclose(archivoSalida);
}

//Esta funcion es el mergesort para ordenar por anios
//Entrada: arreglo de enteros, arreglo de enteros, entero, entero
//Salida: void, se ordena por dentro
void merge(anio* aniosFinal,int inicio,int medio,int fin){
	// Se inicia en cero una lista de struct de datos temporal
	// para dejar los datos.
	anio* temp = (anio*)calloc(fin-inicio+1,sizeof(anio));
	// Se establecen el inicio del arreglo izquierdo y el derecho.
	int izq = inicio;
	int der = medio+1;
	int k = 0;

	// Se sigue copiando hasta que se llegue al final de 
	// alguna de las 2 listas.
	while(izq<=medio && der<=fin){
		if(aniosFinal[izq].anio<=aniosFinal[der].anio){
			// Si la cantidad que se reemplazadoite del elemento izquierdo
			// es el menor se copia en la lista temporal.
			temp[k] = aniosFinal[izq];
			izq++;
		}else{
			// Si no, se copia el elemento derecho en la lista temporal. 
			temp[k] = aniosFinal[der];
			der++;
		}
		k++;
	} 
	// Cuando uno de los 2 se pase del final, se tiene 
	// que copiar el arreglo que sobre.

	// Se supone que en el arreglo izquierdo quedaron datos.
	for(;izq<=medio;izq++){
		temp[k] = aniosFinal[izq];
		k++;
	}

	// Se supone que en el arreglo derecho quedaron datos.
	for(;der<=fin;der++){
		temp[k] = aniosFinal[der];
		k++;
	}

	// Se copia la lista temporal en el arreglo de datos.
	int l=0;
	for(k=inicio;k<=fin;k++){
		aniosFinal[k] = temp[l];
		l++;
	}
	// Se libera el espacio de la lista temporal.
	free(temp);
	return;
}


//Esta funcion es el mergesort para ordenar por anios
//Entrada: arreglo de enteros, entero, entero
//Salida: void, se ordena por dentro
void mergeSort(anio* aniosFinal, int ini, int fin){
	// Caso base, se tiene solo 1 elemento.
	if(ini==fin){
		return;
	} else{ // Se tiene mas de 1 elemento.
		// Se calcula el medio del arreglo.
		int med = (ini+fin)/2;
		// Se ordena el arreglo, pero desde el inicio hasta el medio.
		mergeSort(aniosFinal,ini,med);
		// Se ordena el arreglo, pero desde el medio+1 hasta el fin.
		mergeSort(aniosFinal,med+1,fin);
		// Se unen la lista ordenadamente.
		merge(aniosFinal,ini,med,fin);
		return;
	}
}