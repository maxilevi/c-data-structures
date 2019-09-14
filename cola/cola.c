#include "cola.h"
#include <stdlib.h>

typedef struct nodo {
	void* valor;
	struct nodo* proximo;
} nodo_t;

struct cola {
	nodo_t* inicio;
	nodo_t* fin;
};

nodo_t* crear_nodo() {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL) return NULL;
	nodo->valor = NULL;
	nodo->proximo = NULL;
	return nodo;
}

void destruir_nodo(nodo_t* nodo) {
	free(nodo);
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t* cola_crear(void) {
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL) return NULL;
	cola->inicio = NULL;
	cola->fin = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	nodo_t* actual = cola->inicio;
	while (actual != NULL) {
		if(destruir_dato != NULL)
			destruir_dato(actual->valor);

		nodo_t* previo = actual;
		actual = previo->proximo;
		destruir_nodo(previo);
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
	return cola->inicio == NULL;
}

bool cola_encolar(cola_t *cola, void* valor) {
	nodo_t* nuevo_nodo = crear_nodo();
	if(nuevo_nodo == NULL) return false;

	nuevo_nodo->valor = valor;
	nuevo_nodo->proximo = NULL;

	if(cola->fin != NULL)
		cola->fin->proximo = nuevo_nodo;
	cola->fin = nuevo_nodo;
	if (cola->inicio == NULL)
		cola->inicio = nuevo_nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;
	return cola->inicio->valor;
}

void* cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola)) return NULL;

	nodo_t* primero = cola->inicio;
	void* valor = primero->valor;

	cola->inicio = primero->proximo;
	if(cola->inicio == NULL) {
		cola->fin = NULL;
	}
	
	destruir_nodo(primero);
	return valor;
}