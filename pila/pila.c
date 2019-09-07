#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

bool redimensionar(pila_t* pila, size_t capacidad_nueva) {
	void** datos_nuevos = realloc(pila->datos, capacidad_nueva * sizeof(void*));
	if (datos_nuevos == NULL) return false;

	pila->datos = datos_nuevos;
	pila->capacidad = capacidad_nueva;
	return true;
}

bool hay_que_achicar_pila(pila_t* pila) {
	return pila->cantidad == pila->capacidad / 4;
}

bool hay_que_agrandar_pila(pila_t* pila) {
	return pila->cantidad == pila->capacidad;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

const size_t CAPACIDAD_INICIAL = 8;

pila_t* pila_crear(void) {
	pila_t* pila = malloc(sizeof(pila_t));
	if (pila == NULL) return NULL;

	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INICIAL;
	pila->datos = malloc(pila->capacidad * sizeof(void*));

	if (pila->datos == NULL) {
		free(pila);
		return NULL;
	}
	return pila;
}

void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void* valor) {
	if(hay_que_agrandar_pila(pila)) {
		bool resultado = redimensionar(pila, pila->capacidad * 2);
		if (!resultado) return false;
	}
	pila->datos[pila->cantidad++] = valor;
	return true;
}

void* pila_ver_tope(const pila_t *pila) {
	if(pila_esta_vacia(pila)) return NULL;
	return pila->datos[pila->cantidad-1];
}

void* pila_desapilar(pila_t *pila) {
	if(pila_esta_vacia(pila)) return NULL;

	if(hay_que_achicar_pila(pila)) {
		bool resultado = redimensionar(pila, pila->capacidad / 2);
		if (!resultado) return NULL;
	}
	return pila->datos[(pila->cantidad--) - 1];
}
