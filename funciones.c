#include "funciones.h"

//Variables globales
extern int n;
extern pthread_mutex_t lock;
extern FILE *archivoEntrada;
extern anio *aniosStruct;
extern int cantAnios;
extern int chunk;
extern float precioMinimo;

// Función que agrega un año a un arreglo de años
// Entrada: año
// Salida: nada, pues se uiiliza un arreglo global
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

// Función que transforma una línea de un archivo a un struct juego
// Entrada: string
// Salida: struct juego
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

//Función que revisa si un año está en un arreglo de años
//Entrada: arreglo de años, año a buscar, tamaño del arreglo
//Salida: posición en donde está el año, -1 si no está
int juegoInAnios(juego juego){
    for(int i = 0; i < cantAnios; i++){
        if(juego.fecha == aniosStruct[i].anio){
            return i;
        }
    }
    return -1;
}

//Función que crea un año vacío a partir de un juego
//Entrada: juego
//Salida: año
anio crearAnioVacio(juego juego){
    anio nuevoAnio;
    nuevoAnio.anio = juego.fecha;
    nuevoAnio.cantidadJuegos = 0;
    strcpy(nuevoAnio.nombreCaro,"No tiene");
    nuevoAnio.caro = 0;
    strcpy(nuevoAnio.nombreBarato,"No tiene");
    nuevoAnio.barato = -1;
    nuevoAnio.sumaPrecios = 0;
    nuevoAnio.cantidadWindows = 0;
    nuevoAnio.cantidadMac = 0;
    nuevoAnio.cantidadLinux = 0;
    //Si el juego es gratis
    if(juego.gratis == 1){
        nuevoAnio.cantidadJuegosGratis=1;
        strcpy(nuevoAnio.juegosGratis, juego.nombre);
        strcat(nuevoAnio.juegosGratis, "\n");
        return nuevoAnio;
    }
    //Si el juego no es gratis
    strcpy(nuevoAnio.juegosGratis, "");
    nuevoAnio.cantidadJuegosGratis = 0;
    return nuevoAnio;
}

//Función que crea un año a partir de un juego
//Entrada: juego
//Salida: año
anio crearAnio(juego juego){
    anio nuevoAnio;
    //se crea el año
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

    //Si el juego es gratis
    if(juego.gratis == 1){
        nuevoAnio.cantidadJuegosGratis=1;
        strcpy(nuevoAnio.juegosGratis, juego.nombre);
        strcat(nuevoAnio.juegosGratis, "\n");
        return nuevoAnio;
    }
    //Si el juego no es gratis
    strcpy(nuevoAnio.juegosGratis, "");
    nuevoAnio.cantidadJuegosGratis = 0;
    return nuevoAnio;
}

//Descripción: Función que junta los años a partir de un juego y el arreglo de años global
//Entrada: juego a combinar con los años actuales
//Salida: nada pues se trabaja sobre arreglo de años con el año agregado
void juntarAnios(juego juego){
    int k = juegoInAnios(juego);
    //Si el año del juego no está en el arreglo de años
    if(k == -1){
        anio anioAgregar;
        // Si el precio del juego no supera al precio minimo
        if(precioMinimo > juego.precio){
            // Se crea el año sin la informacion del juego
            anioAgregar = crearAnioVacio(juego);
        }else{
            // Sino, se crea el año con la informacion del juego.
            anioAgregar = crearAnio(juego);
        }
        // Se agrega el año creado a la lista de años
        aniosStruct = agregarAniosFinales(anioAgregar);

    }else{
        //Si el año del juego está en el arreglo de años
        if(juego.gratis == 1){
            aniosStruct[k].cantidadJuegosGratis++;
            strcat(aniosStruct[k].juegosGratis, juego.nombre);
            strcat(aniosStruct[k].juegosGratis, "\n");
        }
        if(precioMinimo > juego.precio){
            return;
        }
        aniosStruct[k].cantidadJuegos++;
        aniosStruct[k].sumaPrecios += juego.precio;
        aniosStruct[k].cantidadWindows += juego.win;
        aniosStruct[k].cantidadMac += juego.mac;
        aniosStruct[k].cantidadLinux += juego.lin;
        if(juego.precio > aniosStruct[k].caro){
            aniosStruct[k].caro = juego.precio;
            strcpy(aniosStruct[k].nombreCaro, juego.nombre);
        }
        if(juego.precio < aniosStruct[k].barato || aniosStruct[k].barato == -1){
            aniosStruct[k].barato = juego.precio;
            strcpy(aniosStruct[k].nombreBarato, juego.nombre);
        }
    }
}

