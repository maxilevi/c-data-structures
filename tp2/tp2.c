#include "validador.h"
#include "ejecutor.h"
#include "strutil.h"
#include <stdio.h>

/* Operaciones */

size_t cantidad_simbolos(char** symbols){
    size_t i = 0;
    while(symbols[i] != NULL) i++;
    return i;
}

void reportar_error(comando_t comando) {
	fprintf(stderr, "ERROR al ejecutar comando '%s'", comando.nombre);
}

bool procesar_linea(char* linea, contexto_ejecucion_t* contexto) {
	bool estado;
    char** symbols = split(linea, ' ');
    size_t cantidad = cantidad_simbolos(symbols);
    comando_t comando = validar_comando(cantidad, symbols);
    if (comando.es_valido) {
		estado = ejecutar_comando(contexto, comando);
    }
	else {
		estado = false;
        reportar_error(comando);
    }

    free_strv(symbols);
    return estado;
}

int main(void) {
    char* linea = NULL;
    size_t n = 0;
	contexto_ejecucion_t* contexto = crear_contexto();
	if (!contexto) return;
    while (getline(&linea, &n, stdin) != -1) {;
        if (procesar_linea(linea, contexto))
			printf("OK\n");
    }
	destruir_contexto(contexto);
    free(linea);
    return 0;
}