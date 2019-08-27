#ifndef TP0_H
#define TP0_H

/* Intercambia dos valores enteros.
 */
void swap(int *x, int *y);

/* Devuelve la posición del mayor elemento del arreglo, o -1 si el
 * el vector es de largo 0.
 */
int maximo(int vector[], int n);

/* Compara dos arreglos de longitud especificada.
 *
 * Devuelve -1 si el primer arreglo es menor que el segundo; 0 si son
 * iguales; o 1 si el primero es el mayor.
 *
 * Un arreglo es menor a otro cuando al compararlos elemento a elemento,
 * el primer elemento en el que difieren no existe o es menor.
 */
int comparar(int vector1[], int n1, int vector2[], int n2);

/* Ordena el arreglo recibido mediante el algoritmo de selección.
 *
 * Debe usarse swap() y maximo() en la implementación.
 */
void seleccion(int vector[], int n);

#endif  // TP0_H