//Función que es la función principal de la hebraa la que se llama, lee el archivo y crea los años
//Entrada: nada, se trabaja con variables globales
//Salida: nada, se trabaja con variables globales
void *funcionHilo(void *arg) {
    int i;
    juego juego;
    //Se lee el archivo
    while(1){
        i=0;
        char linea[500];
        //Se entra a la sección crítica
        pthread_mutex_lock(&lock);
        while(i<chunk && fgets(linea, 500, archivoEntrada)!=NULL){
            juego = lineaToJuego(linea);
            juntarAnios(juego);
            i++;
        }
        //Se sale de la sección crítica
        pthread_mutex_unlock(&lock);
        if (feof(archivoEntrada)!=0){   
            break;
        }
    }
    return NULL;
}


//Función que selecciona las lineas a partir del año de inicio
//Entrada: año de inicio
//Salida: indice de la linea a partir de la cual se debe empezar a escribir
int buscarIndice(int anioInicio){
    if(anioInicio <= aniosStruct[0].anio){return 0;}
    for(int i=0;i<cantAnios;i++){
        if(anioInicio == aniosStruct[i].anio){
            return i;
        }
    }
    return -1;
}


//Función que escribe el archivo de salida
//Entrada: nombre del archivo, año de inicio, bandera de si es que se imprime o no
//Salida: nada, se escribe dentro de un archivo
void escribirArchivo(char * nombreSalida, int anioInicio, int bandera){
    //se inicia la escritura del archivo
    char lineasAEscribir[100000]="";
    char temporal[5000];
    FILE *archivoSalida = fopen(nombreSalida, "w");
    if(archivoSalida == NULL){
        printf("Error: No se pudo abrir el archivo de salida.\n");
        return;
    }
    //Se busca el índice a partir de la bandera de año de inicio
    int indice = buscarIndice(anioInicio);
    if(indice == -1){
        printf("No se encontro el anio %d en el archivo", anioInicio);
        return;
    }
    //Se escriben los datos iniciales para cambiarse después con los datos del arreglo de años
    for(int i=indice;i<cantAnios;i++){
        if(aniosStruct[i].cantidadJuegos == 0){
            aniosStruct[i].cantidadJuegos = 1;
            aniosStruct[i].barato = 0;
        }
        if(aniosStruct[i].cantidadJuegosGratis == 0){
            strcpy(aniosStruct[i].juegosGratis, "No tiene");
        }
        sprintf(temporal, "Año: %d\nJuego más caro: %s %f\nJuego más barato: %s %f\nPromedio de precios: %f\nWindows: %f Mac: %f Linux: %f\nJuegos gratis:\n%s\n\n",
        aniosStruct[i].anio, aniosStruct[i].nombreCaro, aniosStruct[i].caro, aniosStruct[i].nombreBarato, aniosStruct[i].barato, aniosStruct[i].sumaPrecios/aniosStruct[i].cantidadJuegos,
        ((float)aniosStruct[i].cantidadWindows/(float)aniosStruct[i].cantidadJuegos)*100, ((float)aniosStruct[i].cantidadMac/(float)aniosStruct[i].cantidadJuegos)*100, 
        ((float)aniosStruct[i].cantidadLinux/(float)aniosStruct[i].cantidadJuegos)*100, aniosStruct[i].juegosGratis);
        strcat(lineasAEscribir, temporal);
    }

    //si es que se imprime
    if(bandera == 1){
        printf("%s", lineasAEscribir);
    }

    // Se escribe en el archivo final
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