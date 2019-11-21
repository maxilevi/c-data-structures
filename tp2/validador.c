#include "validador.h"
#include "conversion.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

/* Definiciones */
typedef bool (*validador_comando_t)(comando_t);

bool validar_parametros_agregar_archivo(comando_t comando);
bool validar_parametros_ver_tablero(comando_t comando);
bool validar_parametros_info_vuelo(comando_t comando);
bool validar_parametros_prioridad_vuelos(comando_t comando);
bool validar_parametros_borrar(comando_t comando);

static const validador_comando_t FUNCIONES_PUNTEROS[] = {
	&validar_parametros_agregar_archivo,
	&validar_parametros_ver_tablero,
	&validar_parametros_info_vuelo,
	&validar_parametros_prioridad_vuelos,
	&validar_parametros_borrar
};

static const char* FUNCIONES_PUNTEROS_NOMBRES[] = {
	"agregar_archivo",
	"ver_tablero",
	"info_vuelo",
	"prioridad_vuelos",
	"borrar"
};

static const size_t CANTIDAD_FUNCIONES = 5;

const int MAX_PARAMETROS = 4;

comando_t validar_comando(size_t argc, char** argv)
{
	if (argc <= 1)
		return (comando_t) { .es_valido = false, .nombre = (argc == 1 ? argv[0] : NULL) };

	char* nombre_comando = argv[0];
	comando_t comando = { .es_valido = false, .nombre = nombre_comando };
	validador_comando_t funcion = NULL;
	for(size_t i = 0; i < CANTIDAD_FUNCIONES; ++i) {
		if(strcmp(FUNCIONES_PUNTEROS_NOMBRES[i], comando.nombre) == 0)
			funcion = FUNCIONES_PUNTEROS[i];
	}
	if(funcion != NULL)
	{
		comando.cantidad_parametros = argc-1;
		char** parametros = malloc(sizeof(char*) * comando.cantidad_parametros);
		if (!parametros) return comando;
		for(size_t i = 1; i < argc; ++i)
		{
			parametros[i-1] = argv[i];
		}
		comando.parametros = parametros;
		comando.es_valido = funcion(comando);
	}
	return comando;
}

void free_comando(comando_t comando) {
	free(comando.parametros);
}

bool validar_parametros_agregar_archivo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_ver_tablero(comando_t comando)
{
	return comando.cantidad_parametros == 4 
		&& es_entero_valido(comando.parametros[0]) 
		&& es_orden(comando.parametros[1]) 
		/* Valido que la fecha de comienzo sea menor o igual a la de final */
		&& strcmp(comando.parametros[2], comando.parametros[3]) <= 0;
}

bool validar_parametros_info_vuelo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_prioridad_vuelos(comando_t comando)
{
	return comando.cantidad_parametros == 1 
		&& es_entero_valido(comando.parametros[0]);
}

bool validar_parametros_borrar(comando_t comando)
{
	return comando.cantidad_parametros == 2 
		/* Valido que la fecha de comienzo sea menor o igual a la de final */
		&& strcmp(comando.parametros[0], comando.parametros[1]) <= 0;
}