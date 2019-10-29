#include "heap.h"
#include <string.h>
#include "testing.h"

int wrapper_strcmp(const void *a, const void *b) {
	return strcmp((char*)a, (char*)b);
}

void prueba_heap_ver_max() {
	heap_t* heap = heap_crear(wrapper_strcmp);
	const size_t tam = 5;
	char* expected[] = {"Bipartito", "Conexo", "Conexo", "Simple", "Simple" };
	char* shuffled[] = {"Bipartito", "Conexo", "Arbol", "Simple", "Camino" };
	bool todo_ok = heap_esta_vacio(heap);
	for (size_t i = 0; i < tam; ++i) {
		todo_ok &= heap_encolar(heap, shuffled[i]);
		todo_ok &= strcmp((char*)heap_ver_max(heap), expected[i]) == 0;
		todo_ok &= heap_cantidad(heap) == (i+1);
	}
	todo_ok &= !heap_esta_vacio(heap);
	heap_destruir(heap, NULL);
}

void prueba_heapsort() {
	const size_t tam = 5;
	char* expected[] = { "Arbol", "Bipartito", "Camino", "Conexo", "Simple" };
	char* shuffled[] = {"Bipartito", "Simple", "Arbol", "Conexo", "Camino" };
	heap_sort((void**)shuffled, tam, wrapper_strcmp);
	bool todo_ok = true;
	for (size_t i = 0; i < tam; ++i) {
		todo_ok &= strcmp(expected[i], shuffled[i]) == 0;
	}
	print_test("Heapsort ordena correctamente", todo_ok);
}

void prueba_heap_crear_arr() {
	char* expected[] = { "Simple", "Conexo", "Camino", "Bipartito", "Arbol" };
	char* shuffled[] = { "Bipartito", "Conexo", "Arbol", "Simple", "Camino" };
	const size_t tam = 5;
	bool todo_ok = true;
	heap_t* heap = heap_crear_arr((void**)shuffled, tam, wrapper_strcmp);
	for (size_t i = 0; i < tam; ++i) {
		todo_ok &= strcmp(heap_desencolar(heap), (char*)expected[i]) == 0;
	}
	print_test("Heapify crea un heap correcto", todo_ok);
	heap_destruir(heap, NULL);
}

void probar_volumen() {
	heap_t* heap = heap_crear(wrapper_strcmp);

	size_t tam = 10000;
	size_t i = 0;
	bool resultado = true;
	bool resultado_ordenado = true;
	int datos_tam = 10;
	int datos[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (i = 0; i < tam; ++i) {
		resultado &= heap_encolar(heap, (void*)&datos[i % datos_tam]);
		resultado &= heap_cantidad(heap) == (i+1);
		resultado &= !heap_esta_vacio(heap);
	}
	print_test("No falla al encolar 10000", resultado);

	int anterior = datos[datos_tam-1];
	for (i = 0; i < tam; ++i) {
		void* output = heap_desencolar(heap);
		resultado &= (output != NULL);
		int actual = (*((int*)output));
		resultado_ordenado &= actual <= anterior;
		anterior = actual;
		resultado &= heap_cantidad(heap) == (tam-i-1);
	}
	resultado &= heap_esta_vacio(heap);
	print_test("Heap con muchos respeta la prioridad", resultado_ordenado);
	print_test("No falla al encolar y desencolar muchos (volumen)", resultado);
	
	heap_destruir(heap, NULL);
}

void prueba_encolar_y_desencolar() {
	heap_t* heap = heap_crear(wrapper_strcmp);
	const size_t tam = 5;
	char* expected[] = { "Simple", "Conexo", "Camino", "Bipartito", "Arbol" };
	char* shuffled[] = { "Bipartito", "Simple", "Arbol", "Conexo", "Camino" };
	bool todo_ok = true;
	for (size_t i = 0; i < tam; ++i) {
		todo_ok &= heap_encolar(heap, shuffled[i]);
	}
	for (size_t i = 0; i < tam; ++i) {
		todo_ok &= strcmp(heap_desencolar(heap), (char*) expected[i]) == 0;
	}
	print_test("Heap se encolo y desencolo correctamente", todo_ok);
	heap_destruir(heap, NULL);
}

void pruebas_heap_alumno() {
    prueba_heap_ver_max();
    prueba_heapsort();
    prueba_heap_crear_arr();
    prueba_encolar_y_desencolar();
	probar_volumen();
}