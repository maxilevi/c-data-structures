#ifndef __EJECUTOR_H__
#define __EJECUTOR_H__

#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include "hash.h"
#include "abb.h"
#include "comando.h"
#include "conversion.h"

/* Definiciones */

typedef struct contexto_ejecucion {
	abb_t* vuelos_por_fecha;
	hash_t* vuelos_por_codigo;
} contexto_ejecucion_t;

/* Operaciones */

contexto_ejecucion_t* crear_contexto();

void destruir_contexto(contexto_ejecucion_t* contexto);

bool ejecutar_comando(contexto_ejecucion_t* contexto, comando_t comando);

#endif