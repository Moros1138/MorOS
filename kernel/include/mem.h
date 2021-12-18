#ifndef MEM_H
#define MEM_H

#include "multiboot.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void memory_init(multiboot_info_t* mbd);
void print_memory_blocks();

#ifdef __cplusplus
}
#endif

#endif