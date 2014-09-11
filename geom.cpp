#include <math.h>
#include "geom.h"

double NormAngle(double a) {
    while (a < -M_PI) {
        a += M_PI;
    }
    while (a > M_PI) {
        a -= M_PI;
    }

    return a;
}
