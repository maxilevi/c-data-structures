#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void probar_crear_y_destruir() {
	cola_t* cola = cola_crear();
	print_test("Cola se crea correctamente", cola != NULL);
	cola_destruir(cola, NULL);
}

void probar_cola_ver_primero() {
	cola_t* cola = cola_crear();
	int tam = 9;
	bool todo_ok = true;
	char input[] = {'A','U','T','O','M','A','T','A','\0'};
	for(int i = 0; i < tam; ++i) {
		todo_ok &= cola_encolar(cola, &input[i]);
	}
	for(int i = 0; i < tam; ++i) {
		todo_ok &= ((char*)cola_ver_primero(cola)) == &input[i];
		todo_ok &= ((char*)cola_desencolar(cola) == &input[i]);
	}
	print_test("Cola ver primero devuelve el primero correctamente", todo_ok);
	print_test("Cola esta vacia luego de desencolar", cola_esta_vacia(cola));
	cola_destruir(cola, NULL);
}

void destruir_auxiliar(void* valor) {
	/* Marco que la funcion auxiliar de haya llamado sobre el elemento */
	(*((int*)valor)) = -1;
}

void probar_destruir_funcion() {
	cola_t* cola = cola_crear();
	size_t tam = 1000;
	int* valores = malloc(sizeof(int) * tam);
	bool todo_ok = true;
	for(int i = 0; i < tam; ++i) {
		valores[i] = 1;
		todo_ok &= cola_encolar(cola, &valores[i]);
	}
	print_test("Se encolaron los elementos correctamente", todo_ok);
	cola_destruir(cola, &destruir_auxiliar);
	todo_ok = true;
	for(int i = 0; i < tam; ++i) {
		/* Verifico que la funcion se haya llamado sobre todos los elementos */
		todo_ok &= (valores[i] == -1);
	}
	free(valores);
	print_test("Se destruyeron todos los elementos correctamente", todo_ok);
}

void probar_invariante_FIFO() {
	cola_t* cola = cola_crear();

	bool todo_ok = true;
	int tam = 5;
	char input[] = {'H', 'O', 'L', 'A', '\0'};
	for(int i = 0; i < tam; ++i) {
		todo_ok &= cola_encolar(cola, &input[i]);
	}
	print_test("La cola no falla al encolar", todo_ok);
	char output[] = {'H', 'O', 'L', 'A', '\0'};
	for(int i = 0; i < tam; ++i) {
		char obtenido = (*((char*)cola_desencolar(cola)));
		todo_ok &= (obtenido == output[i]);
	}
	print_test("La cola respeta el invariante FIFO", todo_ok);

	cola_destruir(cola, NULL);
}

void probar_volumen() {
	cola_t* cola = cola_crear();

	size_t tam = 10000;
	size_t i = 0;
	bool resultado = true;
	bool resultado_FIFO = true;
	size_t datos_tam = 10;
	int datos[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(i = 0; i < tam; ++i) {
		resultado &= cola_encolar(cola, (void*) &datos[i % datos_tam]);
	}
	print_test("No falla al encolar 10000", resultado);

	for(i = 0; i < tam; ++i) {
		void* output = cola_desencolar(cola);
		resultado &= (output != NULL);
		resultado_FIFO &= ((*((int*)output)) == datos[i % datos_tam]);
	}
	print_test("Cola con muchos respeta el invariante FIFO", resultado_FIFO);
	print_test("No falla al encolar y desencolar muchos (volumen)", resultado);

	cola_destruir(cola, NULL);
}

void probar_acciones_invalidas() {
	cola_t* cola = cola_crear();
	print_test("Cola desencolar devuelve NULL si vacia", cola_desencolar(cola) == NULL);
	print_test("Cola ver_primero devuelve NULL si vacia", cola_ver_primero(cola) == NULL);

	/* Casos borde */
	size_t tam = 100;
	size_t i = 0;
	bool resultado = true;
	for(i = 0; i < tam; ++i) {
		resultado &= cola_encolar(cola, (void*) &i);
	}
	for(i = 0; i < tam; ++i) {
		resultado &= (cola_desencolar(cola) != NULL);
	}
	print_test("Encolar y desencolar muchos", resultado);
	print_test("Cola esta vacio devuelve true luego de desencolar todo", cola_esta_vacia(cola));
	print_test("Se encolan y desencolan todos hasta estar vacia y ver primero devuelve false", cola_ver_primero(cola) == NULL);
	print_test("Se encolan y desencolan todos hasta estar vacia y desencolar devuelve NULL", cola_desencolar(cola) == NULL);

	cola_destruir(cola, NULL);
}

void probar_esta_vacia() {
	cola_t* cola = cola_crear();
	print_test("Cola recien creada esta vacia", cola_esta_vacia(cola));
	int j = 241;
	cola_encolar(cola, &j);
	print_test("Cola con un elemento no esta vacia", !cola_esta_vacia(cola));
	print_test("Cola desencola correctamente el unico elemento", (*((int*)cola_desencolar(cola))) == j);
	print_test("Cola desencolada esta vacia", cola_esta_vacia(cola));
	cola_destruir(cola, NULL);
}

void pruebas_cola_alumno() {
	probar_crear_y_destruir();
	probar_destruir_funcion();
	probar_invariante_FIFO();
	probar_volumen();
	probar_cola_ver_primero();
	probar_acciones_invalidas();
	probar_esta_vacia();
}