//PROGRAMA EJECUTABLE, AFINAR FUNCIONES


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Map.h"


typedef struct Cancion{
    char * titulo;
    char * artista;
    char * duracion;
    char * album;
    char * fecha;
}Cancion;


//Prototipo de Funciones
Cancion *crearCancion(char * linea); //CREAR TIPO CANCION PERO CUANDO SE ACCEDE AL ARCHIVO
Cancion *crearCancionManual(char *nombre_cancion); //CREAR CANCION INGRESANDO EL NOMBRE
const char *get_csv_field (char * tmp, int i);
void cargar_datos_csv(FILE * fp, Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album);
void eliminar_canciones_artista(char * nombre_artista,Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album);
Cancion * buscar_cancion(char * nombre_cancion, Map * mapa_canciones);
Map * buscar_canciones_artista(char * nombre_artista, Map * mapa_artistas);
void mostrar_menu_1();
int stringEqual(const void * key1, const void * key2);
long long stringHash(const void * key);
void agregar_album(char* nombre, char* fecha_lanzamiento,Map *Mapa_album,Map* mapa_canciones);
//Funciones del menu
void importar_archivo(Map* mapa_canciones,Map* mapa_artistas,Map* Mapa_album);
void exportar_archivo(Map *mapa_canciones);
void funcion_agregar_album(Map* Mapa_album,Map *mapa_canciones);
void funcion_agregar_cancion(Map* mapa_canciones,Map *Mapa_album,Map* mapa_artistas);
void funcion_eliminar_canciones_artista(Map* mapa_artistas,Map* Mapa_album, Map* mapa_canciones);
void funcion_buscar(Map* mapa_canciones);
void funcion_buscar_canciones_artista(Map* mapa_artistas,Map* mapa_canciones);
void funcion_buscar_album(Map* Mapa_album);
long long stringHash(const void * key);
int stringEqual(const void * key1, const void * key2);





int main(){
    int indice = 0; //Selector del menu
    char temp[10]; //Temporal
    //Creamos los Mapas principales
    Map *mapa_canciones = createMap(stringHash,stringEqual);
    Map *mapa_artistas = createMap(stringHash,stringEqual);
    Map *Mapa_album = createMap(stringHash,stringEqual);
    do{
        mostrar_menu_1();
        printf("INGRESE LA OPCION A USAR PORFAVOR\n");
        fgets(temp,10,stdin);
        system("cls");
        indice = atoi(temp);
        switch(indice){
            //OPCION 1 IMPORTAR MUSICA DESDE ARCHIVO //LISTO Y PROBADO
            case 1:
                importar_archivo(mapa_canciones,mapa_artistas,Mapa_album);
                system("cls");
                printf("ARCHIVO IMPORTADO CON EXITO\n");
            break;
            //OPCION 2 EXPORTAR A ARCHIVO CSV //LISTO y PROBADO
            case 2:
                exportar_archivo(mapa_canciones);
                system("cls");
                printf("ARCHIVO EXPORTADO CON EXITO\n");
            break;
            //OPCION 3 AGREGAR ALBUM MANUALMENTE //EN PROCESO
            case 3:
                funcion_agregar_album(Mapa_album,mapa_canciones);
                system("cls");
            break;
            //OPCION 4 AGREGAR CANCION //EN PROCESO
            case 4:
                funcion_agregar_cancion(mapa_canciones,Mapa_album,mapa_artistas);
                system("cls");
            break;
            //OPCION 5 ELIMINAR CANCIONES DE UN ARTISTA
            case 5:
                funcion_eliminar_canciones_artista(mapa_artistas,Mapa_album,mapa_canciones);
                system("cls");
            break;
            //OPCION 6 BUSCAR CANCION E IMPRIMIR TODAS LAS CANCIONES Y DATA ASCIADA
            case 6:
                funcion_buscar(mapa_canciones);
            break;
            //OPCION 7 BUSCAR CANCIONES DE UN ARTISTA E IMPRIMIR INFORMACION
            case 7:
                funcion_buscar_canciones_artista(mapa_artistas,mapa_canciones);
            break;
            //OPCION 8 BUSCAR ALBUM E IMPRIMIR CANCIONES
            case 8:
                funcion_buscar_album(Mapa_album);
            break;
            //OPCION 9 SALIR
            case 9:
                exit(0);
            break;
            default:
                printf("OPCION INVALIDA PORFAVOR INGRESE DENUEVO\n");
                system("cls");
            break;
        }
    }while(indice != 9);

}



