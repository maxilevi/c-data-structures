#include "validador.h"
#include "conversion.h"
#include "hash.h"
#include <string.h>
#include <stdbool.h>

/* Definiciones */
typedef bool (*validador_comando_t)(comando_t);

bool validar_parametros_agregar_archivo(comando_t comando);
bool validar_parametros_ver_tablero(comando_t comando);
bool validar_parametros_info_vuelo(comando_t comando);
bool validar_parametros_prioridad_vuelos(comando_t comando);
bool validar_parametros_borrar(comando_t comando);

const validador_comando_t FUNCIONES_PUNTEROS[] = {
	&validar_parametros_agregar_archivo,
	&validar_parametros_ver_tablero,
	&validar_parametros_info_vuelo,
	&validar_parametros_prioridad_vuelos,
	&validar_parametros_borrar
};

const char* FUNCIONES_PUNTEROS_NOMBRES[] = {
	"agregar_archivo",
	"ver_tablero",
	"info_vuelo",
	"prioridad_vuelos",
	"borrar"
};

const int CANTIDAD_FUNCIONES = 5;

const int MAX_PARAMETROS = 4;

comando_t validar_comando(size_t argc, char** argv)
{
	comando_t comando = { .es_valido = false };
	char* nombre_comando = argv[0];
	validador_comando_t funcion = NULL;
	for(size_t i = 0; i < CANTIDAD_FUNCIONES; ++i) {
		if(strcmp(FUNCIONES_PUNTEROS_NOMBRES[i], nombre_comando))
			funcion = FUNCIONES_PUNTEROS[i];
	}
	if(funcion != NULL)
	{
		comando.cantidad_parametros = 0;
		char* parametros[4];//MAX_PARAMETROS];
		for(size_t i = 1; i < argc; ++i)
		{
			parametros[(comando.cantidad_parametros)++] = argv[i];
		}
		comando.parametros = parametros;
		comando.es_valido = funcion(comando);
	}
	return comando;
}

bool validar_parametros_agregar_archivo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_ver_tablero(comando_t comando)
{
	return comando.cantidad_parametros == 4 && es_entero_valido(comando.parametros[0]) && es_orden(comando.parametros[1]) && comando.parametros[3] && comando.parametros[4];
}

bool validar_parametros_info_vuelo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_prioridad_vuelos(comando_t comando)
{
	return comando.cantidad_parametros == 1 && es_entero_valido(comando.parametros[0]);
}

bool validar_parametros_borrar(comando_t comando)
{
	return comando.cantidad_parametros == 2 && strcmp(comando.parametros[0], comando.parametros[1]) < 0;
}