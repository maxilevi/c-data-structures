#ifndef __VALIDADOR_H__
#define __VALIDADOR_H__

#include <stdbool.h>
#include "comando.h"

comando_t validar_comando(int argc, char** argv);

#endif