//Funciones

const char *get_csv_field (char * tmp, int i) {
    //se crea una copia del string tmp
    char * line = _strdup (tmp);
    const char * tok;
    for (tok = strtok (line, ","); tok && *tok; tok = strtok (NULL, ",\n")) {
        if (!--i) {
            return tok;
        }
    }
    return NULL;
}

Cancion *crearCancion(char *linea){
    Cancion *cancion = malloc(sizeof(Cancion));
    const char *titulo = get_csv_field(linea,1);
    const char *artista = get_csv_field(linea,2);
    const char *duracion = get_csv_field(linea,3);
    const char *album = get_csv_field(linea,4);
    cancion->titulo = (char*)titulo;
    cancion->artista = (char*)artista;
    cancion->duracion = (char*)duracion;
    cancion->album = (char*)album;
    cancion->fecha = NULL;
    return cancion;
}

void cargar_datos_csv(FILE * fp, Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album){
    char * linea  = (char* )malloc(1024*sizeof(char));
    while (fgets (linea, 1023, fp) != NULL) {
        linea = strtok(linea,"\r");
        linea = _strdup(linea); //INGESAN EN MAYUSCULA A LA BASE DE DATOS
        Cancion* C = crearCancion(linea);
        Cancion * aux = searchMap(mapa_canciones, C->titulo );
        //Mapa Canciones
        if ( aux == NULL){
            insertMap(mapa_canciones, C->titulo, C);
        }
        //Mapa Artistas
        Map* aux1 = searchMap(mapa_artistas, C->artista );
        if ( aux1 == NULL){
            Map * mapa_artistas_aux = createMap(stringHash, stringEqual);
            insertMap(mapa_artistas_aux, C->titulo, C);
            insertMap(mapa_artistas, C->artista, mapa_artistas_aux);
        }
        else{
            insertMap(aux1,C->titulo,C);
        }
        //Mapa Albums
        Map* aux2 = searchMap(Mapa_album, C->album );
        if ( aux2 == NULL){
            Map * mapa_album_aux = createMap(stringHash, stringEqual);
            insertMap(mapa_album_aux, C->titulo, C);
            insertMap(Mapa_album, C->album, mapa_album_aux);
        }
        else{
            insertMap(aux2,C->album,C);
        }
        linea  = malloc(1024*sizeof(char)*2);
    }

}


void eliminar_canciones_artista(char * nombre_artista,Map * mapa_canciones , Map * mapa_artistas, Map * Mapa_album){
    //Obtenemos el mapa de canciones del artista
    Map *aux_canciones = searchMap(mapa_artistas,nombre_artista);
    //Accedemos a la primera cancion
    Cancion* aux_tipo_cancion = firstMap(aux_canciones);
    while(aux_tipo_cancion != NULL){
        //Eliminamos del mapa canciones
        eraseKeyMap(mapa_canciones, aux_tipo_cancion->titulo);
        //Eliminamos del mapa album
        eraseKeyMap(Mapa_album,aux_tipo_cancion->album);
        //Avanza en la siguiente cancion
        aux_tipo_cancion = nextMap(aux_canciones);
    }
    //Eliminamos del mapa artista
    eraseKeyMap(mapa_artistas, nombre_artista);

}


Cancion * buscar_cancion(char * nombre_cancion, Map * mapa_canciones){
    if(searchMap(mapa_canciones, nombre_cancion)){
        return searchMap(mapa_canciones, nombre_cancion);
    }
    else{
        return NULL;
    }
}

