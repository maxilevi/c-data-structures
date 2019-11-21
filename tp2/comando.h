#ifndef __COMANDO_H__
#define __COMANDO_H__

/* Definiciones */

typedef struct comando {
	bool es_valido;
	char* nombre;
	char** parametros;
	size_t cantidad_parametros;
} comando_t;

#endif