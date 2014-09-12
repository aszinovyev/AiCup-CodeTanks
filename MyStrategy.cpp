#include <iostream>
#include "MyStrategy.h"

#include <math.h>
#include <cstdlib>

//#define M_PI 3.14159265358979323846
//#define _USE_MATH_DEFINES

using namespace std;

MyStrategy::MyStrategy() {
    _first = true;
}

void MyStrategy::gotoXY(double x, double y) {
    const double k = 1;

    double angle = _self.getAngleTo(x, y);
    _move.setTurn(angle);

    angle = fabs(angle);
    _move.setSpeedUp( pow((M_PI - angle) / M_PI, k) );
}

void MyStrategy::act() {
    if (_self.getTeammateIndex() == 1) {
        gotoXY(0, 0);
        _move.setAction(TAKE_PUCK);
        return;
    }

    if (_world.getPuck().getOwnerHockeyistId() == _self.getId()) {
        PlayerF opp = _world.getOpponentPlayer();

        double netX = opp.getNetFront();
        double netY = (opp.getNetBottom() + opp.getNetTop()) / 2;

        gotoXY(netX, netY);

        if (fabs(_self.getAngleTo(netX, netY)) < M_PI / 180) {
            _move.setAction(STRIKE);
        }
    } else {
        _move.setSpeedUp(1);
        _move.setTurn(_self.getAngleTo(_world.getPuck()));
        _move.setAction(TAKE_PUCK);
    }
}

void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {
    _game = GameF(game);

    if (_first) {
        _first = false;

        _fix = CoordFix(_game.getWorldWidth(), _game.getWorldHeight(),
                        world.getMyPlayer().getNetBack() < _game.getWorldWidth() / 2);
    }

    _self = HockeyistF(self, _fix);
    _world = WorldF(world, _fix, _game);
    _move = MoveF(&move, _fix);

    act();
}