Map * buscar_canciones_artista(char * nombre_artista, Map * mapa_artistas){
    if((searchMap(mapa_artistas,nombre_artista))){
        return searchMap(mapa_artistas,nombre_artista);
    }
    else{
        return NULL;
    }
}

void agregar_album(char* nombre, char* fecha_lanzamiento,Map *Mapa_album,Map* mapa_canciones){
    //Se ve si el album se encuentra en el mapa albums
    char* respuesta[10];
    char* cancion_aux[100];
    Map* auxiliar_mapa_album = searchMap(Mapa_album,nombre); //Se busca si el ya esta
    //Si el album esta
    if(auxiliar_mapa_album != NULL){
        printf("ALBUM YA INGRESADO\n");
        //Ingresar mas canciones si se quiere
        do{
            printf("DESEA INGRESAR MAS CANCIONES AL ALBUM? Y/N\n");
            fgets((char*)respuesta,2,stdin);
            if(strcmp((char*)respuesta,"Y")){
                printf("INGRESE EL NOMBRE DE LA CANCION\n");
                fgets((char*)cancion_aux,100,stdin);
                //Creamos la cancion
                Cancion *aux = crearCancionManual((char*)cancion_aux);
                aux->album = strcpy(aux->album,nombre);
                //La ingresamos al mapa del album previamente existente y entregado con la funcion search
                insertMap(auxiliar_mapa_album,nombre,aux);
            }
        }while(!strcmp((char*)respuesta,"N"));
    }
    //Si no esta crear el album
    else{
        Map *album_auxiliar = createMap(stringHash,stringEqual);
        //Preguntamos por el nombre de la primera cancion
        do{
            printf("INGRESE CANCION PARA EL ALBUM\n");
            fgets((char*)cancion_aux,100,stdin);
            //Creamos la cancion con su nombre y asignamos su album
            Cancion *aux = crearCancionManual((char*)cancion_aux);
            aux->album = nombre;
            //Buscamos si la cancion ya existia
            if(searchMap(mapa_canciones,cancion_aux) == NULL){
                //Ingresamos el tema al album auxiliar
                insertMap(album_auxiliar,cancion_aux,aux);
                //Agregamos la cancion en el mapa canciones
                insertMap(mapa_canciones,cancion_aux,aux);
                //Preguntamos si queremos seguir agregando canciones
                printf("QUIERES AGREGAR OTRA CANCION AL ALBUM? Y/N\n");
                fgets((char*)respuesta,2,stdin);
                if(strcmp((char*)respuesta,"N")){
                    insertMap(Mapa_album,nombre,album_auxiliar);
                    break;
                }
            }
        }while(!strcmp((char*)respuesta,"N"));

    }
}



Cancion *crearCancionManual(char *nombre_cancion){
    //Ingresamos el nombre y dejamos el resto de sus datos en nulo
    Cancion *cancion = (Cancion*) malloc(sizeof(cancion));
    cancion->album = NULL;
    cancion->artista = NULL;
    cancion->duracion = NULL;
    cancion->fecha = NULL;
    cancion->titulo = strcpy(cancion->titulo,nombre_cancion);
    return cancion;
}

long long stringHash(const void * key) {
    long long hash = 5381;

    const char * ptr;

    for (ptr = key; *ptr != '\0'; ptr++) {
        hash = ((hash << 5) + hash) + tolower(*ptr); /* hash * 33 + c */
    }

    return hash;
}

int stringEqual(const void * key1, const void * key2) {
    const char * A = key1;
    const char * B = key2;

    return strcmp(A, B) == 0;
}

