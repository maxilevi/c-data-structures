#include "conversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool parsear_entero_si_es_valido(char* numero, size_t* n) {
	return sscanf(numero, "%zu", n) == 1 && *n > 0;
}

bool es_entero_valido(char* numero) {
	size_t n;
	return parsear_entero_si_es_valido(numero, &n);
}

size_t parsear_entero(char* numero) {
	size_t n;
	parsear_entero_si_es_valido(numero, &n);
	return n;
}

bool es_orden(char* orden) {
	return (strcmp(orden, "asc") == 0 || strcmp(orden, "desc") == 0);
}