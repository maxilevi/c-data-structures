#include "conversion.h"
#include <stdlib.h>
#include <stdio.h>

fecha_t parsear_fecha(char* string) {
	fecha_t fecha;
	fecha.es_valida = (ssprintf(string, "%d-%d-%dT%d:%d:%d", &fecha.anio, &fecha.mes, &fecha.dia, &fecha.hora, &fecha.minutos, &fecha.segundos) == 6);
	return fecha;
}

bool fecha_valida(char* string) {
	fecha_t fecha = parsear_fecha(string);
	return fecha.es_valida;
}

int comparar_fechas(fecha_t* fecha1, fecha_t* fecha2) {
	return 0;
}

bool es_natural_valido(char* numero) {
	size_t n;
	return parsear_natural_si_es_valido(numero, &n);
}

size_t parsear_natural(char* numero) {
	size_t n;
	parsear_natural_si_es_valido(numero, &n);
	return n;
}

bool parsear_natural_si_es_valido(char* numero, size_t* n) {
	return sscanf(numero, "%d", &n) == 1;
}

bool es_orden(char* orden) {
	return (strcmp(orden, "asc") == 0 || strcmp(orden, "desc") == 0);
}