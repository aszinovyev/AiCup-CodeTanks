#ifndef POLYGON_H
#define POLYGON_H

#include "coordfix.h"

class Polygon {
public:
    virtual ~Polygon() {}

    virtual bool contains(double x, double y) const = 0;
    virtual bool containsU(const UnitF& u) const;
};

//

class Circle : public Polygon {
public:
    Circle(double x, double y, double r);

    virtual bool contains(double x, double y) const;

    double getX() const;
    double getY() const;
    double getR() const;

private:
    double _x;
    double _y;
    double _r;
};

//

class Rectangle : public Polygon {
public:
    Rectangle(double x0, double x1, double y0, double y1);

    bool contains(double x, double y) const;

private:
    double _x0;
    double _x1;
    double _y0;
    double _y1;
};

#endif // POLYGON_H
