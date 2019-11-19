#ifndef __COMANDO_H__
#define __COMANDO_H__

/* Definiciones */

typedef struct comando {
	bool es_valido;
	char* nombre;
	char** parametros;
	int cantidad_parametros;
} comando_t;

#endif