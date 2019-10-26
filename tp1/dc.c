#include "pila.h"
#include "strutil.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Definiciones */

typedef enum op_result {
	OP_EXITOSA,
	OP_ERROR
} op_result_t;

typedef enum op_type {
	OP_SUMA,
	OP_RESTA,
	OP_DIVISION,
	OP_MULTIPLICAR,
	OP_POTENCIA,
	OP_LOGARITMO,
	OP_TERNARIA,
	OP_RAIZ
} op_type_t;

typedef op_result_t(*operacion_ternaria_t)(int, int, int, int*);
typedef op_result_t(*operacion_binaria_t)(int, int, int*);
typedef op_result_t(*operacion_unaria_t)(int, int*);

typedef struct entry_unaria {
	op_type_t tipo;
	operacion_unaria_t op;
} entry_unaria_t;

typedef struct entry_binaria {
	op_type_t tipo;
	operacion_binaria_t op;
} entry_binaria_t;

typedef struct entry_ternaria {
	op_type_t tipo;
	operacion_ternaria_t op;
} entry_ternaria_t;

typedef struct entry_operador {
	char* nombre;
	int cantidad;
	op_type_t tipo;
} entry_operador_t;

op_result_t sumar(int op1, int op2, int* result);
op_result_t restar(int op1, int op2, int* result);
op_result_t multiplicar(int op1, int op2, int* result);
op_result_t dividir(int op1, int op2, int* result);
op_result_t potencia(int x, int n, int* result);
op_result_t logaritmo(int x, int b, int* result);
op_result_t raiz(int x, int* result);
op_result_t ternaria(int op1, int op2, int op3, int* result);

const entry_unaria_t OPERACIONES_UNARIAS[] = {
	{OP_RAIZ, raiz},
};

const entry_binaria_t OPERACIONES_BINARIAS[] = {
	{OP_SUMA, sumar},
	{OP_RESTA, restar},
	{OP_MULTIPLICAR, multiplicar},
	{OP_DIVISION, dividir},
	{OP_POTENCIA, potencia},
	{OP_LOGARITMO, logaritmo}
};

const entry_ternaria_t OPERACIONES_TERNARIAS[] = {
	{OP_TERNARIA, ternaria},
};

const entry_operador_t OPERADORES[] = {
	{"+", 2, OP_SUMA},
	{"-", 2, OP_RESTA},
	{"*", 2, OP_MULTIPLICAR},
	{"/", 2, OP_DIVISION},
	{"sqrt", 1, OP_RAIZ},
	{"log", 2, OP_LOGARITMO},
	{"^", 2, OP_POTENCIA},
	{"?", 3, OP_TERNARIA},
};

const int CANTIDAD_OPERACIONES_UNARIAS = 1;
const int CANTIDAD_OPERACIONES_BINARIAS = 6;
const int CANTIDAD_OPERACIONES_TERNARIAS = 1;
const int CANTIDAD_OPERADORES = 8;

/* Auxiliares */

char* left_trim(char* s)
{
	while (isspace(*s)) s++;
	return s;
}

char* right_trim(char* s)
{
	char* back = s + strlen(s);
	while (isspace(*--back));
	*(back + 1) = '\0';
	return s;
}

char* trim(char* s)
{
	return right_trim(left_trim(s));
}

/*
 * La verdad es que podria ahorrarme estas 3 haciendo un macro que genere funciones y
 * llamarlo con unaria binaria y ternaria, etc. Pero creo que complicaria mas el codigo
 */
operacion_unaria_t obtener_operacion_unaria(op_type_t type) {
	for (int i = 0; i < CANTIDAD_OPERACIONES_UNARIAS; ++i) {
		if (type == OPERACIONES_UNARIAS[i].tipo)
			return OPERACIONES_UNARIAS[i].op;
	}
	return NULL;
}

operacion_binaria_t obtener_operacion_binaria(op_type_t type) {
	for (int i = 0; i < CANTIDAD_OPERACIONES_BINARIAS; ++i) {
		if (type == OPERACIONES_BINARIAS[i].tipo)
			return OPERACIONES_BINARIAS[i].op;
	}
	return NULL;
}

operacion_ternaria_t obtener_operacion_ternaria(op_type_t type) {
	for (int i = 0; i < CANTIDAD_OPERACIONES_TERNARIAS; ++i) {
		if (type == OPERACIONES_TERNARIAS[i].tipo)
			return OPERACIONES_TERNARIAS[i].op;
	}
	return NULL;
}

/* Operaciones */

op_result_t sumar(int op1, int op2, int* result) {
	*result = op1 + op2;
	return OP_EXITOSA;
}

op_result_t restar(int op1, int op2, int* result) {
	*result = op1 - op2;
	return OP_EXITOSA;
}

