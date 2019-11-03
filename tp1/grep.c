#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cola.h"

void mostrar_encontrada(cola_t* buffer, int* cantidad) {
    while(!cola_esta_vacia(buffer)) {
        char* str = cola_desencolar(buffer);
        printf("%s", str);
        /* El string se creo con strdup */
        free(str);
    }
    *cantidad = 0;
}

int mostrar_error(char* msg) {
    fprintf(stderr, "%s\n", msg);
    return -1;
}

bool guardar_linea(cola_t* buffer, char* linea, int* cantidad, int max) {
    if(max == *cantidad) {
        void* str = cola_desencolar(buffer);
        free(str);
        *cantidad -= 1;
    }
    *cantidad += 1;
    return cola_encolar(buffer, strdup(linea));
}

void procesar(FILE* stream, char* aguja, int contexto) {
    int cantidad = 0;
    cola_t* buffer = cola_crear();
    if (buffer == NULL) {
        mostrar_error("No se pudo conseguir la memoria necesaria");
        return;
    }
    char* linea = NULL;
    size_t n = 0;
    while (getline(&linea, &n, stream) != -1) {
        if(!guardar_linea(buffer, linea, &cantidad, contexto + 1)) {
            mostrar_error("No se pudo conseguir la memoria necesaria");
            break;
        }
        if (strstr(linea, aguja)) {
            mostrar_encontrada(buffer, &cantidad);
        }
    }
    cola_destruir(buffer, free);
    free(linea);
}

bool assert_cantidad_argumentos(int argc) {
    return argc == 3 || argc == 4;
}

bool es_numero(char* str) {
    for(int i = 0; str[i] != '\0'; ++i) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool assert_contexto(char* contexto, int* parsed) {
    if(!es_numero(contexto)) return false;
    *parsed = atoi(contexto);
    return true;
}

bool assert_file(char* filename, FILE** stream) {
    FILE* new = fopen(filename, "r");
    if(new == NULL) return false;
    *stream = new;
    return true;
}

void cerrar_archivo(FILE* stream) {
    fclose(stream);
}

int main(int argc, char** argv) {
    int contexto;
    FILE* stream = stdin;
    bool hay_archivo = argc == 4;
    if(!assert_cantidad_argumentos(argc)) return mostrar_error("Cantidad de parametros erronea");
    if(!assert_contexto(argv[2], &contexto)) return mostrar_error("Tipo de parametro incorrecto");
    if(hay_archivo && !assert_file(argv[3], &stream)) return mostrar_error("No se pudo leer el archivo indicado");

    procesar(stream, argv[1], contexto);

    if(hay_archivo)
        cerrar_archivo(stream);
    return 0;
}
