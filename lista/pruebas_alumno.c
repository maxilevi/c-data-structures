#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


/* ******************************************************************
 *                   PRUEBAS UNITARIAS LISTA
 * *****************************************************************/

typedef struct comprobacion {
	int* expected;
	bool isOk;
	int index;
} comprobacion_t;

bool comprobador_auxiliar(void* dato, void* extra) {
	comprobacion_t* comp = extra;
	comp->isOk &= (*(int*)dato) == comp->expected[comp->index];
	comp->index++;
	return true;
}

void probar_crear_y_destruir() {
	lista_t* lista = lista_crear();
	print_test("Lista se crea correctamente", lista != NULL);
	lista_destruir(lista, NULL);
}

void probar_orden() {
	lista_t* lista = lista_crear();

	bool todo_ok = true;
	int tam = 5;
	char input[] = {'H', 'O', 'L', 'A', '\0'};
	char inv_input[] = {'\0', 'A', 'L', 'O', 'H'};

	for(int i = 0; i < tam; ++i) {
		todo_ok &= lista_insertar_ultimo(lista, &input[i]);
	}
	print_test("La lista no falla al insertar en el ultimo", todo_ok);
	for(int i = 0; i < tam; ++i) {
		char obtenido = (*((char*)lista_borrar_primero(lista)));
		todo_ok &= (obtenido == input[i]);
	}

	for(int i = 0; i < tam; ++i) {
		todo_ok &= lista_insertar_primero(lista, &input[i]);
	}
	print_test("La lista no falla al insertar en el inicio", todo_ok);
	for(int i = 0; i < tam; ++i) {
		char obtenido = (*((char*)lista_borrar_primero(lista)));
		todo_ok &= (obtenido == inv_input[i]);
	}

	print_test("La lista respeta el orden", todo_ok);

	lista_destruir(lista, NULL);
}

bool iterar_interno_auxiliar_sin_corte(void *elemento, void* extra) {
	int* suma = extra;
	(*suma) += (*(int*)elemento);
	return true;
}

bool iterar_interno_auxiliar(void *elemento, void* extra) {
	iterar_interno_auxiliar_sin_corte(elemento, extra);
	return *((int*)extra) < 30;
}

void probar_iterar_interno() {
	lista_t* lista = lista_crear();

	int input[] = {10, 10, 10, 10};
	bool todo_ok = true;
	const int tam = 4;
	for(int i = 0; i < tam; ++i) {
		todo_ok &= lista_insertar_primero(lista, &input[i]);
	}
	print_test("Se insertaron los numeros correctamente", todo_ok);

	int suma = 0;
	lista_iterar(lista, iterar_interno_auxiliar, &suma);
	print_test("Se itero correctamente hasta sumar 30", suma == 30);

	suma = 0;
	lista_iterar(lista, iterar_interno_auxiliar_sin_corte, &suma);
	print_test("Se itero la totalidad de los elementos", suma == 40);

	lista_destruir(lista, NULL);
}

void probar_iterar_interno_recorre_todos() {
	lista_t* lista = lista_crear();

	int input[] = {1, 2, 3, 4, 5, 6, 7, 8};
	comprobacion_t comp = {input, true, 0};
	lista_iterar(lista, comprobador_auxiliar, &comp);
	print_test("Iterador interno recorre todos los elementos", comp.isOk);

	lista_destruir(lista, NULL);
}

void probar_lista_ver_primero() {
	lista_t* lista = lista_crear();
	int tam = 9;
	bool todo_ok = true;
	char input[] = {'A','U','T','O','M','A','T','A','\0'};
	for(int i = 0; i < tam; ++i) {
		todo_ok &= lista_insertar_primero(lista, &input[i]);
		todo_ok &= ((char*)lista_ver_primero(lista)) == &input[i];
	}
	for(int i = 0; i < tam; ++i) {
		todo_ok &= ((char*)lista_borrar_primero(lista) == &input[tam-i-1]);
	}
	print_test("Lista ver primero devuelve el primero correctamente", todo_ok);
	print_test("Lista esta vacia luego de desenlistar", lista_esta_vacia(lista));
	lista_destruir(lista, NULL);
}

void probar_lista_ver_ultimo() {
	lista_t* lista = lista_crear();
	int tam = 9;
	bool todo_ok = true;
	char input[] = {'A','U','T','O','M','A','T','A','\0'};
	for(int i = 0; i < tam; ++i) {
		todo_ok &= lista_insertar_ultimo(lista, &input[i]);
		todo_ok &= ((char*)lista_ver_ultimo(lista)) == &input[i];
	}
	for(int i = 0; i < tam; ++i) {
		todo_ok &= ((char*)lista_borrar_primero(lista)) == &input[i];
	}
	print_test("Lista ver ultimo devuelve el ultimo correctamente", todo_ok);
	print_test("Lista esta vacia luego de desenlistar", lista_esta_vacia(lista));
	lista_destruir(lista, NULL);
}

