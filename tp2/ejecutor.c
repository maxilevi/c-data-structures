#include "ejecutor.h"
#include "heap.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include "strutil.h"

/* Definiciones */

typedef bool (*ejecutar_comando_t)(contexto_ejecucion_t*, comando_t);

typedef struct vuelo {
	char* codigo;
	char* airline;
	char* src_airport;
	char* dst_airport;
	char* tail;
	size_t priority;
	char* horario;
	char* delay;
	char* airtime;
	char* cancelled;
	char** strv;
} vuelo_t;

bool ejecutar_agregar_archivo(contexto_ejecucion_t*, comando_t);
bool ejecutar_ver_tablero(contexto_ejecucion_t*, comando_t);
bool ejecutar_info_vuelo(contexto_ejecucion_t*, comando_t);
bool ejecutar_prioridad_vuelos(contexto_ejecucion_t*, comando_t);
bool ejecutar_borrar(contexto_ejecucion_t*, comando_t);


const ejecutar_comando_t FUNCIONES_PUNTEROS[] = {
	&ejecutar_agregar_archivo,
	&ejecutar_ver_tablero,
	&ejecutar_info_vuelo,
	&ejecutar_prioridad_vuelos,
	&ejecutar_borrar
};

const char* FUNCIONES_PUNTEROS_NOMBRES[] = {
	"agregar_archivo",
	"ver_tablero",
	"info_vuelo",
	"prioridad_vuelos",
	"borrar"
};

const int CANTIDAD_FUNCIONES = 5;


/* Manejo de vuelos */

void destruir_vuelo(void* vuelo_ptr) {
	vuelo_t* vuelo = (vuelo_t*) vuelo_ptr;
	free_strv(vuelo->strv);
	free(vuelo);
}

vuelo_t* crear_vuelo(char** valores) {
	vuelo_t* vuelo = malloc(sizeof(vuelo_t));
	if (!vuelo) return NULL;
	vuelo->codigo = valores[0];
	vuelo->airline = valores[1];
	vuelo->src_airport = valores[2];
	vuelo->dst_airport = valores[3];
	vuelo->tail = valores[4];
	vuelo->priority = parsear_entero(valores[5]);
	vuelo->horario = valores[6];
	vuelo->delay = valores[7];
	vuelo->airtime = valores[8];
	vuelo->cancelled = valores[9];
	vuelo->strv = valores;
	return vuelo;
}

/* Funciones de comparacion */

int comparar_vuelos_prioridad(const void* x, const void* y) {
	vuelo_t* xvuelo = (vuelo_t*)x;
	vuelo_t* yvuelo = (vuelo_t*)y;
	if (xvuelo->priority == yvuelo->priority)
		return strcmp(xvuelo->codigo, yvuelo->codigo);
	else if (xvuelo->priority < yvuelo->priority)
		return -1;
	return 1;
}

/* Operaciones */

contexto_ejecucion_t* crear_contexto() {
	contexto_ejecucion_t* contexto = malloc(sizeof(contexto_ejecucion_t));
	if (!contexto) return NULL;
	contexto->vuelos_por_codigo = hash_crear(destruir_vuelo);
	if (!contexto->vuelos_por_codigo) {
		destruir_contexto(contexto);
		return NULL;
	}
	contexto->vuelos_por_fecha = abb_crear(strcmp, destruir_vuelo);
	if (!contexto->vuelos_por_fecha) {
		destruir_contexto(contexto);
		return NULL;
	}
	return contexto;
}

void destruir_contexto(contexto_ejecucion_t* contexto) {
	if(contexto->vuelos_por_fecha)
		abb_destruir(contexto->vuelos_por_fecha);
	if(contexto->vuelos_por_codigo)
		hash_destruir(contexto->vuelos_por_codigo);
	if(contexto)
		free(contexto);
}

