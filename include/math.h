#ifndef MATH_H
#define MATH_H

#include "hardware/fpu.h"

#ifdef __cplusplus
extern "C" {
#endif

#define M_E         2.7182818284590452354	/* e */
#define M_LOG2E     1.4426950408889634074	/* log_2 e */
#define M_LOG10E	0.43429448190325182765	/* log_10 e */
#define M_LN2		0.69314718055994530942	/* log_e 2 */
#define M_LN10		2.30258509299404568402	/* log_e 10 */
#define M_PI		3.14159265358979323846	/* pi */
#define M_PI_2		1.57079632679489661923	/* pi/2 */
#define M_PI_4		0.78539816339744830962	/* pi/4 */
#define M_1_PI		0.31830988618379067154	/* 1/pi */
#define M_2_PI		0.63661977236758134308	/* 2/pi */
#define M_2_SQRTPI  1.12837916709551257390	/* 2/sqrt(pi) */
#define M_SQRT2     1.41421356237309504880	/* sqrt(2) */
#define M_SQRT1_2   0.70710678118654752440	/* 1/sqrt(2) */

#define abs(x)      fpu_abs(x)
#define atan(x)     fpu_atan(x)
#define atan2(y, x) fpu_atan2(y, x)
#define ceil(x)     fpu_ceil(x)
#define cos(x)      fpu_cos(x)
#define floor(x)    fpu_floor(x)
#define mod(x, y)   fpu_modulo(x, y)
#define pow(x, y)   fpu_pow(x, y)
#define sin(x)      fpu_sin(x)
#define sqrt(x)     fpu_sqrt(x)
#define tan(x)      fpu_tan(x)
#define trunc(x)    fpu_trunc(x)

#ifdef __cplusplus
}
#endif

#endif // MATH_H