void destruir_auxiliar(void* valor) {
	/* Marco que la funcion auxiliar de haya llamado sobre el elemento */
	(*((int*)valor)) = -1;
}

void probar_destruir_funcion() {
	lista_t* lista = lista_crear();
	size_t tam = 1000;
	int* valores = malloc(sizeof(int) * tam);
	bool todo_ok = true;
	for(int i = 0; i < tam; ++i) {
		valores[i] = 1;
		todo_ok &= lista_insertar_primero(lista, &valores[i]);
	}
	print_test("Largo de la lista es el mismo que la cantidad de elementos insertados", lista_largo(lista) == tam);
	print_test("Se añadieron los elementos correctamente", todo_ok);
	lista_destruir(lista, &destruir_auxiliar);
	todo_ok = true;
	for(int i = 0; i < tam; ++i) {
		/* Verifico que la funcion se haya llamado sobre todos los elementos */
		todo_ok &= (valores[i] == -1);
	}
	free(valores);
	print_test("Se destruyeron todos los elementos correctamente", todo_ok);
}

void probar_volumen() {
	lista_t* lista = lista_crear();

	size_t tam = 10000;
	size_t i = 0;
	bool resultado = true;
	bool resultado_orden = true;
	bool resultado_longitud = true;
	size_t datos_tam = 10;
	int datos[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(i = 0; i < tam; ++i) {
		resultado_longitud &= lista_largo(lista) == i;
		resultado &= lista_insertar_ultimo(lista, (void*) &datos[i % datos_tam]);
		resultado_longitud &= lista_largo(lista) == (i + 1);
	}
	print_test("No falla al enlistar 10000", resultado);
	print_test("Longitud de la lista se actualiza correctamente", resultado_longitud);
	print_test("Longitud de la lista es el mismo que la cantidad de elementos", lista_largo(lista) == tam);
	for(i = 0; i < tam; ++i) {
		resultado_longitud &= lista_largo(lista) == tam-i;
		void* output = lista_borrar_primero(lista);
		resultado &= (output != NULL);
		resultado_orden &= ((*((int*)output)) == datos[i % datos_tam]);
		resultado_longitud &= lista_largo(lista) == tam-i-1;
	}
	print_test("Lista con muchos respeta el invariante orden", resultado_orden);
	print_test("No falla al enlistar y desenlistar muchos (volumen)", resultado);
	print_test("Longitud de la lista se actualiza correctamente al sacar elementos", resultado_longitud);

	lista_destruir(lista, NULL);
}

void probar_esta_vacia() {
	lista_t* lista = lista_crear();
	print_test("Lista recien creada esta vacia", lista_esta_vacia(lista));
	print_test("Lista recien creada tiene largo 0", lista_largo(lista) == 0);
	int j = 241;
	lista_insertar_primero(lista, &j);
	print_test("Lista con un elemento no esta vacia", !lista_esta_vacia(lista));
	print_test("Lista con un elemento tiene largo 1", lista_largo(lista) == 1);
	print_test("Lista borra correctamente el unico elemento", (*((int*)lista_borrar_primero(lista))) == j);
	print_test("Lista sin elementos esta vacia", lista_esta_vacia(lista));
	print_test("Lista sin elemento tiene largo 0", lista_largo(lista) == 0);
	lista_destruir(lista, NULL);
}

void pruebas_lista() {
	probar_crear_y_destruir();
	probar_destruir_funcion();
	probar_lista_ver_ultimo();
	probar_orden();
	probar_volumen();
	probar_iterar_interno();
	probar_lista_ver_primero();
	probar_esta_vacia();
	probar_iterar_interno_recorre_todos();
}

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ITERADOR
 * *****************************************************************/

void probar_iter_crear() {
	lista_t* lista = lista_crear();
	lista_iter_t* iter = lista_iter_crear(lista);

	print_test("El iterador externo de la lista se creo correctamente", iter != NULL);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void probar_iter_inicio() {
	lista_t* lista = lista_crear();

	char* turing = "turing";
	char* pollock = "pollock";
	lista_insertar_primero(lista, "planck");
	lista_insertar_primero(lista, pollock);
	lista_insertar_primero(lista, turing);

	lista_iter_t* iter = lista_iter_crear(lista);

	print_test("Se borro el primer elemento correctamente usando el iterador externo", ((char*)lista_iter_borrar(iter)) == turing);
	print_test("Remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista", lista_ver_primero(lista) == pollock);

	char* beethoven = "beethoven";
	lista_iter_insertar(iter, beethoven);
	print_test("Insertar un elemento en la posición en la que se crea el iterador se inserta al principio", ((char*)lista_ver_primero(lista)) == beethoven);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void probar_iter_final() {
	lista_t* lista = lista_crear();

	char* planck = "planck";
	char* mozart = "mozart";
	lista_insertar_primero(lista, planck);
	lista_insertar_primero(lista, mozart);
	lista_insertar_primero(lista, "turing");

	lista_iter_t* iter = lista_iter_crear(lista);

	int i = 0;
	while (i < 2)
	{
		lista_iter_avanzar(iter);
		i++;
	}

	print_test("Se borro el ultimo elemento correctamente usando el iterador externo", ((char*)lista_iter_borrar(iter)) == planck);
	print_test("Remover el último elemento con el iterador cambia el último de la lista", lista_ver_ultimo(lista) == mozart);

	char* van_gogh = "van gogh";
	lista_iter_insertar(iter, van_gogh);
	print_test("Insertar un elemento en la posición final es equivalente a insertar al final", ((char*)lista_ver_ultimo(lista)) == van_gogh);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void probar_iter_insertar_medio() {
	lista_t* lista = lista_crear();
	int input[] = {1,2,3,4,5,16};

	lista_insertar_ultimo(lista, &input[0]);
	lista_insertar_ultimo(lista, &input[1]);
	lista_insertar_ultimo(lista, &input[2]);
	lista_insertar_ultimo(lista, &input[3]);
	lista_insertar_ultimo(lista, &input[4]);

	lista_iter_t* iter = lista_iter_crear(lista);

	int i = 0;
	while(i < 2)
	{
		lista_iter_avanzar(iter);
		i++;
	}
	print_test("El iterador de la lista avanza y se ve correctamente", 3 == *(int*)lista_iter_ver_actual(iter));
	lista_iter_insertar(iter, &input[5]);

	/* Verificar que el orden sea correcto usando el iterador interno */
	int esperado[] = {1,2,16,3,4,5};
	comprobacion_t comp = {esperado, true, 0};
	lista_iterar(lista, comprobador_auxiliar, &comp);
	print_test("Insertar un elemento en el medio mantiene el orden correctamente", comp.isOk);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void probar_iter_remover_medio() {
	lista_t* lista = lista_crear();
	int input[] = {1,2,3,4,5};

	lista_insertar_ultimo(lista, &input[0]);
	lista_insertar_ultimo(lista, &input[1]);
	lista_insertar_ultimo(lista, &input[2]);
	lista_insertar_ultimo(lista, &input[3]);
	lista_insertar_ultimo(lista, &input[4]);

	lista_iter_t* iter = lista_iter_crear(lista);

	int i = 0;
	while(i < 2)
	{
		lista_iter_avanzar(iter);
		i++;
	}
	print_test("El iterador de la lista avanza y se ve correctamente", 3 == *(int*)lista_iter_ver_actual(iter));
	lista_iter_borrar(iter);

	/* Verificar que el orden sea correcto usando el iterador interno */
	int esperado[] = {1,2,4,5};
	comprobacion_t comp = {esperado, true, 0};
	lista_iterar(lista, comprobador_auxiliar, &comp);
	print_test("Borrar un elemento en el medio mantiene el orden correctamente", comp.isOk);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void probar_iter_recorre_todos() {
	lista_t* lista = lista_crear();
	int input[] = { 1,2,3,4,5 };

	lista_insertar_ultimo(lista, &input[0]);
	lista_insertar_ultimo(lista, &input[1]);
	lista_insertar_ultimo(lista, &input[2]);
	lista_insertar_ultimo(lista, &input[3]);
	lista_insertar_ultimo(lista, &input[4]);

	bool todo_ok = true;
	int i = 0;
	lista_iter_t* iter = lista_iter_crear(lista);
	while (!lista_iter_al_final(iter))
	{
		todo_ok &= (*(int*)lista_iter_ver_actual(iter)) == input[i];
		lista_iter_avanzar(iter);
		i++;
	}
	print_test("El iterador de la lista recorre todos correctamente", todo_ok);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}


void prueba_iterador_insertar_y_borrar_sin_orden() {
	/* Este test es mas que nada para generar algun error de inconsistencia */
	lista_t* lista = lista_crear();
	int input[] = { 1,2,3,4,5 };

	bool todo_ok = true;
	int i = 0;
	lista_iter_t* iter = lista_iter_crear(lista);
	todo_ok &= lista_iter_insertar(iter, &input[i++]);
	todo_ok &= lista_iter_avanzar(iter);
	todo_ok &= lista_iter_insertar(iter, &input[i]);
	todo_ok &= (*(int*)lista_iter_borrar(iter)) == 2;
	todo_ok &= lista_iter_insertar(iter, &input[i++]);
	todo_ok &= lista_iter_avanzar(iter);
	todo_ok &= lista_iter_insertar(iter, &input[i++]);
	todo_ok &= (*(int*)lista_iter_borrar(iter)) == 3;
	todo_ok &= lista_iter_borrar(iter) == NULL;
	todo_ok &= lista_iter_borrar(iter) == NULL;

	print_test("Se completaron todas las operaciones correctamente", todo_ok);

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void pruebas_iterador() {
	probar_iter_crear();
	probar_iter_inicio();
	probar_iter_final();
	probar_iter_insertar_medio();
	probar_iter_remover_medio();
	probar_iter_recorre_todos();
	prueba_iterador_insertar_y_borrar_sin_orden();
}

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/

void pruebas_lista_alumno() {
	pruebas_lista();
	pruebas_iterador();
}