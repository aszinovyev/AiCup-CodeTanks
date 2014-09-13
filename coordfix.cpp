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
    _invY = false;
}

void CoordFix::setInvY(bool invY) {
    _invY = invY;
}

bool CoordFix::isInvY() const {
    return _invY;
}

double CoordFix::invcX(double x) const {
    return _w - x;
}

double CoordFix::invcY(double y) const {
    return _h - y;
}

double CoordFix::invcAngle(double angle) const {
    return NormAngle(angle + M_PI);
}

double CoordFix::invcDx(double dx) const {
    return -dx;
}

double CoordFix::invcDy(double dy) const {
    return -dy;
}

double CoordFix::invyAngle(double angle) const {
    return -angle;
}

double CoordFix::fixX(double x) const {
    if (_isOnLeft) {
        return x;
    } else {
        return invcX(x);
    }
}

double CoordFix::fixY(double y) const {
    if (_isOnLeft != _invY) {
        return y;
    } else {
        return invcY(y);
    }
}

double CoordFix::fixAngle(double angle) const {
    if (!_isOnLeft) {
        angle = invcAngle(angle);
    }

    if (_invY) {
        angle = invyAngle(angle);
    }

    return angle;
}

double CoordFix::fixDx(double dx) const {
    if (_isOnLeft) {
        return dx;
    } else {
        return invcDx(dx);
    }
}

double CoordFix::fixDy(double dy) const {
    if (_isOnLeft != _invY) {
        return dy;
    } else {
        return invcDy(dy);
    }
}

//

MoveF::MoveF() {
    _move = 0;
}

MoveF::MoveF(Move* m, const CoordFix* fix) : _fix(fix) {
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

//

GameF::GameF() {
}

GameF::GameF(const Game& g) : Game(g) {
}

double GameF::getWorldWidth() const {
    return getRinkLeft() + getRinkRight();
}

double GameF::getWorldHeight() const {
    return getRinkTop() + getRinkBottom();
}

//

PlayerF::PlayerF(const Player& p, const CoordFix* fix) : Player(p), _fix(fix) {
}

double PlayerF::getNetLeft() const {
    return min(getNetFront(), getNetBack());
}

double PlayerF::getNetRight() const {
    return max(getNetFront(), getNetBack());
}

double PlayerF::getNetFront() const {
    return _fix->fixX(Player::getNetFront());
}

double PlayerF::getNetBack() const {
    return _fix->fixX(Player::getNetBack());
}

//

UnitF::UnitF(const Unit& u, const CoordFix* fix) : Unit(u), _fix(fix) {
}

double UnitF::getX() const {
    return _fix->fixX(Unit::getX());
}

double UnitF::getY() const {
    return _fix->fixY(Unit::getY());
}

double UnitF::getSpeedX() const {
    return _fix->fixDx(Unit::getSpeedX());
}

double UnitF::getSpeedY() const {
    return _fix->fixDy(Unit::getSpeedY());
}

double UnitF::getAngle() const {
    return _fix->fixAngle(Unit::getAngle());
}

double UnitF::getAngleTo(double x, double y) const {
    return Unit::getAngleTo(_fix->fixX(x), _fix->fixY(y));
}

double UnitF::getAngleTo(const Unit& unit) const {
    return Unit::getAngleTo(unit);
}

double UnitF::getDistanceTo(double x, double y) const {
    return Unit::getDistanceTo(_fix->fixX(x), _fix->fixY(y));
}

double UnitF::getDistanceTo(const Unit& unit) const {
    return Unit::getDistanceTo(unit);
}

//

HockeyistF::HockeyistF() {
}

HockeyistF::HockeyistF(const Hockeyist& h, const CoordFix* fix) : Hockeyist(h), _fix(fix) {
}

double HockeyistF::getX() const {
    return _fix->fixX(Hockeyist::getX());
}

double HockeyistF::getY() const {
    return _fix->fixY(Hockeyist::getY());
}

double HockeyistF::getSpeedX() const {
    return _fix->fixDx(Hockeyist::getSpeedX());
}

double HockeyistF::getSpeedY() const {
    return _fix->fixDy(Hockeyist::getSpeedY());
}

double HockeyistF::getAngle() const {
    return _fix->fixAngle(Hockeyist::getAngle());
}

double HockeyistF::getAngleTo(double x, double y) const {
    return Hockeyist::getAngleTo(_fix->fixX(x), _fix->fixY(y));
}

double HockeyistF::getAngleTo(const Unit& unit) const {
    return Hockeyist::getAngleTo(unit);
}

double HockeyistF::getDistanceTo(double x, double y) const {
    return Hockeyist::getDistanceTo(_fix->fixX(x), _fix->fixY(y));
}

double HockeyistF::getDistanceTo(const Unit& unit) const {
    return Hockeyist::getDistanceTo(unit);
}

//

PuckF::PuckF(const Puck& p, const CoordFix* fix) : Puck(p), _fix(fix) {
}

double PuckF::getX() const {
    return _fix->fixX(Puck::getX());
}

double PuckF::getY() const {
    return _fix->fixY(Puck::getY());
}

double PuckF::getSpeedX() const {
    return _fix->fixDx(Puck::getSpeedX());
}

double PuckF::getSpeedY() const {
    return _fix->fixDy(Puck::getSpeedY());
}

double PuckF::getAngle() const {
    return _fix->fixAngle(Puck::getAngle());
}

double PuckF::getAngleTo(double x, double y) const {
    return Puck::getAngleTo(_fix->fixX(x), _fix->fixY(y));
}

double PuckF::getAngleTo(const Unit& unit) const {
    return Puck::getAngleTo(unit);
}

double PuckF::getDistanceTo(double x, double y) const {
    return Puck::getDistanceTo(_fix->fixX(x), _fix->fixY(y));
}

double PuckF::getDistanceTo(const Unit& unit) const {
    return Puck::getDistanceTo(unit);
}

//

WorldF::WorldF() {
}

WorldF::WorldF(const World &w, const CoordFix* fix, const GameF& game) : World(w), _fix(fix) {
    _game = game;
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

PuckF WorldF::getPuck() const {
    return PuckF(World::getPuck(), _fix);
}

double WorldF::getWidth() const {
    return _game.getWorldWidth();
}

double WorldF::getHeight() const {
    return _game.getWorldHeight();
}
