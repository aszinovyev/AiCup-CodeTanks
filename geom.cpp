//#include <iostream> //

#include <math.h>
#include "geom.h"

double NormAngle(double a) {
    while (a < -M_PI) {
        a += M_PI * 2;
    }
    while (a > M_PI) {
        a -= M_PI * 2;
    }

    return a;
}

double Pif(double a, double b) {
    return sqrt(a*a + b*b);
}

double ctg(double a) {
    return 1 / tan(a);
}
