//#include <iostream> //

#include "geom.h"

double isEqual(double a, double b, double eps) {
    return (fabs(a - b) < eps);
}

double NormAngle(double a) {
    while (a < -M_PI) {
        a += M_PI * 2;
    }
    while (a > M_PI) {
        a -= M_PI * 2;
    }

    return a;
}

double Pif2(double a, double b) {
    return a*a + b*b;
}

double Pif(double a, double b) {
    return sqrt(Pif2(a, b));
}

double ctg(double a) {
    return 1 / tan(a);
}

double vectorMul(double x1, double y1, double x2, double y2) {
    return x1 * y2 - x2 * y1;
}

double orientedS(double x1, double y1, double x2, double y2, double x3, double y3) {
    return vectorMul(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
}
