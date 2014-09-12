#include <math.h>
//#include <iostream>

#include "coordfix.h"
#include "geom.h"

CoordFix::CoordFix() {
}

CoordFix::CoordFix(double width, double height, bool isOnLeft) {
    _w = width;
    _h = height;
    _isOnLeft = isOnLeft;
}

double CoordFix::invX(double x) const {
    return _w - x;
}

double CoordFix::invY(double y) const {
    return _h - y;
}

double CoordFix::invAngle(double angle) const {
    return NormAngle(angle + M_PI);
}

double CoordFix::invDx(double dx) const {
    return -dx;
}

double CoordFix::invDy(double dy) const {
    return -dy;
}

double CoordFix::fixX(double x) const {
    if (_isOnLeft) {
        return x;
    } else {
        return invX(x);
    }
}

double CoordFix::fixY(double y) const {
    if (_isOnLeft) {
        return y;
    } else {
        return invY(y);
    }
}

double CoordFix::fixAngle(double angle) const {
    if (_isOnLeft) {
        return angle;
    } else {
        return invAngle(angle);
    }
}

double CoordFix::fixDx(double dx) const {
    if (_isOnLeft) {
        return dx;
    } else {
        return invDx(dx);
    }
}

double CoordFix::fixDy(double dy) const {
    if (_isOnLeft) {
        return dy;
    } else {
        return invDy(dy);
    }
}

//

MoveF::MoveF() {
    _move = 0;
}

MoveF::MoveF(Move* m, const CoordFix& fix) {
    _move = m;
    _fix = fix;
}

double MoveF::getSpeedUp() const {
    return _move->getSpeedUp();
}

void MoveF::setSpeedUp(const double speedUp) {
    _move->setSpeedUp(speedUp);
}

double MoveF::getTurn() const {
    return _fix.fixAngle(_move->getTurn());
}

void MoveF::setTurn(const double turn) {
    return _move->setTurn(_fix.fixAngle(turn));
}

ActionType MoveF::getAction() const {
    return _move->getAction();
}

void MoveF::setAction(const ActionType action) {
    _move->setAction(action);
}

double MoveF::getPassPower() const {
    return _move->getPassPower();
}

void MoveF::setPassPower(const double passPower) {
    _move->setPassPower(passPower);
}

double MoveF::getPassAngle() const {
    return _fix.fixAngle(_move->getPassAngle());
}

void MoveF::setPassAngle(const double passAngle) {
    _move->setPassAngle(_fix.fixAngle(passAngle));
}

int MoveF::getTeammateIndex() const {
    return _move->getTeammateIndex();
}

void MoveF::setTeammateIndex(const int teammateIndex) {
    _move->setTeammateIndex(teammateIndex);
}

//

PlayerF::PlayerF(const Player& p, const CoordFix& fix) : Player(p) {
    _fix = fix;
}

double PlayerF::getNetLeft() const {
    return min(getNetFront(), getNetBack());
}

double PlayerF::getNetRight() const {
    return max(getNetFront(), getNetBack());
}

double PlayerF::getNetFront() const {
    return _fix.fixX(Player::getNetFront());
}

double PlayerF::getNetBack() const {
    return _fix.fixX(Player::getNetBack());
}

//

UnitF::UnitF(const Unit& u, const CoordFix& fix) : Unit(u) {
    _fix = fix;
}

double UnitF::getX() const {
    return _fix.fixX(Unit::getX());
}

double UnitF::getY() const {
    return _fix.fixY(Unit::getY());
}

double UnitF::getSpeedX() const {
    return _fix.fixDx(Unit::getSpeedX());
}

double UnitF::getSpeedY() const {
    return _fix.fixDy(Unit::getSpeedY());
}

double UnitF::getAngle() const {
    return _fix.fixAngle(Unit::getAngle());
}

double UnitF::getAngleTo(double x, double y) const {
    return _fix.fixAngle( Unit::getAngleTo(_fix.fixX(x), _fix.fixY(y)) );
}

double UnitF::getDistanceTo(double x, double y) const {
    return Unit::getDistanceTo(_fix.fixX(x), _fix.fixY(y));
}

//

HockeyistF::HockeyistF() {
}

HockeyistF::HockeyistF(const Hockeyist& h, const CoordFix& fix) : Hockeyist(h) {
    _fix = fix;
}

double HockeyistF::getX() const {
    return _fix.fixX(HockeyistF::getX());
}

double HockeyistF::getY() const {
    return _fix.fixY(HockeyistF::getY());
}

double HockeyistF::getSpeedX() const {
    return _fix.fixDx(HockeyistF::getSpeedX());
}

double HockeyistF::getSpeedY() const {
    return _fix.fixDy(HockeyistF::getSpeedY());
}

double HockeyistF::getAngle() const {
    return _fix.fixAngle(HockeyistF::getAngle());
}

double HockeyistF::getAngleTo(double x, double y) const {
    return _fix.fixAngle( HockeyistF::getAngleTo(_fix.fixX(x), _fix.fixY(y)) );
}

double HockeyistF::getDistanceTo(double x, double y) const {
    return HockeyistF::getDistanceTo(_fix.fixX(x), _fix.fixY(y));
}

//

PuckF::PuckF(const Puck& p, const CoordFix& fix) : Puck(p) {
    _fix = fix;
}

double PuckF::getX() const {
    return _fix.fixX(PuckF::getX());
}

double PuckF::getY() const {
    return _fix.fixY(PuckF::getY());
}

double PuckF::getSpeedX() const {
    return _fix.fixDx(PuckF::getSpeedX());
}

double PuckF::getSpeedY() const {
    return _fix.fixDy(PuckF::getSpeedY());
}

double PuckF::getAngle() const {
    return _fix.fixAngle(PuckF::getAngle());
}

double PuckF::getAngleTo(double x, double y) const {
    return _fix.fixAngle( PuckF::getAngleTo(_fix.fixX(x), _fix.fixY(y)) );
}

double PuckF::getDistanceTo(double x, double y) const {
    return PuckF::getDistanceTo(_fix.fixX(x), _fix.fixY(y));
}

//

WorldF::WorldF() {
}

WorldF::WorldF(const World &w, const CoordFix& fix) : World(w) {
    _fix = fix;
}

const vector<PlayerF> WorldF::getPlayers() const {
    vector<PlayerF> res;
    res.push_back(getMyPlayer());
    res.push_back(getOpponentPlayer());

    return res;
}

PlayerF WorldF::getMyPlayer() const {
    return PlayerF(World::getMyPlayer(), _fix);
}

PlayerF WorldF::getOpponentPlayer() const {
    return PlayerF(World::getOpponentPlayer(), _fix);
}

const PuckF WorldF::getPuck() const {
    return PuckF(World::getPuck(), _fix);
}
