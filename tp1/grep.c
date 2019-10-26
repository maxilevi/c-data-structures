#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void guardar_linea(char** buffer, char* linea, int buffer_length) {
    for(int i = buffer_length-1; i > 0; --i) {
        if (i == buffer_length-1 && buffer[i]) free(buffer[i]);
        buffer[i] = buffer[i-1];
    }
    buffer[0] = strdup(linea);
}

void nulear_buffer(char** buffer, int length, bool primera_vez) {
    for(int i = 0; i < length; ++i) {
        /* Si es la primera vez que lo limpiamos podemos llegar a estar free'd a cosas no inicializadas que no sean NULL */
        if(!primera_vez && buffer[i]) free(buffer[i]);
        buffer[i] = NULL;
    }
}

void limpiar_buffer(char** buffer, int length) {
    nulear_buffer(buffer, length, false);
}

void inicializar_buffer(char** buffer, int length) {
    nulear_buffer(buffer, length, true);
}

void mostrar_encontrada(char** buffer, int length) {
    for(int i = length-1; i > -1; --i) {
        if(buffer[i])
            printf("%s", buffer[i]);
    }
}

int mostrar_error(char* msg) {
    fprintf(stderr, "%s\n", msg);
    return -1;
}

void destruir_buffer(char** buffer, int length) {
    for(int i = 0; i < length; ++i) {
        if(buffer[i]) free(buffer[i]);
    }
    free(buffer);
}

void procesar(FILE* stream, char* aguja, int contexto) {
    int buffer_length = contexto + 1;
    char** buffer = malloc(sizeof(char*) * (buffer_length));
    if(buffer == NULL) {
        mostrar_error("No se pudo conseguir la memoria necesaria");
        return;
    }
    inicializar_buffer(buffer, buffer_length);
    char* linea = NULL;
    size_t n = 0;
    while (getline(&linea, &n, stream) != -1) {
        guardar_linea(buffer, linea, buffer_length);
        if (strstr(linea, aguja)) {
            mostrar_encontrada(buffer, buffer_length);
            limpiar_buffer(buffer, buffer_length);
        }
    }
    destruir_buffer(buffer, buffer_length);
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
