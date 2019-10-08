#include <stdbool.h>
#include <stdlib.h>

typedef struct abb abb_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* Crea el ABB */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
void *abb_borrar(abb_t *arbol, const char *clave);

void *abb_obtener(const abb_t *arbol, const char *clave);
bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol);

/* Destruye el ABB */
void abb_destruir(abb_t *arbol);

/* Definicion del iterador del ABB */
typedef struct abb_iter abb_iter_t;

/* Crea el iterador del ABB */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza el iterador del ABB */ 
bool abb_iter_in_avanzar(abb_iter_t *iter);

const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
bool abb_iter_in_al_final(const abb_iter_t *iter);
void abb_iter_in_destruir(abb_iter_t* iter);