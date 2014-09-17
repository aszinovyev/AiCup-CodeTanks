#include <cassert>

#include "polygon.h"
#include "geom.h"

bool IPolygon::containsU(const UnitF &u) const {
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

//

Polygon::Polygon(vector<double> x, vector<double> y) {
    assert(x.size() > 2);
    assert(x.size() == y.size());

    _x = x;
    _y = y;
}

bool Polygon::contains(double x, double y) const {
    bool sign = (orientedS(_x[_x.size() - 1], _y[_y.size() - 1], _x[0], _y[0], x, y) > 0);

    for (unsigned int i = 0; i < _x.size() - 1; ++i) {
        if ( (orientedS(_x[i], _y[i], _x[i + 1], _y[i + 1], x, y) > 0) != sign ) {
            return false;
        }
    }

    return true;
}

//

SectorX::SectorX(double x, double y, double rs, double rb, double a1, double a2) {
    _x = x;
    _y = y;
    _rb = rb;
    _rs = rs;
    _a1 = a1;
    _a2 = a2;
}

bool SectorX::contains(double x, double y) const {
    double rSqr = Pif2(_x - x, _y - y);
    double a = atan2(y - _y, x - _x);

    return ( (rSqr > _rs*_rs) && (rSqr < _rb*_rb) && (a > _a1) && (a < _a2) );
}