bool ejecutar_comando(contexto_ejecucion_t* contexto, comando_t comando) {
	ejecutar_comando_t funcion;
	for(size_t i = 0; i < CANTIDAD_FUNCIONES; ++i) {
		if(strcmp(FUNCIONES_PUNTEROS_NOMBRES[i], comando.nombre))
			funcion = FUNCIONES_PUNTEROS[i];
	}
	bool resultado = funcion(contexto, comando);
	return resultado;
}

/* Funciones de ejecucion */

bool ejecutar_agregar_archivo(contexto_ejecucion_t* contexto, comando_t comando) {
	FILE* archivo = fopen(comando.parametros[0], "r");
	if (!archivo) return false;
	/* Formato ejemplo de CSV 4608,OO,PDX,SEA,N812SK,08,2018-04-10T23:22:55,05,43,0 */
	char* linea = NULL;
	size_t n = 0;
	bool resultado = true;
	while (getline(&linea, &n, stdin) != -1) {
		char** valores = split(linea, ',');
		if (!valores) {
			resultado = false;
			break;
		}

		vuelo_t* vuelo = crear_vuelo(valores);
		if (!vuelo) {
			free_strv(valores);
			resultado = false;
			break;
		}
		hash_guardar(contexto->vuelos_por_codigo, vuelo->codigo, vuelo);
		abb_guardar(contexto->vuelos_por_fecha, vuelo->horario, vuelo);

		/* No hago un free_strv porque se hace cuando se borra el vuelo*/
	}
	free(linea);
	fclose(archivo);
	return resultado;
}

bool ejecutar_ver_tablero(contexto_ejecucion_t* contexto, comando_t comando) {
	return false;
}

bool ejecutar_info_vuelo(contexto_ejecucion_t* contexto, comando_t comando) {
	char* codigo = comando.parametros[0];
	if (hash_pertenece(contexto->vuelos_por_codigo, codigo)) {
		vuelo_t* vuelo = (vuelo_t*) hash_obtener(contexto->vuelos_por_codigo, codigo);
		/* Ejemplo formato 4608 OO PDX SEA N812SK 08 2018-04-10T23:22:55 05 43 0 */
		printf(
			"%s %s %s %s %s %ld %s %s %s %s",
			vuelo->codigo,
			vuelo->airline,
			vuelo->dst_airport,
			vuelo->src_airport,
			vuelo->tail,
			vuelo->priority,
			vuelo->horario,
			vuelo->delay,
			vuelo->airtime,
			vuelo->cancelled
		);
		return true;
	}
	return false;
}

bool ejecutar_prioridad_vuelos(contexto_ejecucion_t* contexto, comando_t comando) {
	size_t k = parsear_entero(comando.parametros[0]);
	hash_iter_t* iter = hash_iter_crear(contexto->vuelos_por_codigo);
	if (!iter) return false;
	heap_t* heap = heap_crear(comparar_vuelos_prioridad);
	if (!heap) {
		hash_iter_destruir(iter);
		return false;
	}
	bool resultado = true;

	/* Iteramos por el hash y hacemos top K */
	while (!hash_iter_al_final(iter) && resultado) {
		const char* vuelo_codigo = hash_iter_ver_actual(iter);
		void* vuelo = hash_obtener(contexto->vuelos_por_codigo, vuelo_codigo);
		if (heap_cantidad(heap) < k) {
			if (!heap_encolar(heap, vuelo)) resultado = false;
		}
		else {
			if (comparar_vuelos_prioridad(vuelo, heap_ver_max(heap)) < 0) {
				heap_desencolar(heap);
				if (!heap_encolar(heap, vuelo)) resultado = false;
			}
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);

	if (resultado) {
		/* Imprimimos */
		while (!heap_esta_vacio(heap)) {
			vuelo_t* vuelo = heap_desencolar(heap);
			printf("%ld - %s", vuelo->priority, vuelo->codigo);
		}
	}

	heap_destruir(heap, NULL);
	return resultado;
}

bool ejecutar_borrar(contexto_ejecucion_t* contexto, comando_t comando) {
	return false;
}