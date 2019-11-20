#ifndef __VALIDADOR_H__
#define __VALIDADOR_H__

#include <stdbool.h>
#include <stdlib.h>
#include "comando.h"

comando_t validar_comando(size_t argc, char** argv);

#endif