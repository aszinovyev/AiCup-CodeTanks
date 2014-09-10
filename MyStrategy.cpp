#include <iostream>
#include "MyStrategy.h"

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

MyStrategy::MyStrategy() { }

void MyStrategy::gotoXY(double x, double y) {
    const double k = 1;

    double angle = _self.getAngleTo(x, y);
    _move->setTurn(_self.getAngleTo(x, y));

    angle = fabs(angle);
    _move->setSpeedUp( pow((M_PI - angle) / M_PI, k) );
}

void MyStrategy::act() {
    if (_self.getTeammateIndex() == 1) {
        gotoXY(0, 0);
        _move->setAction(TAKE_PUCK);
        return;
    }

    if (_world.getPuck().getOwnerHockeyistId() == _self.getId()) {
        Player opp = _world.getOpponentPlayer();

        double netX = opp.getNetFront();
        double netY = (opp.getNetBottom() + opp.getNetTop()) / 2;

        gotoXY(netX, netY);

        if (fabs(_self.getAngleTo(netX, netY)) < M_PI / 180) {
            _move->setAction(STRIKE);
        }
    } else {
        _move->setSpeedUp(1);
        _move->setTurn(_self.getAngleTo(_world.getPuck()));
        _move->setAction(TAKE_PUCK);
    }
}

void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {
    _self = self;
    _world = world;
    _game = game;
    _move = &move;

    act();
}
