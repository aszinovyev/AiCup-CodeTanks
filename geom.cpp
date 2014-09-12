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
