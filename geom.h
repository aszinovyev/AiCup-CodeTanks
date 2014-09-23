#ifndef GEOM_H
#define GEOM_H

#define M_PI 3.14159265358979323846

#include <math.h>

const int Inf = 1234567890;

double isEqual(double a, double b, double eps = 10e-6);

double NormAngle(double a);

double Pif2(double a, double b);
double Pif(double a, double b);

double ctg(double a);

double vectorMul(double x1, double y1, double x2, double y2);
double scalarMul(double x1, double y1, double x2, double y2);
double orientedS(double x1, double y1, double x2, double y2, double x3, double y3);

bool intersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4,
                  double& x, double& y);

double height(double x, double y, double x1, double y1, double x2, double y2);

double sqr(double a);

#endif // GEOM_H
