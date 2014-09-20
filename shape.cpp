#include <iostream>
#include <cassert>

#include "shape.h"
#include "geom.h"

using namespace std;

bool Shape::containsU(const UnitF &u) const {
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

Sector::Sector(double x, double y, double r, double a1, double a2) {
    _x = x;
    _y = y;
    _r = r;
    _a1 = a1;
    _a2 = a2;
}

bool Sector::contains(double x, double y) const {
    double rSqr = Pif2(_x - x, _y - y);
    double a = atan2(y - _y, x - _x);

    return ( (rSqr <= _r*_r) && (a >= _a1) && (a <= _a2) );
}

//

ShapeSubtraction::ShapeSubtraction() {
    _s1 = 0;
    _s2 = 0;
}

ShapeSubtraction::~ShapeSubtraction() {
    delete _s1;
    delete _s2;

    _s1 = 0;
    _s2 = 0;
}

void ShapeSubtraction::set(Shape* s1, Shape* s2) {
    delete _s1;
    delete _s2;

    _s1 = s1;
    _s2 = s2;
}

bool ShapeSubtraction::contains(double x, double y) const {
    return (_s1->contains(x, y) && !_s2->contains(x, y));
}

//

ShapeInvX::ShapeInvX(const Shape& s, const double width) : _s(s), _width(width) {
}

bool ShapeInvX::contains(double x, double y) const {
    return _s.contains(_width - x, y);
}

ShapeInvY::ShapeInvY(const Shape& s, const double height) : _s(s), _height(height) {
}

bool ShapeInvY::contains(double x, double y) const {
    return _s.contains(x, _height - y);
}
