#include "polygon.h"
#include "geom.h"

bool Polygon::containsU(const UnitF &u) const {
    return contains(u.getX(), u.getY());
}

//

Circle::Circle(double x, double y, double r) {
    _x = x;
    _y = y;
    _r = r;
}

bool Circle::contains(double x, double y) const {
    return (Pif2(x - _x, y - _y) <= _r*_r);
}

double Circle::getX() const {
    return _x;
}

double Circle::getY() const {
    return _y;
}

double Circle::getR() const {
    return _r;
}

//

Rectangle::Rectangle(double x0, double x1, double y0, double y1) {
    _x0 = x0;
    _x1 = x1;
    _y0 = y0;
    _y1 = y1;
}

bool Rectangle::contains(double x, double y) const {
    return ( (x >= _x0) && (x <= _x1) && (y >= _y0) && (y <= _y1) );
}