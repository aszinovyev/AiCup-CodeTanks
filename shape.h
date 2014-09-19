#ifndef POLYGON_H
#define POLYGON_H

#include "coordfix.h"

class Shape {
public:
    virtual ~Shape() {}

    virtual bool contains(double x, double y) const = 0;
    virtual bool containsU(const UnitF& u) const;
};

//

class Circle : public Shape {
public:
    Circle() {}
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

class Rectangle : public Shape {
public:
    Rectangle() {}
    Rectangle(double x0, double x1, double y0, double y1);

    bool contains(double x, double y) const;

private:
    double _x0;
    double _x1;
    double _y0;
    double _y1;
};

class Polygon : public Shape {
public:
    Polygon(vector<double> x, vector<double> y);

    bool contains(double x, double y) const;

private:
    vector<double> _x;
    vector<double> _y;
};

//

class Sector : public Shape {
public:
    Sector() {}
    Sector(double x, double y, double r, double a1, double a2);

    bool contains(double x, double y) const;

private:
    double _x;
    double _y;
    double _r;
    double _a1;
    double _a2;
};

//

template<class T1, class T2>
class ShapeSubtraction : public Shape {
public:
    ShapeSubtraction() {}
    ShapeSubtraction(const T1& s1, const T2& s2) {
        _s1 = s1;
        _s2 = s2;
    }

    bool contains(double x, double y) const {
        return (_s1.contains(x, y) && !_s2.contains(x, y));
    }

private:
    T1 _s1;
    T2 _s2;
};

#endif // POLYGON_H
