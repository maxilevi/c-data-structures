#include "testing.h"
#include <stdio.h>

extern void pruebas_lista_alumno(void);
extern void pruebas_lista_catedra(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

int main(void) {
    /* Ejecuta todas las pruebas unitarias. */
    printf("~~~ PRUEBAS DEL ALUMNO ~~~\n");
    pruebas_lista_alumno();

    #ifdef CORRECTOR
        printf("\n~~~ PRUEBAS CÁTEDRA ~~~\n");
        pruebas_lista_catedra();
    #endif 

    return failure_count() > 0;
}
