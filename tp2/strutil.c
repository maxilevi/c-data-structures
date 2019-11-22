#include "strutil.h"
#include "string.h"
#include "stdlib.h"

#include <string.h>
#include <ctype.h>

/* Este trio de funciones las use para el TP1, las añadi aca porque son utiles. */

char* left_trim(char* s) {
	while (isspace(*s)) s++;
	return s;
}

char* right_trim(char* s) {
	size_t len = strlen(s);
	if (len == 0) return s;
	char* back = s + strlen(s)-1;
	while (isspace(*back)) {
		back--;
	}
	*(back + 1) = '\0';
	return s;
}

char* trim(char* s) {
	return right_trim(left_trim(s));
}

char* substr(const char* str, size_t n) {
	char* copia = malloc(sizeof(char) * (n + 1));
	if (!copia) return NULL;
	strncpy(copia, str, n);
	copia[n] = '\0';
	return copia;
}

size_t cantidad_ocurrencias(const char* str, char elemento) {
	size_t count = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == elemento)
			count++;
	}
	return count;
}

char** split(const char* str, char sep) {
	size_t cantidad = cantidad_ocurrencias(str, sep) + 1;
	/* Sabemos que si hay n ocurrencias del separador va a convertirse en n+1 strings distintos */
	char** arreglo = malloc(sizeof(char*) * (cantidad + 1));
	if (!arreglo) return NULL;

	int inicio = 0;
	size_t largo = 0;
	int indice = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == sep) {
			arreglo[indice++] = substr(str + inicio, largo);
			largo = 0;
			inicio = i + 1;
		}
		else {
			largo++;
		}
	}
	arreglo[cantidad - 1] = substr(str + inicio, largo);
	/* Marcamos donde termina el arreglo */
	arreglo[cantidad] = NULL;
	return arreglo;
}

size_t calcular_largo_union(char** strv, size_t tam, char sep) {
	size_t largo = 0;
	for (int i = 0; i < tam; ++i) {
		/* Sumamos +1 por el espacio del caracter que los va a unir */
		int agregar_espacio = i+1 < tam && sep != '\0';
		/* Si el caracter es el nulo o el string es el ultimo del array entonces no necesitamos espacio extra */
		largo += strlen(strv[i]) + (agregar_espacio ? 1 : 0);
	}
	return largo;
}

size_t calcular_cant_strings(char** strv) {
	size_t count = 0;
	for (int i = 0; strv[i] != NULL; ++i) count++;
	return count;
}

char* join(char** strv, char sep) {
	/* Sumamos 1 al final por el \0 */
	size_t cant_strings = calcular_cant_strings(strv);
	size_t largo = calcular_largo_union(strv, cant_strings, sep) + 1;
	char* nuevo = malloc(sizeof(char) * largo);
	if (!nuevo) return NULL;

	int k = 0;
	int j = 0;
	/* Iteramos por todo el string resultante y vamos a�adiendo los caracteres uno a uno asi hacemos que el join sea O(N) */
	for (int i = 0; i < largo-1; ++i) {
		char siguiente = strv[j][k];
		if (siguiente == '\0') {
			j++;
			k = 0;
			/* 
			 * Si el separador es el caracter nulo entonces no hay espacio entre los strings y directamente a�adimos el siguiente.
			 * Pero tenemos que checkear de no estar en el ultimo string asi no hacemos un out of bounds 
			 */
			if (j < cant_strings && sep == '\0') {
				nuevo[i] = strv[j][k++];
			}
			else {
				nuevo[i] = sep;
			}
		}
		else {
			nuevo[i] = siguiente;
			k++;
		}
	}
	nuevo[largo - 1] = '\0';
	return nuevo;
}

void free_strv(char* strv[]) {
	for (int i = 0; strv[i] != NULL; ++i) {
		free(strv[i]);
	}
	free(strv);
}