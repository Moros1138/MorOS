#ifndef FPU_H
#define FPU_H

#ifdef __cplusplus
extern "C" {
#endif

#define FPU_TEST(func, x) printf("%s(%lf) = %lf%c", #func, x, func(x), 10)
#define FPU_TEST2(func, x, y) printf("%s(%lf, %lf) = %lf%c", #func, x, y, func(x, y), 10)

int fpu_init();

double fpu_abs(double x);
double fpu_atan(double x);
double fpu_atan2(double y, double x);
double fpu_ceil(double x);
double fpu_cos(double x);
double fpu_floor(double x);
double fpu_modulo(double x, double y);
double fpu_pow(double x, double y);
double fpu_sin(double x);
double fpu_sqrt(double x);
double fpu_tan(double x);
double fpu_trunc(double x);

#ifdef __cplusplus
}
#endif

#endif // FPU_H
