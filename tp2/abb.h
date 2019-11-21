#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* Crea el ABB */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda en el ABB */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del ABB */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene un valor de un ABB a partir de una clave */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Devuelve un bool que indica si la clave pertenece al ABB */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve un size_t que indica la cantidad de elementos que contiene el ABB */
size_t abb_cantidad(abb_t *arbol);

/* Destruye el ABB */
void abb_destruir(abb_t *arbol);

/* Iterador interno del ABB */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* Itera el abb usando un rango como los bounds */
void abb_iterar_rango(abb_t* arbol, char* start, char* end, bool visitar(const char*, void*, void*), void* extra);

/* Definicion del iterador del ABB */
typedef struct abb_iter abb_iter_t;

/* Crea el iterador del ABB */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza el iterador del ABB */ 
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Devuelve la clave actual del iterador */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Devuelve un bool que representa si el iterador llego al final */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador */
void abb_iter_in_destruir(abb_iter_t* iter);