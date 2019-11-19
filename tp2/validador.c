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

const int MAX_PARAMETROS = 4;

hash_t* crear_tabla() {
	hash_t* hash = hash_crear(NULL);
	if (!hash) return NULL;
	hash_guardar(hash, "agregar_archivo", &validar_parametros_agregar_archivo);
	hash_guardar(hash, "ver_tablero", &validar_parametros_ver_tablero);
	hash_guardar(hash, "info_vuelo", &validar_parametros_info_vuelo);
	hash_guardar(hash, "prioridad_vuelos", &validar_parametros_prioridad_vuelos);
	hash_guardar(hash, "borrar", &validar_parametros_borrar);
	return hash;
}

comando_t validar_comando(int argc, char** argv)
{
	comando_t comando = { .es_valido = false };
	hash_t* tabla = crear_tabla();
	char* nombre_comando = argv[0];
	if(tabla && hash_pertenece(tabla, nombre_comando))
	{
		comando.cantidad_parametros = 0;
		char* parametros[4];//MAX_PARAMETROS];//
		for(int i = 1; i < argc; ++i)
		{
			parametros[(comando.cantidad_parametros)++] = argv[i];
		}
		comando.parametros = parametros;
		comando.es_valido = (*((validador_comando_t)hash_obtener(tabla, nombre_comando)))(comando);
	}
	if (tabla) hash_destruir(tabla);
	return comando;
}

bool validar_parametros_agregar_archivo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_ver_tablero(comando_t comando)
{
	return comando.cantidad_parametros == 4 && es_natural_valido(comando.parametros[0]) && es_orden(comando.parametros[1]) && es_fecha(comando.parametros[3]) && es_fecha(comando.parametros[4]);
}

bool validar_parametros_info_vuelo(comando_t comando)
{
	return comando.cantidad_parametros == 1;
}

bool validar_parametros_prioridad_vuelos(comando_t comando)
{
	return comando.cantidad_parametros == 1 && es_natural_valido(comando.parametros[0]);
}

bool validar_parametros_borrar(comando_t comando)
{
	return comando.cantidad_parametros == 2 && es_fecha(comando.parametros[0]) && es_fecha(comando.parametros[1]);
}