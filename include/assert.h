#ifndef ASSERT_H
#define ASSERT_H

#include "stdlib.h"
#define assert(expr) if(!(expr)) { printf("assert(%s) has failed in %s - line %d\n",#expr, __FILE__, __LINE__); while(1); }

#endif // ASSERT_H
