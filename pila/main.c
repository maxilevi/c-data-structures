#include "pila.h"
#include "testing.h"
#include <stdio.h>

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_pila_catedra();

int main(void)
{
    /* Ejecuta todas las pruebas unitarias. */
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    pruebas_pila_alumno();

    #ifdef CORRECTOR
        printf("\n~~~ PRUEBAS CÁTEDRA ~~~\n");
        pruebas_pila_catedra();
    #endif 

    return failure_count() > 0;
}
