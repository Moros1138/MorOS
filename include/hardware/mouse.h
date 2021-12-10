#ifndef MOUSE_H
#define MOUSE_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOUSE_LEFT      0
#define MOUSE_MIDDLE    2
#define MOUSE_RIGHT     1
#define MOUSE_EXTRA1    3
#define MOUSE_EXTRA2    4

void mouse_init();

#ifdef __cplusplus
}
#endif

#endif // MOUSE_H