#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
	struct nodo* proximo;
	void* valor;
} nodo_t;

struct lista {
	nodo_t* inicio;
	nodo_t* fin;
	size_t largo;
};

nodo_t* crear_nodo(void* dato) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return NULL;
	nodo->valor = dato;
	nodo->proximo = NULL;
	return nodo;
}

void insertar_inicio(lista_t* lista, nodo_t* nuevo) {
	nuevo->proximo = lista->inicio;
	lista->inicio = nuevo;
	lista->largo++;
}

void insertar_final(lista_t* lista, nodo_t* nuevo) {
	nodo_t* ultimo = lista->fin;
	ultimo->proximo = nuevo;
	lista->fin = nuevo;
	lista->largo++;
}

void insertar_despues(lista_t* lista, nodo_t* anterior, nodo_t* nuevo) {
	nuevo->proximo = anterior->proximo;
	anterior->proximo = nuevo;
	lista->largo++;
}

void insertar_unico(lista_t* lista, nodo_t* nuevo) {
	lista->inicio = nuevo;
	lista->fin = nuevo;
	lista->largo++;
}

void* borrar_nodo(lista_t* lista, nodo_t* nodo) {
	void* dato = nodo->valor;
	free(nodo);
	lista->largo--;
	return dato;
}

void* borrar_fin(lista_t* lista, nodo_t* anterior) {
	nodo_t* actual = lista->fin;
	lista->fin = anterior;
	anterior->proximo = actual->proximo;
	return borrar_nodo(lista, actual);
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void) {
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista) return NULL;
	lista->largo = 0;
	lista->inicio = NULL;
	lista->fin = NULL;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
	return lista_largo(lista) == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
	nodo_t* nuevo = crear_nodo(dato);
	if(!nuevo) return false;
	if (lista_esta_vacia(lista))
		insertar_unico(lista, nuevo);
	else
		insertar_inicio(lista, nuevo);
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
	nodo_t* nuevo = crear_nodo(dato);
	if(!nuevo) return false;
	if (lista_esta_vacia(lista))
		insertar_unico(lista, nuevo);
	else
		insertar_final(lista, nuevo);
	return true;
}

void* lista_borrar_primero(lista_t *lista) {
	if (lista_esta_vacia(lista)) return NULL;
	nodo_t* primero = lista->inicio;
	lista->inicio = primero->proximo;
	return borrar_nodo(lista, primero);
}

void* lista_ver_primero(const lista_t *lista) {
	if(lista_esta_vacia(lista)) return NULL;
	return lista->inicio->valor;
}

void* lista_ver_ultimo(const lista_t* lista) {
	if(lista_esta_vacia(lista)) return NULL;
	return lista->fin->valor;
}

size_t lista_largo(const lista_t *lista) {
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)) {
	while(!lista_esta_vacia(lista)) {
		void* dato = lista_borrar_primero(lista);
		if(destruir_dato)
			destruir_dato(dato);
	}
	free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	nodo_t* actual = lista->inicio;
	while (actual && visitar(actual->valor, extra)) {
		actual = actual->proximo;
	}
}

/* ******************************************************************
 *                    PRIMITIVAS DE LOS ITERADORES
 * *****************************************************************/

struct lista_iter {
	lista_t* lista;
	nodo_t* anterior;
	nodo_t* actual;
};

lista_iter_t *lista_iter_crear(lista_t *lista) {
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter) return NULL;
	iter->lista = lista;
	iter->anterior = NULL;
	iter->actual = lista->inicio;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return false;
	iter->anterior = iter->actual;
	iter->actual = iter->actual->proximo;
	return true;
}

void* lista_iter_ver_actual(const lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return NULL;
	return iter->actual->valor;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
	return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
	/* La memoria de los nodos son de la lista, asi que no hay que borrarlos */
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
	/* Casos borde, si el iterador esta al inicio o al final */
	bool resultado = true;
	if(iter->actual == iter->lista->inicio) {
		resultado &= lista_insertar_primero(iter->lista, dato);
		if(resultado) iter->actual = iter->lista->inicio;
	}
	else if(iter->anterior == iter->lista->fin) {
		resultado &= lista_insertar_ultimo(iter->lista, dato);
		if(resultado) iter->actual = iter->lista->fin;
	}
	else {
		nodo_t* nuevo = crear_nodo(dato);
		resultado &= nuevo != NULL;
		if(resultado) {
			insertar_despues(iter->lista, iter->anterior, nuevo);
			iter->actual = nuevo;
		}
	}
	return resultado;
}

void* lista_iter_borrar(lista_iter_t *iter) {
	if (lista_iter_al_final(iter)) return NULL;
	if(iter->actual == iter->lista->inicio) {
		void* dato = lista_borrar_primero(iter->lista);
		iter->actual = iter->lista->inicio;
		return dato;
	}
	else if (iter->actual == iter->lista->fin) {
		void* dato = borrar_fin(iter->lista, iter->anterior);
		iter->actual = NULL;
		return dato;
	}
	nodo_t* nodo = iter->actual;
	iter->anterior->proximo = nodo->proximo;
	iter->actual = nodo->proximo;
	return borrar_nodo(iter->lista, nodo);
}