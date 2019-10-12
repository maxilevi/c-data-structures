#define _POSIX_C_SOURCE 200809L /* Para el strdup */

#include "abb.h"
#include "tdas/pila.h"
#include <string.h>
#include <stdio.h>

/* Definiciones */

typedef struct abb_nodo {
    struct abb_nodo* left;
    struct abb_nodo* right;
    void* value;
    char* key;
} abb_nodo_t;

struct abb {
    abb_nodo_t* root;
    abb_comparar_clave_t comparador;
    abb_destruir_dato_t destructor;
    size_t cantidad;
};

typedef void (*abb_traversal_t) (abb_t*, abb_nodo_t*);

/* Auxiliares */

/* Devuelve un nodo nuevo o NULL en caso de fallar */
abb_nodo_t* crear_nodo(const char* clave, void* dato) {
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if(!nodo) return NULL;
    nodo->key = strdup(clave);
    nodo->value = dato;
    nodo->left = NULL;
    nodo->right = NULL;
    return nodo;
}

/* Elimina y destruye el valor de un nodo si es necesario */
void eliminar_nodo(abb_t* abb, abb_nodo_t* nodo, bool borrar_dato) {
	if (abb->destructor && borrar_dato)
		abb->destructor(nodo->value);
	free(nodo);
}

/* Funcion auxiliar para destruir un nodo */
void destruir_nodo(abb_t* abb, abb_nodo_t* nodo) {
	eliminar_nodo(abb, nodo, true);
}

/* Itera por todos los nodos en post order, principalmente se usa para destruir el arbol */
void iterar_post_order(abb_t* abb, abb_nodo_t* nodo, abb_traversal_t funcion) {
    if(!nodo) return;
    iterar_post_order(abb, nodo->left, funcion);
    iterar_post_order(abb, nodo->right, funcion);
    funcion(abb, nodo);
}

/* Reemplaza el valor de un nodo por otro nuevo, usando la funcion de destruir para borrar el valor anterior */
bool reemplazar_existente(abb_t* abb, abb_nodo_t* nodo, void* nuevo) {
    if(abb->destructor)
        abb->destructor(nodo->value);
    nodo->value = nuevo;
    return true;
}

/* Inserta un nodo no existente en el arbol */
void insertar_nuevo(abb_nodo_t** root, abb_nodo_t* nodo, abb_comparar_clave_t cmp) {
    if(!(*root)) (*root) = nodo;
    else {
        int result = cmp(nodo->key, (*root)->key);
        if (result < 0)
            insertar_nuevo(&((*root)->left), nodo, cmp);
        else if(result > 0)
            insertar_nuevo(&((*root)->right), nodo, cmp);
    }
}

/* Devuelve un bool que representa si el nodo es una hoja */
bool es_hoja(abb_nodo_t* nodo) {
	return nodo->left == NULL && nodo->right == NULL;
}

/* Devuelve un bool que representa si el nodo tiene 1 hijo */
bool es_nodo_con_hijo(abb_nodo_t* nodo) {
	return nodo->left != NULL || nodo->right != NULL;
}

/* Busca iterativamente al nodo padre del nodo que contiene la clave */
abb_nodo_t** buscar_puntero_a_nodo_iterativo(abb_t* arbol, const char* clave) {
	abb_nodo_t** ptr_nodo = &arbol->root;
	abb_nodo_t* actual = arbol->root;
	int resultado;
	while ((resultado = arbol->comparador(clave, actual->key)) != 0) {
		ptr_nodo = resultado < 0 ? &actual->left : &actual->right;
		actual = resultado < 0 ? actual->left : actual->right;
	}
	return ptr_nodo;
}

/* Devuelve el nodo en el cual se encuentra el valor o NULL si no existe. Preorder */
abb_nodo_t* buscar_recursivo(const char* clave, abb_nodo_t* nodo, abb_comparar_clave_t cmp) {
    if(!nodo) return NULL;
    int result = cmp(clave, nodo->key);
    if (result == 0) return nodo;
    else if(result < 0) return buscar_recursivo(clave, nodo->left, cmp);
    return buscar_recursivo(clave, nodo->right, cmp);
}

/* Elimina un nodo hoja, borrando las referencias del padre */
void eliminar_hoja(abb_t* abb, const char* clave) {
	abb_nodo_t** ptr_nodo = buscar_puntero_a_nodo_iterativo(abb, clave);
	(*ptr_nodo) = NULL;
}

/* Elimina un nodo con 1 hijo */
void eliminar_nodo_con_hijo(abb_t* abb, const char* clave, abb_nodo_t* nodo) {
	abb_nodo_t* reemplazante = nodo->left ? nodo->left : nodo->right;
	abb_nodo_t** ptr_nodo = buscar_puntero_a_nodo_iterativo(abb, clave);
	(*ptr_nodo) = reemplazante;
}

/* Busca el menor para usar como reemplazante. Lo busca llendo todo a la izquierda recursivamente */
abb_nodo_t* buscar_minimo(abb_nodo_t* padre) {
	if (!padre) return NULL;
	abb_nodo_t* minimo = buscar_minimo(padre->left);
	return minimo == NULL ? padre : minimo;
}

/* Hace un swap de un nodo con 2 hijos por su sucesor en el inorder */
void swap_nodo_con_hijos(abb_nodo_t* nodo) {
	void* valor = nodo->value;
	void* clave = nodo->key;

	abb_nodo_t* reemplazante = buscar_minimo(nodo->right);
	/* Hacemos un swap de los nodos */
	nodo->key = reemplazante->key;
	nodo->value = reemplazante->value;
	reemplazante->key = clave;
	reemplazante->value = valor;
}

/* Primitivas */

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = malloc(sizeof(abb_t));
    if(!abb) return NULL;
    abb->comparador = cmp;
    abb->destructor = destruir_dato;
    abb->cantidad = 0;
    abb->root = NULL;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    abb_nodo_t* nodo = buscar_recursivo(clave, arbol->root, arbol->comparador);
    if(nodo) return reemplazar_existente(arbol, nodo, dato);

    abb_nodo_t* nuevo = crear_nodo(clave, dato);
    if(!nuevo) return false;
    insertar_nuevo(&arbol->root, nuevo, arbol->comparador);
    arbol->cantidad++;
    return true;
}

void* abb_borrar(abb_t *arbol, const char *clave) {
	abb_nodo_t* nodo = buscar_recursivo(clave, arbol->root, arbol->comparador);
	if (!nodo) return NULL;
	void* dato = nodo->value;

	/* Caso 1 */
	if (es_hoja(nodo)) {
		eliminar_hoja(arbol, clave);
	}
	/* Caso 2*/
	else if (es_nodo_con_hijo(nodo)) {
		eliminar_nodo_con_hijo(arbol, clave, nodo);
	}
	/* Caso 3 */
	else {
		swap_nodo_con_hijos(nodo);
		return abb_borrar(arbol, clave);
	}
	eliminar_nodo(arbol, nodo, false);
	arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    abb_nodo_t* nodo = buscar_recursivo(clave, arbol->root, arbol->comparador);
    return nodo ? nodo->value : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    return buscar_recursivo(clave, arbol->root, arbol->comparador);
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
    iterar_post_order(arbol, arbol->root, destruir_nodo);
    free(arbol);
}

/* Iterador */

struct abb_iter {
    pila_t* pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    iter->pila = pila_crear();
    if(!iter->pila) {
        free(iter);
        return NULL;
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    return pila_ver_tope(iter->pila);
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}