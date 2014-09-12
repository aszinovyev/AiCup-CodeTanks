//#include <iostream> //

#include <math.h>
#include "geom.h"

double NormAngle(double a) {
    //std::cout << "norm " << a << " ";

    while (a < -M_PI) {
        a += M_PI * 2;
    }
    while (a > M_PI) {
        a -= M_PI * 2;
    }

    //std::cout << a << std::endl;

    return a;
}
