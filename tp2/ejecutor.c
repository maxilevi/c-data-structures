#include "ejecutor.h"
#include "heap.h"

/* Definiciones */

typedef bool (*ejecutar_comando_t)(contexto_ejecucion_t*, comando_t);
typedef struct vuelo {
	char* codigo;
	char* airline;
	char* src_airport;
	char* dst_airport;
	char* tail;
	size_t priority;
	fecha_t horario;
	char* delay;
	char* airtime;
	char* cancelled;
	char** strv;
} vuelo_t;

/* Operaciones */

contexto_ejecucion_t* crear_contexto() {
	contexto_ejecucion_t* contexto = malloc(sizeof(contexto_ejecucion_t));
	if (!contexto) return NULL;
	contexto->vuelos_por_codigo = hash_crear(destruir_vuelo);
	if (!contexto->vuelos_por_codigo) {
		destruir_contexto(contexto);
		return NULL;
	}
	contexto->vuelos_por_fecha = abb_crear(comparar_vuelos_fecha, destruir_vuelo);
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
	hash_t* tabla = crear_tabla();
	if (!tabla) return false;
	bool resultado = ((*((ejecutar_comando_t)hash_obtener(tabla, comando.nombre)))(contexto, comando));
	hash_destruir(tabla);
	return resultado;
}

/* Funciones de comparacion */

int comparar_vuelos_fecha(void* x, void* y) {
	vuelo_t* xvuelo = (vuelo_t*)x;
	vuelo_t* yvuelo = (vuelo_t*)y;
}

int comparar_vuelos_prioridad(void* x, void* y) {
	vuelo_t* xvuelo = (vuelo_t*)x;
	vuelo_t* yvuelo = (vuelo_t*)y;
	if (xvuelo->priority == yvuelo->priority)
		return strcmp(xvuelo->codigo, yvuelo->codigo);
	else if(xvuelo->priority < yvuelo->priority)
		return -1;
	return 1;
}

void destruir_vuelo(vuelo_t* vuelo) {
	free_strv(vuelo->strv);
	free(vuelo);
}

/* Funciones de ejecucion */

bool ejecutar_agregar_archivo(contexto_ejecucion_t* contexto, comando_t comando) {
	while {

	}
	/* Formato ejemplo de CSV 4608,OO,PDX,SEA,N812SK,08,2018-04-10T23:22:55,05,43,0 */
}

bool ejecutar_ver_tablero(contexto_ejecucion_t* contexto, comando_t comando) {

}

bool ejecutar_info_vuelo(contexto_ejecucion_t* contexto, comando_t comando) {
	char* codigo = comando.parametros[0];
	if (hash_pertenece(contexto->vuelos_por_codigo, codigo)) {
		vuelo_t* vuelo = (vuelo_t*) hash_obtener(contexto->vuelos_por_codigo, codigo);
		/* Ejemplo formato 4608 OO PDX SEA N812SK 08 2018-04-10T23:22:55 05 43 0 */
		printf(
			"%s %s %s %s %s %s %d-%d-%dT%d:%d:%d %s %s %s",
			vuelo->codigo,
			vuelo->airline,
			vuelo->dst_airport,
			vuelo->src_airport,
			vuelo->tail,
			vuelo->priority,
			vuelo->horario.anio, vuelo->horario.mes, vuelo->horario.dia, vuelo->horario.hora, vuelo->horario.minutos, vuelo->horario.segundos,
			vuelo->delay,
			vuelo->airtime,
			vuelo->cancelled
		);
		return true;
	}
	return false;
}

bool ejecutar_prioridad_vuelos(contexto_ejecucion_t* contexto, comando_t comando) {
	size_t k = parsear_natural(comando.parametros[0]);
	hash_iter_t* iter = hash_crear_iter(contexto->vuelos_por_codigo);
	heap_t* heap = heap_crear(comparar_vuelos_prioridad);
	bool resultado = true;

	/* Iteramos por el hash y hacemos top K */
	while (!hash_iter_al_final(contexto->vuelos_por_codigo) && resultado) {
		void* vuelo = hash_iter_ver_actual(contexto->vuelos_por_codigo);
		if (heap_cantidad(heap) < k) {
			if (!heap_encolar(heap, vuelo)) resultado = false;
		}
		else {
			if (comparar_vuelos_prioridad(vuelo, heap_ver_max(heap)) < 0) {
				heap_desencolar(heap);
				if (!heap_encolar(heap, vuelo)) resultado = false;
			}
		}
		hash_iter_avanzar(contexto->vuelos_por_codigo);
	}

	if (resultado) {
		/* Imprimimos */
		while (!heap_esta_vacio(heap)) {
			vuelo_t* vuelo = heap_desencolar(heap);
			printf("%d - %s", vuelo->priority, vuelo->codigo);
		}
	}

	heap_destruir(heap, NULL);
	return resultado;
}

bool ejecutar_borrar(contexto_ejecucion_t* contexto, comando_t comando) {

}

/* Tabla */

hash_t* crear_tabla() {
	hash_t* hash = hash_crear(NULL);
	if (!hash) return NULL;
	hash_guardar(hash, "agregar_archivo", &ejecutar_agregar_archivo);
	hash_guardar(hash, "ver_tablero", &ejecutar_ver_tablero);
	hash_guardar(hash, "info_vuelo", &ejecutar_info_vuelo);
	hash_guardar(hash, "prioridad_vuelos", &ejecutar_prioridad_vuelos);
	hash_guardar(hash, "borrar", &ejecutar_borrar);
	return hash;
}