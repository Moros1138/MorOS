#ifndef _MOROS_UTILS_H
#define _MOROS_UTILS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <Terminal.h>

namespace MorOS
{
    
    void itoa(char* buf, int base, int value);

} // MorOS

#endif // _MOROS_UTILS_H