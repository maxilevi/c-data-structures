#define _POSIX_C_SOURCE 200809L
#include "ejecutor.h"
#include "heap.h"
#include <stdio.h>
#include <string.h>
#include "strutil.h"
#include "pila.h"

/* Definiciones */

typedef bool (*ejecutar_comando_t)(contexto_ejecucion_t*, comando_t);

typedef struct vuelo {
	char* codigo;
	char* airline;
	char* src_airport;
	char* dst_airport;
	char* tail;
	char* priority;
	char* horario;
	char* delay;
	char* airtime;
	char* cancelled;
	char** strv;
	char* clave_abb;
	size_t referencias;
} vuelo_t;

typedef struct iteracion_aux {
	pila_t* stack;
	bool resultado;
} iteracion_aux_t;

bool ejecutar_agregar_archivo(contexto_ejecucion_t*, comando_t);
bool ejecutar_ver_tablero(contexto_ejecucion_t*, comando_t);
bool ejecutar_info_vuelo(contexto_ejecucion_t*, comando_t);
bool ejecutar_prioridad_vuelos(contexto_ejecucion_t*, comando_t);
bool ejecutar_borrar(contexto_ejecucion_t*, comando_t);


static const ejecutar_comando_t FUNCIONES_PUNTEROS[] = {
	&ejecutar_agregar_archivo,
	&ejecutar_ver_tablero,
	&ejecutar_info_vuelo,
	&ejecutar_prioridad_vuelos,
	&ejecutar_borrar
};

static const char* FUNCIONES_PUNTEROS_NOMBRES[] = {
	"agregar_archivo",
	"ver_tablero",
	"info_vuelo",
	"prioridad_vuelos",
	"borrar"
};

static const size_t CANTIDAD_FUNCIONES = 5;
const char* CODIGO_MINIMO = "0000";
const char* CODIGO_MAXIMO = "9999";

/* Manejo de vuelos */

vuelo_t* crear_vuelo(char** valores) {
	vuelo_t* vuelo = malloc(sizeof(vuelo_t));
	if (!vuelo) return NULL;
	vuelo->codigo = valores[0];
	vuelo->airline = valores[1];
	vuelo->src_airport = valores[2];
	vuelo->dst_airport = valores[3];
	vuelo->tail = valores[4];
	vuelo->priority = valores[5];
	vuelo->horario = valores[6];
	vuelo->delay = valores[7];
	vuelo->airtime = valores[8];
	vuelo->cancelled = valores[9];
	vuelo->strv = valores;
	return vuelo;
}

void destruir_vuelo(void* vuelo_ptr) {
	vuelo_t* vuelo = (vuelo_t*)vuelo_ptr;
	free(vuelo->clave_abb);
	free_strv(vuelo->strv);
	free(vuelo);
}

void borrar_vuelo(contexto_ejecucion_t* contexto, vuelo_t* vuelo) {
	abb_borrar(contexto->vuelos_por_fecha, vuelo->clave_abb);
	hash_borrar(contexto->vuelos_por_codigo, vuelo->codigo);
	destruir_vuelo(vuelo);
}

void imprimir_vuelo(vuelo_t* vuelo) {
	printf(
		"%s %s %s %s %s %s %s %s %s %s\n",
		vuelo->codigo,
		vuelo->airline,
		vuelo->src_airport,
		vuelo->dst_airport,
		vuelo->tail,
		vuelo->priority,
		vuelo->horario,
		vuelo->delay,
		vuelo->airtime,
		vuelo->cancelled
	);
}


/* Funciones de comparacion */

int comparar_vuelos_prioridad(const void* x, const void* y) {
	vuelo_t* xvuelo = (vuelo_t*)x;
	vuelo_t* yvuelo = (vuelo_t*)y;
	int result = -strcmp(xvuelo->priority, yvuelo->priority);
	if (result == 0)
		return strcmp(xvuelo->codigo, yvuelo->codigo);
	return result;
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
	contexto->vuelos_por_fecha = abb_crear(strcmp, NULL);
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
	ejecutar_comando_t funcion = NULL;
	for(size_t i = 0; i < CANTIDAD_FUNCIONES; ++i) {
		if(strcmp(FUNCIONES_PUNTEROS_NOMBRES[i], comando.nombre) == 0)
			funcion = FUNCIONES_PUNTEROS[i];
	}
	return funcion(contexto, comando);
}

char* concatenar_horario_y_codigo(char* horario, char* codigo) {
	char** arr = malloc(sizeof(char*) * 3);
	if(!arr) return NULL;
	arr[0] = horario;
	arr[1] = codigo;
	arr[2] = NULL;
	char* concatenado = join(arr, '\0');
	free(arr);
	return concatenado;
}

/* Funciones de ejecucion */

bool ejecutar_agregar_archivo(contexto_ejecucion_t* contexto, comando_t comando) {
	FILE* archivo = fopen(comando.parametros[0], "r");
	if (!archivo) return false;

	/* Formato ejemplo de CSV 4608,OO,PDX,SEA,N812SK,08,2018-04-10T23:22:55,05,43,0 */
	char* linea = NULL;
	size_t n = 0;
	bool resultado = true;
	while (getline(&linea, &n, archivo) != -1) {
		char** valores = split(trim(linea), ',');
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

		/* Hago esto para poder comparar por horario y luego codigo */
		vuelo->clave_abb = concatenar_horario_y_codigo(vuelo->horario, vuelo->codigo);
		if(!vuelo->clave_abb) {
			free(vuelo);
			free_strv(valores);
			resultado = false;
			break;
		}

		if (hash_pertenece(contexto->vuelos_por_codigo, vuelo->codigo)) {
			vuelo_t* vuelo_viejo = hash_obtener(contexto->vuelos_por_codigo, vuelo->codigo);
			/* Borro el viejo y pongo el nuevo */
			abb_borrar(contexto->vuelos_por_fecha, vuelo_viejo->clave_abb);
		}
		abb_guardar(contexto->vuelos_por_fecha, vuelo->clave_abb, vuelo);
		hash_guardar(contexto->vuelos_por_codigo, vuelo->codigo, vuelo);

		/* No hago un free_strv porque se hace cuando se borra el vuelo */
	}
	free(linea);
	fclose(archivo);
	return resultado;
}

