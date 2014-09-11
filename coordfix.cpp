#include <math.h>

#include "coordfix.h"
#include "geom.h"

CoordFix::CoordFix() {
}

CoordFix::CoordFix(double width, double height, bool isOnLeft) {
    _w = width;
    _h = height;
    _isOnLeft = isOnLeft;
}

double CoordFix::invX(double x) {
    return _w - x;
}

double CoordFix::invY(double y) {
    return _h - y;
}

double CoordFix::invAngle(double angle) {
    return NormAngle(angle + M_PI);
}

double CoordFix::fixX(double x) {
    if (_isOnLeft) {
        return x;
    } else {
        return invX(x);
    }
}

double CoordFix::fixY(double y) {
    if (_isOnLeft) {
        return y;
    } else {
        return invY(y);
    }
}

double CoordFix::fixAngle(double angle) {
    if (_isOnLeft) {
        return angle;
    } else {
        return invAngle(angle);
    }
}

//

HockeyistF::HockeyistF() {
}

HockeyistF::HockeyistF(const model::Hockeyist& h) : model::Hockeyist(h) {
}

//

WorldF::WorldF() {
}

WorldF::WorldF(const model::World &w) : model::World(w) {
}

//

GameF::GameF() {
}

GameF::GameF(const model::Game& g) : model::Game(g) {
}

//

MoveF::MoveF() {
    _move = 0;
}

MoveF::MoveF(model::Move* m) {
    _move = m;
}

double MoveF::getSpeedUp() const {
    return _move->getSpeedUp();
}

void MoveF::setSpeedUp(const double speedUp) {
    _move->setSpeedUp(speedUp);
}

double MoveF::getTurn() const {
    return _move->getTurn();
}

void MoveF::setTurn(const double turn) {
    return _move->setTurn(turn);
}

model::ActionType MoveF::getAction() const {
    return _move->getAction();
}

void MoveF::setAction(const model::ActionType action) {
    _move->setAction(action);
}

double MoveF::getPassPower() const {
    return _move->getPassPower();
}

void MoveF::setPassPower(const double passPower) {
    _move->setPassPower(passPower);
}

double MoveF::getPassAngle() const {
    return _move->getPassAngle();
}

void MoveF::setPassAngle(const double passAngle) {
    _move->setPassAngle(passAngle);
}

int MoveF::getTeammateIndex() const {
    return _move->getTeammateIndex();
}

void MoveF::setTeammateIndex(const int teammateIndex) {
    _move->setTeammateIndex(teammateIndex);
}
