#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include <stdbool.h>

/* Formato YYYY-MM-DDTHH:MM:SS */
typedef struct fecha {
	bool es_valida;
	int anio;
	int mes;
	int dia;
	int hora;
	int mes;
	int minutos;
	int segundos;
} fecha_t;

fecha_t parsear_fecha(char* fecha);

bool fecha_valida(char* fecha);

bool es_natural_valido(char* numero);

size_t parsear_natural(char* numero);

int comparar_fechas(fecha_t fecha1, fecha_t fecha2);

#endif