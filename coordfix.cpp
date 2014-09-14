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

HockeyistF::HockeyistF() : UnitF(Hockeyist(), 0) {
}

HockeyistF::HockeyistF(const Hockeyist& h, const CoordFix* fix) : Hockeyist(h), UnitF(h, fix), _fix(fix) {
}

long long HockeyistF::getId() const {
    return UnitF::getId();
}

double HockeyistF::getMass() const {
    return UnitF::getMass();
}

double HockeyistF::getRadius() const {
    return UnitF::getRadius();
}

double HockeyistF::getX() const {
    return UnitF::getX();
}

double HockeyistF::getY() const {
    return UnitF::getY();
}

double HockeyistF::getSpeedX() const {
    return UnitF::getSpeedX();
}

double HockeyistF::getSpeedY() const {
    return UnitF::getSpeedY();
}

double HockeyistF::getAngle() const {
    return UnitF::getAngle();
}

double HockeyistF::getAngularSpeed() const {
    return UnitF::getAngularSpeed();
}

double HockeyistF::getAngleTo(double x, double y) const {
    return UnitF::getAngleTo(x, y);
}

double HockeyistF::getAngleTo(const Unit& unit) const {
    return UnitF::getAngleTo(unit);
}

double HockeyistF::getDistanceTo(double x, double y) const {
    return UnitF::getDistanceTo(x, y);
}

double HockeyistF::getDistanceTo(const Unit& unit) const {
    return UnitF::getDistanceTo(unit);
}

//

PuckF::PuckF(const Puck& p, const CoordFix* fix) : Puck(p), UnitF(p, fix) {
}

long long PuckF::getId() const {
    return UnitF::getId();
}

double PuckF::getMass() const {
    return UnitF::getMass();
}

double PuckF::getRadius() const {
    return UnitF::getRadius();
}

double PuckF::getX() const {
    return UnitF::getX();
}

double PuckF::getY() const {
    return UnitF::getY();
}

double PuckF::getSpeedX() const {
    return UnitF::getSpeedX();
}

double PuckF::getSpeedY() const {
    return UnitF::getSpeedY();
}

double PuckF::getAngle() const {
    return UnitF::getAngle();
}

double PuckF::getAngularSpeed() const {
    return UnitF::getAngularSpeed();
}

double PuckF::getAngleTo(double x, double y) const {
    return UnitF::getAngleTo(x, y);
}

double PuckF::getAngleTo(const Unit& unit) const {
    return UnitF::getAngleTo(unit);
}

double PuckF::getDistanceTo(double x, double y) const {
    return UnitF::getDistanceTo(x, y);
}

double PuckF::getDistanceTo(const Unit& unit) const {
    return UnitF::getDistanceTo(unit);
}

//

WorldF::WorldF() {
}

WorldF::WorldF(const World &w, const CoordFix* fix, const GameF& game) : World(w), _fix(fix) {
    _game = game;
}

vector<PlayerF> WorldF::getPlayers() const {
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

vector<HockeyistF> WorldF::getHockeyists() const {
    const vector<Hockeyist>& in = World::getHockeyists();

    vector<HockeyistF> out;
    for (vector<Hockeyist>::const_iterator it = in.cbegin(); it != in.cend(); ++it) {
        out.push_back(HockeyistF(*it, _fix));
    }

    return out;
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