op_result_t multiplicar(int op1, int op2, int* result) {
	*result = op1 * op2;
	return OP_EXITOSA;
}

op_result_t dividir(int op1, int op2, int* result) {
	if (op2 == 0) return OP_ERROR;
	*result = op1 / op2;
	return OP_EXITOSA;
}

int potencia_aux(int x, int n) {
	if (n == 0) return 1;
	int mult = 1;
	if (n % 2 != 0) mult = x;
	int p = potencia_aux(x, n / 2);
	return p * p * mult;
}

op_result_t potencia(int x, int n, int* result) {
	if (n < 0) return OP_ERROR;
	*result = potencia_aux(x, n);
	return OP_EXITOSA;
}

int logaritmo_aux(int x, int b) {
	return x > 1 ? 1 + logaritmo_aux(x / b, b) : 0;
}

op_result_t logaritmo(int x, int b, int* result) {
	if (x <= 1 || b < 1) return OP_ERROR;
	*result = logaritmo_aux(x, b);
	return OP_EXITOSA;
}

op_result_t raiz(int x, int* result) {
	if (x < 0) return OP_ERROR;
	int i = 0, j = 0;
	while (j < x) {
		i++;
		j = i * i;
	}
	*result = i-1;
	return OP_EXITOSA;
}

op_result_t ternaria(int op1, int op2, int op3, int* result) {
	*result = op1 == 0 ? op2 : op3;
	return OP_EXITOSA;
}

/* Lógica del programa */

bool es_operador(char* symbol, entry_operador_t* informacion) {
	for (int i = 0; i < CANTIDAD_OPERADORES; ++i) {
		(*informacion) = OPERADORES[i];
		if (strcmp(symbol, OPERADORES[i].nombre) == 0) {
			return true;
		}
	}
	return false;
}

bool es_numero(char* str) {
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] < '0' || str[i] > '9') return false;
	}
	return true;
}

bool obtener_op(int* op, pila_t* pila) {
	void* valor = pila_desapilar(pila);
	if (valor == NULL) return false;
	*op = *((int*)valor);
	free(valor);
	return true;
}

op_result_t ejecutar_operacion(entry_operador_t entry, pila_t* pila, int* result) {
	int op1, op2, op3;
	switch (entry.cantidad)
	{
	case 1:
		if (!obtener_op(&op1, pila)) return OP_ERROR;
		return obtener_operacion_unaria(entry.tipo)(op1, result);
	case 2:
		if (!obtener_op(&op1, pila)) return OP_ERROR;
		if (!obtener_op(&op2, pila)) return OP_ERROR;
		return obtener_operacion_binaria(entry.tipo)(op1, op2, result);
	case 3:
		if (!obtener_op(&op1, pila)) return OP_ERROR;
		if (!obtener_op(&op2, pila)) return OP_ERROR;
		if (!obtener_op(&op3, pila)) return OP_ERROR;
		return obtener_operacion_ternaria(entry.tipo)(op1, op2, op3, result);
	}
	return OP_ERROR;
}

bool guardar_numero(pila_t* pila, int n) {
	int* ptr = malloc(sizeof(int));
	if (ptr == NULL) return false;
	*ptr = n;
	return pila_apilar(pila, ptr);
}

void limpiar_destruir_pila(pila_t* pila) {
	while (!pila_esta_vacia(pila)) {
		free((int*)pila_desapilar(pila));
	}
	pila_destruir(pila);
}

op_result_t procesar_linea(char* linea, int* result) {
	char** symbols = split(linea, ' ');
	pila_t* pila = pila_crear();
	entry_operador_t entry;
	op_result_t estado = OP_EXITOSA;

	for (int i = 0; symbols[i] != NULL; ++i) {
		char* cleaned_symbol = trim(symbols[i]);
		if (es_operador(cleaned_symbol, &entry)) {
			op_result_t op_result = ejecutar_operacion(entry, pila, result);
			if (op_result == OP_ERROR) {
				estado = OP_ERROR;
				break;
			}
			if (!guardar_numero(pila, *result)) {
				estado = OP_ERROR;
				break;
			}
		}
		else {
			if (!es_numero(cleaned_symbol)) {
				estado = OP_ERROR;
				break;
			}
			int parsed = atoi(cleaned_symbol);
			if (!guardar_numero(pila, parsed)) {
				estado = OP_ERROR;
				break;
			}
		}
	}

	limpiar_destruir_pila(pila);
	free_strv(symbols);
	return estado;
}

int main(void) {
	char* linea = NULL;
	size_t n = 0;
	while (getline(&linea, &n, stdin) != -1) {
		int result = 0;
		op_result_t resultado = procesar_linea(linea, &result);
		if (resultado == OP_EXITOSA) printf("%d\n", result);
		else printf("ERROR\n");
	}
	free(linea);
	return 0;
}