void imprimir_item_tablero(vuelo_t* vuelo) {
	printf("%s - %s\n", vuelo->horario, vuelo->codigo);
}

bool iterar_rango_aux_asc(const char* key, void* dato, void* extra) {
	size_t* contador = extra;
	imprimir_item_tablero((vuelo_t*)dato);
	(*contador)--;
	return *contador > 0;
}

bool iterar_rango_aux_desc(const char* key, void* dato, void* extra) {
	iteracion_aux_t* aux = extra;
	aux->resultado &= pila_apilar(aux->stack, dato);
	return aux->resultado;
}

bool ejecutar_ver_tablero(contexto_ejecucion_t* contexto, comando_t comando) {
	size_t cantidad = parsear_entero(comando.parametros[0]);
	bool ascendente = strcmp(comando.parametros[1], "asc") == 0;
	char* inicio = concatenar_horario_y_codigo(comando.parametros[2], (char*)CODIGO_MINIMO);
	if (!inicio) return false;
	char* fin = concatenar_horario_y_codigo(comando.parametros[3], (char*)CODIGO_MAXIMO);
	if (!fin) {
		free(inicio);
		return false;
	}

	size_t contador = cantidad;
	/* Si es desc creamos un stack y apilamos todo ahi */
	bool stack_existe = !ascendente;
	iteracion_aux_t aux = {
		.resultado = true,
		.stack = stack_existe ? pila_crear() : NULL,
	};
	if (stack_existe && !aux.stack) return false;

	abb_iterar_rango(
		contexto->vuelos_por_fecha,
		inicio,
		fin,
		ascendente ? &iterar_rango_aux_asc : &iterar_rango_aux_desc,
		ascendente ? (void*)&contador : (void*)&aux
	);

	/* aux.resultado == false sii func_aux == iterar_rango_aux_desc sii orden == desc sii stack_existe == true */
	if (!aux.resultado) {
		pila_destruir(aux.stack);
		return false;
	}

	if (stack_existe) {
		/* Vaciamos e imprimimos. Al ser una pila se imprime en reverso al asc */
		while (!pila_esta_vacia(aux.stack) && contador > 0) {
			imprimir_item_tablero((vuelo_t*)pila_desapilar(aux.stack));
			contador--;
		}
		pila_destruir(aux.stack);
	}

	free(inicio);
	free(fin);

	return true;
}

bool ejecutar_info_vuelo(contexto_ejecucion_t* contexto, comando_t comando) {
	char* codigo = comando.parametros[0];
	if (hash_pertenece(contexto->vuelos_por_codigo, codigo)) {
		vuelo_t* vuelo = (vuelo_t*) hash_obtener(contexto->vuelos_por_codigo, codigo);
		/* Ejemplo formato 4608 OO PDX SEA N812SK 08 2018-04-10T23:22:55 05 43 0 */
		imprimir_vuelo(vuelo);
		return true;
	}
	return false;
}

bool ejecutar_prioridad_vuelos(contexto_ejecucion_t* contexto, comando_t comando) {
	size_t k = parsear_entero(comando.parametros[0]);
	if (k > hash_cantidad(contexto->vuelos_por_codigo)) return false;
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
		/* Usamos un stack para imprimir al revez */
		pila_t* stack = pila_crear();
		if (stack) {
			while (!heap_esta_vacio(heap) && resultado) {
				resultado &= pila_apilar(stack, heap_desencolar(heap));
			}
			/* Imprimimos */
			while (!pila_esta_vacia(stack) && resultado) {
				vuelo_t* vuelo = pila_desapilar(stack);
				printf("%s - %s\n", vuelo->priority, vuelo->codigo);
			}
			pila_destruir(stack);
		}
		else {
			resultado = false;
		}
	}

	heap_destruir(heap, NULL);
	return resultado;
}

bool iterar_rango_aux_asc_borrar(const char* key, void* dato, void* extra) {
	iteracion_aux_t* aux = extra;
	imprimir_vuelo((vuelo_t*)dato);
	aux->resultado &= pila_apilar(aux->stack, dato);
	return aux->resultado;
}

bool ejecutar_borrar(contexto_ejecucion_t* contexto, comando_t comando) {
	char* inicio = concatenar_horario_y_codigo(comando.parametros[0], (char*)CODIGO_MINIMO);
	if (!inicio) return false;
	char* fin = concatenar_horario_y_codigo(comando.parametros[1], (char*)CODIGO_MAXIMO);
	if (!fin) {
		free(inicio);
		return false;
	}

	iteracion_aux_t aux = {
		.resultado = true,
		.stack = pila_crear(),
	};
	if (!aux.stack) return false;

	abb_iterar_rango(
		contexto->vuelos_por_fecha,
		inicio,
		fin,
		iterar_rango_aux_asc_borrar,
		&aux
	);

	/* Si fallo al apilar entonces devolvemos false */
	if (!aux.resultado) {
		pila_destruir(aux.stack);
		return false;
	}
	
	while (!pila_esta_vacia(aux.stack)) {
		vuelo_t* vuelo = (vuelo_t*)pila_desapilar(aux.stack);
		borrar_vuelo(contexto, vuelo);
	}

	pila_destruir(aux.stack);
	free(inicio);
	free(fin);
	return true;
}