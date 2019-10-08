#define _POSIX_C_SOURCE 200809L /* Para el strdup */

#include "abb.h"
#include <string.h>

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
    return nodo;
}

/* Funcion auxiliar para destruir un nodo */
void destruir_nodo(abb_t* abb, abb_nodo_t* nodo) {
    if(abb->destructor)
        abb->destructor(nodo->value);
    free(nodo);
}

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

/* Devuelve el nodo en el cual se encuentra el valor o NULL si no existe. Preorder */
abb_nodo_t* buscar_recursivo(const char* clave, abb_nodo_t* nodo, abb_comparar_clave_t cmp) {
    if(!nodo) return NULL;
    int result = cmp(clave, nodo->key);
    if (result == 0) return nodo;
    else if(result < 0) buscar_recursivo(clave, nodo->left, cmp);
    return buscar_recursivo(clave, nodo->right, cmp);
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

void *abb_borrar(abb_t *arbol, const char *clave) {
    return NULL;
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
    char tu_hermana;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    return NULL;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return false;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    return;
}