#include "strutil.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

void assert(const char* expected, char* value, const char* mensaje, void (*destructor)(void*)) {
	if (strcmp(value, expected) != 0) {
		printf("Prueba '%s' fallo al comparar el valor '%s' == '%s'\n", mensaje, value, expected);
	}
	else {
		printf("Prueba '%s' que compara '%s' == '%s' fue existosa\n", mensaje, value, expected);
	}
	fflush(stdout);
	if(destructor)
		destructor((void*)value);
}

void probar_substr() {
	char* cadena = "PRINCIPIO DE INDUCCION COMPLETO";
	assert("PRINCIPIO", substr(cadena, 9), "substr", free);
	assert("INDUCCION", substr(cadena + 13, 9), "substr con offset", free);
}

void probar_split() {
	char* expected[] = { "PRINCIPIO", "DE", "INDUCCION", "COMPLETO"};
	char* cadena = "PRINCIPIO DE INDUCCION COMPLETO";

	char** separados = split(cadena, ' ');
	for (int i = 0; i < 4; ++i) {
		assert(expected[i], separados[i], "split", NULL);
	}

	free_strv(separados);
}

void probar_join() {
	char* input[] = { "PRINCIPIO", "DE", "INDUCCION", "COMPLETO", NULL};

	assert("PRINCIPIO DE INDUCCION COMPLETO", join(input, ' '), "join", free);
	assert("PRINCIPIODEINDUCCIONCOMPLETO", join(input, '\0'), "join vacio", free);
}

int main(void) {
	probar_substr();
	probar_split();
	probar_join();
}

