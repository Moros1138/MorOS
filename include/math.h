#ifndef MATH_H
#define MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#define E  2.71828
#define PI 3.14159265358979323846264338327950

double fabs(double x);
float fabsf(float x);
double fmod(double x, double m);
double sin(double x);
double cos(double x);
double sqrt(double x);
double pow(double x, double y);

#ifdef __cplusplus
}
#endif

#endif // MATH_H