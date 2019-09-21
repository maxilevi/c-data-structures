#include <stdbool.h>
#include <stdlib.h>

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista. Devuelve NULL en caso de error.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Pre: la lista fue creada.
// Post: Devuelve un bool que representa si la lista esta vacia.
bool lista_esta_vacia(const lista_t *lista);

// Pre: la lista fue creada. Devuelve falso en caso de error.
// Post: Inserta un elemento al inicio de la lista
bool lista_insertar_primero(lista_t *lista, void *dato);

// Pre: la lista fue creada. Devuelve falso en caso de error.
// Post: Inserta un elemento al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Pre: la lista fue creada. Devuelve NULL si la lista esta vacia.
// Post: Remueve el elemento al inicio de la lista y devuelve su valor.
void* lista_borrar_primero(lista_t *lista);

// Pre: la lista fue creada. Devuelve NULL si la lista esta vacia.
// Post: Devuelve el valor al principio de la lista.
void* lista_ver_primero(const lista_t *lista);

// Pre: la lista fue creada. Devuelve NULL si la lista esta vacia.
// Post: Devuelve el valor al final de la lista.
void* lista_ver_ultimo(const lista_t* lista);

// Pre: la lista fue creada.
// Post: Devuelve un size_t que representa la longitud de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

// Pre: la lista fue creada. La funcion visitar debe ser un puntero valido.
// Post: Ejecuta la funcion visitar una vez por cada elemento de la lista hasta llegar al final o si devuelve false.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea un iterador de lista. Devuelve NULL en caso de error.
// Post: devuelve un nuevo iterador de lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Pre: el iterador fue creado.
// Post: mueve la posicion del iterador al siguiente elemento de la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

// Pre: el iterador fue creado. Que el iterador no este al final.
// Post: Devuelve el valor que se encuentra en la posicion actual del iterador 
void* lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: Devuelve un bool que indica si el iterador se encuentra al final de la lista (se pasaron por todos los elementos)
bool lista_iter_al_final(const lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: Destruye el iterador y libera su memoria.
void lista_iter_destruir(lista_iter_t *iter);

// Pre: el iterador fue creado.
// Post: Inserta un elemento en la posicion actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Pre: el iterador fue creado.
// Post:  Borra el elemento que se encuentra en la posicion actual del iterador
void *lista_iter_borrar(lista_iter_t *iter);