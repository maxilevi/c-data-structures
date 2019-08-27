#include "tp0.h"
#include "testing.h"
#include <stddef.h>

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

/* Prueba que la funcion swap se ejecute correctamente. */
void prueba_swap() {
	int a = 5, b = 6;
	swap(&a,&b);
	print_test("Prueba swap 1", (a==6 && b==5));

	a = 10, b = -10;
	swap(&a,&b);
	print_test("Prueba swap 2", (a==-10 && b==10));

	a = 3, b = 3;
	swap(&a,&b);
	print_test("Prueba swap 3, numeros iguales", (a==3 && b==3));
}

#define VACIO NULL
#define NVACIO 0

/* Prueba que la funcion maximo se ejecute correctamente. */
void prueba_maximo() {
	/* Declaro los vectores a utilizar. */
	int unico[] = { 8 };
	int vector1[] = { 1, 2, 3, 4, 5 };
	int vector2[] = { -2000, -1500, -1000, -3000 };
	int vector3[] = { 8, -10, 27, 3, -50 };
	int vector4[] = { 8, -10, 27 };
	int vector5[] = { 100, -10, 27 };
	/* Declaro los largos de los vectores */
	int nunico = 1;
	int nvector1 = 5;
	int nvector2 = 4;
	int nvector3 = 5;
	int nvector4 = 3;
	int nvector5 = 3;

	print_test("Prueba maximo vacio", maximo(VACIO, NVACIO) == -1);
	print_test("Prueba maximo unico", maximo(unico, nunico) == 0);
	print_test("Prueba maximo vector1", maximo(vector1, nvector1) == 4);
	print_test("Prueba maximo vector2", maximo(vector2, nvector2) == 2);
	print_test("Prueba maximo vector3", maximo(vector3, nvector3) == 2);
	print_test("Prueba maximo vector4", maximo(vector4, nvector4) == 2);
	print_test("Prueba maximo vector5", maximo(vector5, nvector5) == 0);
}

/* Prueba que la funcion comparar se ejecute correctamente. */
void prueba_comparar() {
	/* Declaro los vectores a utilizar. */
	int unico[] = { 5 };
	int vec1[] = { 1, 2, 3, 4, 5 };
	int vec2[] = { 1, 2, 5 };
	int vec3[] = { 5, 8, 9 };
	int vec4[] = { 5, 8, 9, 10, 3 };
	int vec5[] = { 5, 8, 9, 10, 4 };
	int vec6[] = { 10, 7, 2, 1, -1 };
	int vec7[] = { -10 ,7 , 2,1, 1 };
	/* Declaro los largos de los vectores */
	int nunico = 1;
	int nvec1 = 5;
	int nvec2 = 3;
	int nvec3 = 3;
	int nvec4 = 5;
	int nvec5 = 5;
	int nvec6 = 5;
	int nvec7 = 5;

	print_test("Prueba comparar vacio, vacio",
	comparar(VACIO, NVACIO, VACIO, NVACIO) == 0);
	print_test("Prueba comparar vacio, unico",
	comparar(VACIO, NVACIO, unico, nunico) == -1);
	print_test("Prueba comparar unico, vec3",
	comparar(unico, nunico, vec3, nvec3) == -1);
	print_test("Prueba comparar vec1, vec1",
	comparar(vec1, nvec1, vec1, nvec1) == 0);
	print_test("Prueba comparar vec1, vec2",
	comparar(vec1, nvec1, vec2, nvec2) == -1);
	print_test("Prueba comparar vec2, vec1",
	comparar(vec2, nvec2, vec1, nvec1) == 1);
	print_test("Prueba comparar vec3, vec2",
	comparar(vec3, nvec3, vec2, nvec2) == 1);
	print_test("Prueba comparar vec3, vec4",
	comparar(vec3, nvec3, vec4, nvec4) == -1);
	print_test("Prueba comparar vec4, vec3",
	comparar(vec4, nvec4, vec3, nvec3) == 1);
	print_test("Prueba comparar vec6, vec7",
	comparar(vec6, nvec6, vec7, nvec7) == 1);
	print_test("Prueba comparar vec7, vec6",
	comparar(vec7, nvec7, vec6, nvec6) == -1);

	print_test("Prueba comparar slices",
		   comparar(vec4, nvec4-1, vec5, nvec5-1) == 0);

	{
	  int a[] = { 1, 2, -3, -4 };
	  int b[] = { 1, 2, -3 };
	  int c[] = { 1, 2, -5 };
	  int len_a = 4;
	  int len_b = 3;
	  int len_c = 3;

	  print_test("Prueba con valores negativos",
		     comparar(a, len_a, b, len_b) == 1);

	  print_test("Prueba con valores negativos",
		     comparar(a, len_a, c, len_c) == 1);
	}
}

/* Prueba de la funcion seleccion. */
void prueba_seleccion() {
	/* Declaro los vectores a utilizar. */
	int unico[] = {8}, unico_ord[] = {8};
	int vec1[] = {3, 5, 4, 2, 1}, vec1_ord[] = {1, 2, 3, 4, 5};
	int vec2[] = {4, 8, 15, 16, 23, 42}, vec2_ord[] = {4, 8, 15, 16, 23, 42};
	int vec3[] = {-38, -46, -65, -78}, vec3_ord[] = {-78, -65, -46, -38};

	/* Declaro los largos de los veces */
	int nunico = 1;
	int nvec1 = 5;
	int nvec2 = 6;
	int nvec3 = 4;

	/* Prueba que la funcion seleccion se ejecute correctamente. */
	seleccion(VACIO, NVACIO);
	print_test("Prueba seleccion vacio",
	comparar(VACIO, NVACIO, VACIO, NVACIO) == 0);

	seleccion(unico, nunico);
	print_test("Prueba seleccion unico",
	comparar(unico, nunico, unico_ord, nunico) == 0);

	seleccion(vec1, nvec1);
	print_test("Prueba seleccion vec1",
	comparar(vec1, nvec1, vec1_ord, nvec1) == 0);

	seleccion(vec2, nvec2);
	print_test("Prueba seleccion vec2",
	comparar(vec2, nvec2, vec2_ord, nvec2) == 0);

	seleccion(vec3, nvec3);
	print_test("Prueba seleccion vec3",
	comparar(vec3, nvec3, vec3_ord, nvec3) == 0);
}

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(void) {
	/* Ejecutar todas las pruebas unitarias. */
	prueba_swap();
	prueba_maximo();
	prueba_comparar();
	prueba_seleccion();

	return failure_count() > 0;
}
