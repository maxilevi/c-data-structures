#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap (int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}


int maximo(int vector[], int n) {
    if(n == 0) return -1; 
    int posicion = 0;
    for(int i = 0; i < n; ++i) {
        if(vector[i] > vector[posicion]) {
            posicion = i;
        }
    }
    return posicion;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    int minimo = n1 < n2 ? n1 : n2;
    for(int i = 0; i < minimo; ++i) {
        int num1 = vector1[i];
        int num2 = vector2[i];
        if(num1 != num2)
            return num1 < num2 ? -1 : 1;
    }
    if(n1 == n2)
        return 0;
    else if(n1 < n2)
        return -1;
    else
        return 1;
}


void seleccion(int vector[], int n) {
    for(int i = n-1; i > -1; --i) {
        swap(&vector[i], &vector[maximo(vector, i+1)]);
    }
}
