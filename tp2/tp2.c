#define _POSIX_C_SOURCE 200809L
#include "validador.h"
#include "ejecutor.h"
#include "strutil.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Auxiliares */

char* left_trim(char* s)
{
	while (isspace(*s)) s++;
	return s;
}

char* right_trim(char* s)
{
	size_t len = strlen(s);
	if (len == 0) return s;
	char* back = s + strlen(s)-1;
	while (isspace(*back)) {
		back--;
	}
	*(back + 1) = '\0';
	return s;
}

char* trim(char* s)
{
	return right_trim(left_trim(s));
}

/* Operaciones */

size_t cantidad_simbolos(char** symbols){
    size_t i = 0;
    while(symbols[i] != NULL) i++;
    return i;
}

void reportar_error(comando_t comando) {
	fprintf(stderr, "ERROR en comando %s\n", comando.nombre);
}

void sanitizar_simbolos(char** symbols) {
	for(size_t i = 0; symbols[i] != NULL; ++i) {
		symbols[i] = trim(symbols[i]);
	}
}

bool procesar_linea(char* linea, contexto_ejecucion_t* contexto) {
    char** symbols = split(linea, ' ');
	sanitizar_simbolos(symbols);
    size_t cantidad = cantidad_simbolos(symbols);
	comando_t comando = validar_comando(cantidad, symbols);
	bool estado = comando.es_valido ? ejecutar_comando(contexto, comando) : false;
	if (!estado)
		reportar_error(comando);
	free_comando(comando);
    free_strv(symbols);
    return estado;
}

int main(void) {
    char* linea = NULL;
    size_t n = 0;
	contexto_ejecucion_t* contexto = crear_contexto();
	if (!contexto) return 1;
    while (getline(&linea, &n, stdin) != -1) {
		if (procesar_linea(linea, contexto))
			printf("OK\n");
    }
	destruir_contexto(contexto);
    free(linea);
    return 0;
}