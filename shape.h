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

    virtual bool contains(double x, double y) const;

private:
    double _x0;
    double _x1;
    double _y0;
    double _y1;
};

class Polygon : public Shape {
public:
    Polygon(vector<double> x, vector<double> y);

    virtual bool contains(double x, double y) const;

private:
    vector<double> _x;
    vector<double> _y;
};

//

class Sector : public Shape {
public:
    Sector() {}
    Sector(double x, double y, double r, double a1, double a2);

    virtual bool contains(double x, double y) const;

private:
    double _x;
    double _y;
    double _r;
    double _a1;
    double _a2;
};

//

class ShapeSubtraction : public Shape {
public:
    ShapeSubtraction();
    virtual ~ShapeSubtraction();

    void set(Shape* s1, Shape* s2);

    virtual bool contains(double x, double y) const;

private:
    ShapeSubtraction(const ShapeSubtraction& s) {
        (void)s;
    }
    ShapeSubtraction& operator=(const ShapeSubtraction& s) {
        (void)s;
        return *this;
    }

    Shape* _s1;
    Shape* _s2;
};

#endif // POLYGON_H
