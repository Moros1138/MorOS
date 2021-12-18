#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void timer_init();
uint32_t timer_get_time_point();

#ifdef __cplusplus
}
#endif

#endif