//FUNCIONES DEL MENU
void mostrar_menu_1(){
    printf("|----------------------------------------------------------------------|\n");
    printf("|----------------BIENVVENIDOS AL ORGANIZADOR DE MUSICA-----------------|\n");
    printf("|--------------------CHARMICHARMICAT 2000------------------------------|\n");
    printf("|----------------------------------------------------------------------|\n");
    printf("| 1) IMPORTAR MUSICA DESDE ARCHIVO                                     |\n");
    printf("| 2) EXPORTAR MUSICA ARCHIVO CSV                                       |\n");
    printf("| 3) AGREGAR ALBUM                                                     |\n");
    printf("| 4) AGREGAR CANCION                                                   |\n");
    printf("| 5) ELIMINAR CANCIONES DE UN ARTISTA                                  |\n");
    printf("| 6) BUSCAR CANCION                                                    |\n");
    printf("| 7) BUSCAR CANCIONES DE UN ARTISTA                                    |\n");
    printf("| 8) BUSCAR ALBUM                                                      |\n");
    printf("| 9) SALIR DEL PROGRAMA                                                |\n");
    printf("|----------------------------------------------------------------------|\n");
}

void importar_archivo(Map* mapa_canciones,Map* mapa_artistas,Map* Mapa_album){ //LISTO NO TOCAR
    //Abrimos el archivo
    FILE *fp = fopen("canciones.csv","r");
    //Usamos la funcion cargar datos
    cargar_datos_csv(fp,mapa_canciones,mapa_artistas,Mapa_album);
    //Cerramos el archivo
    fclose(fp);

}

void exportar_archivo(Map *mapa_canciones){  //LISTO NO TOCAR
    //Creamos el archivo para guardar la data
    FILE *wp = fopen("musica.csv","w");
    //Recorremos la fuente de las canciones
    Cancion* cancion_aux = firstMap(mapa_canciones);
    while(cancion_aux != NULL){
        //ESCRIBIMOS EL ARCHIVO NUEVO LINEA A LINEA
        fprintf(wp,"%s, %s, %s, %s \n",cancion_aux->titulo,cancion_aux->artista,cancion_aux->duracion,cancion_aux->album);
        cancion_aux = nextMap(mapa_canciones);
    }
    fclose(wp);
}

void funcion_agregar_album(Map* Mapa_album,Map *mapa_canciones){
    char* nombre_album[51];
    char* fecha_lanzamiento[20];
    printf("INGRESE EL NOMBRE DEL ALBUM A INGRESAR\n");
    fgets((char*)nombre_album,50,stdin);
    printf("INGRESE EL ANNO DE LANZAMIENTO\n");
    fgets((char*)fecha_lanzamiento,20,stdin);
    agregar_album((char*)nombre_album,(char*)fecha_lanzamiento,Mapa_album,mapa_canciones);
}

void funcion_agregar_cancion(Map* mapa_canciones,Map *Mapa_album,Map* mapa_artistas){
    char *nombre_cancion[100];
    printf("INGRESE EL NOMBRE DE LA CANCION\n");
    fgets((char*)nombre_cancion,100,stdin);
    //Buscamos si la cancion esta en el mapa canciones
    if(searchMap(mapa_canciones,nombre_cancion) == NULL){
        char *artista[100];
        char *album[100];
        char *duracion[100];
        //Si no esta se ingresan los datos y se crea la cancion
        Cancion *cancion_aux = (Cancion*) malloc(sizeof(Cancion));
        cancion_aux->titulo = strcpy(cancion_aux->titulo,(char*)nombre_cancion);
        printf("INGRESE EL ARTISTA DE LA CANCION\n");
        fgets((char*)artista,100,stdin);
        cancion_aux->artista = strcpy(cancion_aux->artista,(char*)artista);
        printf("INGRESE EL ALBUM DE LA CANCION\n");
        fgets((char*)album,100,stdin);
        cancion_aux->album = strcpy(cancion_aux->album,(char*)album);
        getchar();
        printf("INGRESE LA DURACION DEL ALBUM\n");
        fgets((char*)duracion,100,stdin);
        cancion_aux->duracion = strcpy(cancion_aux->duracion,(char*)duracion);
        //Se inserta la cancion en el mapa canciones
        insertMap(mapa_canciones,cancion_aux->titulo,cancion_aux);
        //Se inserta la cancion en el mapa albums y artistas
        insertMap(searchMap(mapa_artistas,artista),cancion_aux->artista,cancion_aux);
        insertMap(Mapa_album,cancion_aux->album,cancion_aux);
        //Imprimir mensaje
        printf("CANCION INGRESADA SATISFACTORIAMENTE \n");
    }
    else{
        printf("CANCION YA INGRESADA\n");
    }
}


