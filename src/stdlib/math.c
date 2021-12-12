#include "math.h"

double fabs(double x)
{
    return x < 0.0 ? -x : x;
}

float fabsf( float x )
{
    return (float)fabs(x);
}

double fmod(double x, double m)
{
    double result;
    
    __asm__(
        "1: fprem\n\t"
        "fnstsw %%ax\n\t"
        "sahf\n\t"
        "jp 1b"
        : "=t"(result) : "0"(x), "u"(m) : "ax", "cc"
    );

    return result;
}

double sin(double x)
{
    double result;
    
    __asm__("fsin" : "=t"(result) : "0"(x));
    
    return result;
}

double cos(double x)
{
    return sin(x + PI / 2.0);
}

// black magic
double pow(double x, double y)
{
    double out;

    __asm__(
        "fyl2x;"
        "fld %%st;"
        "frndint;"
        "fsub %%st,%%st(1);"
        "fxch;"
        "fchs;"
        "f2xm1;"
        "fld1;"
        "faddp;"
        "fxch;"
        "fld1;"
        "fscale;"
        "fstp %%st(1);"
        "fmulp;" : "=t"(out) : "0"(x),"u"(y) : "st(1)"
    );
    
    return out;
}

double sqrt(double x)
{
    return pow(x, 0.5f);
}
