#include "heap.h"
#include <stdlib.h>

/* Definiciones */
struct heap {
    void** datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

static const size_t CAPACIDAD_INICIAL = 8;

/* Internas */

size_t maximo(size_t x, size_t y) {
	return x > y ? x : y;
}

size_t padre(size_t index) {
    return (size_t) (index-1) / 2;
}

size_t hijo_izq(size_t index) {
    return 2 * index + 1;
}

size_t hijo_der(size_t index) {
    return 2 * index + 2;
}

void swap(void** datos, size_t x, size_t y) {
    void* tmp = datos[x];
    datos[x] = datos[y];
    datos[y] = tmp;
}

void downheap(void** datos, size_t cantidad, cmp_func_t cmp, size_t index) {
    size_t indice_izq = hijo_izq(index);
    size_t indice_der = hijo_der(index);
    void* hijo_izq = indice_izq < cantidad ? datos[indice_izq] : NULL;
    void* hijo_der = indice_der < cantidad ? datos[indice_der] : NULL;
    size_t nuevo_indice = index;
    if(hijo_izq != NULL && cmp(hijo_izq, datos[nuevo_indice]) > 0) nuevo_indice = indice_izq;
    if(hijo_der != NULL && cmp(hijo_der, datos[nuevo_indice]) > 0) nuevo_indice = indice_der;
    if (nuevo_indice != index) {
        swap(datos, index, nuevo_indice);
        downheap(datos, cantidad, cmp, nuevo_indice);
    }
}

void upheap(void** datos, cmp_func_t cmp, size_t index) {
    void* actual = datos[index];
    size_t indice_padre = padre(index);
    void* padre = index > 0 ? datos[indice_padre] : NULL;
    if (padre != NULL && cmp(actual, padre) > 0) {
        swap(datos, index, indice_padre);
        upheap(datos, cmp, indice_padre);
    }
}

bool redimensionar(heap_t* heap, size_t capacidad_nueva) {
	void** datos_nuevos = realloc(heap->datos, capacidad_nueva * sizeof(void*));
	if (datos_nuevos == NULL) return false;

	heap->datos = datos_nuevos;
	heap->capacidad = capacidad_nueva;
	return true;
}

bool hay_que_achicar_heap(heap_t* heap) {
	return heap->cantidad == heap->capacidad / 4 && heap->capacidad > CAPACIDAD_INICIAL;
}

bool hay_que_agrandar_heap(heap_t* heap) {
	return heap->cantidad == heap->capacidad;
}

/* Primitivas */

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (heap == NULL) return NULL;

    heap->cmp = cmp;
	heap->cantidad = 0;
	heap->capacidad = CAPACIDAD_INICIAL;
	heap->datos = malloc(heap->capacidad * sizeof(void*));

	if (heap->datos == NULL) {
		free(heap);
		return NULL;
	}
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = heap_crear(cmp);
	if (!heap) return NULL;
	size_t capacidad = maximo(CAPACIDAD_INICIAL, n);
	bool resultado = redimensionar(heap, capacidad);
	if (!resultado) {
		heap_destruir(heap, NULL);
		return NULL;
	}
	heap->cantidad = n;

	for (size_t i = heap->cantidad; i > 0; --i) {
		heap->datos[i-1] = arreglo[i-1];
		downheap(heap->datos, heap->cantidad, cmp, i-1);
	}
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)) {
    for(size_t i = 0; i < heap->cantidad; ++i) {
		if(destruir_elemento)
			destruir_elemento(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap_cantidad(heap) == 0;
}

void *heap_ver_max(const heap_t *heap) {
    if(heap_esta_vacio(heap)) return NULL;
    return heap->datos[0];
}

bool heap_encolar(heap_t *heap, void *elem) {
    if(hay_que_agrandar_heap(heap)) {
        bool resultado = redimensionar(heap, heap->capacidad * 2);
		if (!resultado) return false;
    }
    heap->datos[heap->cantidad] = elem;
    upheap(heap->datos, heap->cmp, heap->cantidad++);
    return true;
}

void *heap_desencolar(heap_t *heap) {
    if(heap_esta_vacio(heap)) return NULL;
    void* max = heap->datos[0];
    heap->datos[0] = heap->datos[heap->cantidad-1];
    downheap(heap->datos, --heap->cantidad, heap->cmp, 0);
    if(hay_que_achicar_heap(heap))
        redimensionar(heap, heap->capacidad / 2);
    /* Si falla igual devolvemos */
    return max;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    /* Hacemos un heapify sobre el arreglo */
    for(size_t i = cant; i > 0; --i) {
        downheap(elementos, cant, cmp, i-1);
    }
    for(size_t i = cant; i > 0; --i) {
        swap(elementos, i-1, 0);
        downheap(elementos, i-1, cmp, 0);
    }
}