void funcion_eliminar_canciones_artista(Map* mapa_artistas,Map* Mapa_album, Map* mapa_canciones){
    char* artista_aux[100];
    printf("INGRESE EL NOMBRE DEL ARTISTA\n");
    fgets((char*)artista_aux,100,stdin);
    //BUSCAMOS SI EL ARTISTA ESTA EN EL MAPA ARTISTAS
    if(searchMap(mapa_artistas,artista_aux)){
        //Usamos la funcion eliminar
        eliminar_canciones_artista((char*)artista_aux,mapa_canciones ,mapa_artistas,Mapa_album);
        printf("ARTISTA Y CANCIONES ELIMINADAS SATISFACTORIAMENTE\n");
    }
    else{
        printf("ARTISTA NO ENCONTRADO \n");

    }
}

void funcion_buscar(Map* mapa_canciones){
    char* nombre_cancion_buscar[100];
    printf("INGRESE CANCION A BUSCAR\n");
    fgets((char*)nombre_cancion_buscar,100,stdin);
    Cancion *auxiliar_imprimir = buscar_cancion((char*)nombre_cancion_buscar,mapa_canciones);
    do{
        if(auxiliar_imprimir != NULL){
            //Imprimimos la data de los temas
            printf("|--DATOS DE LA CANCION                            |\n");
            printf("|--CANCION: %s                                    |\n",auxiliar_imprimir->titulo);
            printf("|--ARTISTA: %s                                    |\n",auxiliar_imprimir->artista);
            printf("|--ALBUM: %s                                      |\n",auxiliar_imprimir->album);
            printf("|--DURACION: %s                                   |\n",auxiliar_imprimir->duracion);
            printf("|                                                 |\n");
        }
        else{
            printf("CANCION NO ENCONTRADA EN LA BASE DE DATOS\n");
            break;
        }
            auxiliar_imprimir = buscar_cancion((char*)nombre_cancion_buscar,mapa_canciones);
    }while(auxiliar_imprimir!= NULL);
    printf("CANCIONES MOSTRADAS SATISFACTORIAMENTE\n");

}

void funcion_buscar_canciones_artista(Map* mapa_artistas,Map* mapa_canciones){
    //INGRESAMOS EL NOMBRE DEL ARTISTA
    char* artista_aux2 = "";
    printf("INGRESE EL NOMBRE DEL ARTISTA \n");
    scanf("%s",artista_aux2);
    //Usamos la funcion
    Map* map_aux = buscar_canciones_artista(artista_aux2,mapa_artistas);
    Cancion *auxiliar= firstMap(map_aux);
    if(auxiliar != NULL){
        //Imprimimos por pantalla
        while(auxiliar != NULL){
            printf("|--CANCION: %s                                    \n",auxiliar->titulo);
            printf("|--ARTISTA: %s                                    \n",auxiliar->artista);
            printf("|--ALBUM: %s                                      \n",auxiliar->album);
            printf("|--DURACION: %s                                   \n",auxiliar->duracion);
            printf("|                                                 \n");
                auxiliar = nextMap(map_aux);
        }
    }
    else{
        printf("ARTISTA NO ENCONTRADO EN LA BASE DE DATOS \n");
    }
}


void funcion_buscar_album(Map* Mapa_album){
    //INGRESAMOS
    printf("INGRESE EL ALBUM QUE DESEA BUSCAR\n");
    char* album_aux = "";
    scanf("%s",album_aux);
    Map* map_aux = searchMap(Mapa_album,album_aux);
    Cancion* auxiliar = firstMap(map_aux);
    if(auxiliar != NULL){
        //Imrpimimos
        printf("LISTA DE CANCIONES DEL ALBUM : \n");
        printf("                                   \n");
        while(auxiliar != NULL){
            printf("%s \n",auxiliar->titulo);
            printf("                                   \n");
            auxiliar = nextMap(map_aux);
        }
    }
}

