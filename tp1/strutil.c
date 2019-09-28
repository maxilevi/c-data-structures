#include "strutil.h"
#include "string.h"
#include "stdlib.h"

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

size_t calcular_largo_union(char** strv) {
	size_t largo = 0;
	for (int i = 0; strv[i] != NULL; ++i) {
		/* Sumamos +1 por el espacio del caracter que los va a unir */
		largo += strlen(strv[i]) + 1;
	}
	return largo;
}

char* join(char** strv, char sep) {
	/* Sumamos 1 al final por el \0 */
	size_t largo = calcular_largo_union(strv) + 1;
	char* nuevo = malloc(sizeof(char) * largo);
	if (!nuevo) return NULL;

	char separador[] = { sep, '\0' };
	memset(nuevo, '\0', largo);
	for (int i = 0; strv[i] != NULL; ++i) {
		if(i != 0)
			strcat(nuevo, separador);
		strcat(nuevo, strv[i]);
	}
	return nuevo;
}

void free_strv(char* strv[]) {
	for (int i = 0; strv[i] != NULL; ++i) {
		free(strv[i]);
	}
	free(strv);
}