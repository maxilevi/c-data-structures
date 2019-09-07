#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void probar_crear_y_destruir() {
	pila_t* pila = pila_crear();
	print_test("Pila se crea correctamente", pila != NULL);
	pila_destruir(pila);
}

void probar_invariante_LIFO() {
	pila_t* pila = pila_crear();

	bool todo_ok = true;
	int tam = 5;
	char input[] = {'H', 'O', 'L', 'A', '\0'};
	for(int i = 0; i < tam; ++i) {
		todo_ok &= pila_apilar(pila, &input[i]);
	}
	print_test("La pila no falla al apilar", todo_ok);
	char output[] = {'\0', 'A', 'L', 'O', 'H'};
	for(int i = 0; i < tam; ++i) {
		char obtenido = (*((char*)pila_desapilar(pila)));
		printf("%c", obtenido);
		todo_ok &= (obtenido == output[i]);
	}
	print_test("La pila respeta el invariante LIFO", todo_ok);

	pila_destruir(pila);
}

void probar_volumen() {
	pila_t* pila = pila_crear();

	size_t tam = 10000;
	size_t i = 0;
	bool resultado = true;
	for(i = 0; i < tam; ++i) {
		resultado &= pila_apilar(pila, (void*) &i);
	}
	for(i = 0; i < tam; ++i) {
		resultado &= (pila_desapilar(pila) != NULL);
	}

	print_test("No falla al apilar y desapilar muchos (volumen)", resultado);

	pila_destruir(pila);
}

void probar_apilar_null() {
	pila_t* pila = pila_crear();
	print_test("Se pudo apilar NULL", pila_apilar(pila, NULL));
	print_test("Pila no esta vacia", pila != NULL);
	pila_destruir(pila);
}

void probar_acciones_invalidas() {
	pila_t* pila = pila_crear();
	print_test("No se puede ver el tope al estar vacia", !pila_ver_tope(pila));
	print_test("Desapilar devuelve null si esta vacia", pila_desapilar(pila) == NULL);

	/* Casos borde */
	size_t tam = 100;
	size_t i = 0;
	bool resultado = true;
	for(i = 0; i < tam; ++i) {
		resultado &= pila_apilar(pila, (void*) &i);
	}
	for(i = 0; i < tam; ++i) {
		resultado &= (pila_desapilar(pila) != NULL);
	}
	print_test("Apilar y desapilar muchos", resultado);
	print_test("Se apilan y desapilan todos hasta estar vacia y ver tope devuelve false", !pila_ver_tope(pila));
	print_test("Se apilan y desapilan todos hasta estar vacia y desapilar devuelve NULL", pila_desapilar(pila) == NULL);

	pila_destruir(pila);
}

void probar_esta_vacia() {
	pila_t* pila = pila_crear();
	print_test("Pila recien creada esta vacia", pila_esta_vacia(pila));
	int k = 1;
	pila_apilar(pila, &k);
	print_test("Pila con un elemento no esta vacia", !pila_esta_vacia(pila));
	pila_destruir(pila);
}

void pruebas_pila_alumno() {
	probar_crear_y_destruir();
	probar_invariante_LIFO();
	probar_volumen();
	probar_apilar_null();
	probar_acciones_invalidas();
	probar_esta_vacia();
}