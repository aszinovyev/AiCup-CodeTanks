#include <iostream>
#include <math.h>

#include "MyStrategy.h"
#include "geom.h"

using namespace std;

MyStrategy::MyStrategy() {
    _first = true;
}

void MyStrategy::move(const Hockeyist& self, const World& world, const Game& game, Move& move) {
    _game = GameF(game);

    if (_first) {
        _first = false;

        _fix = CoordFix(_game.getWorldWidth(), _game.getWorldHeight(),
                        world.getMyPlayer().getNetBack() < _game.getWorldWidth() / 2);
    }

    _fix.setInvY(false);

    _self = HockeyistF(self, &_fix);
    _world = WorldF(world, &_fix, _game);
    _move = MoveF(&move, &_fix);

    act();
}

//

double MyStrategy::conv0(double x, double z) {
    return (x * M_PI / z / 2);
}

double MyStrategy::conv1(double x, double z) {
    return M_PI / 2 * (x + M_PI - 2 * z) / (M_PI - z);
}

void MyStrategy::gotoXY(double x, double y) {
    const double z = M_PI * 2 / 3;
    const double m = 6;
    const double n = -4;
    const double alpha = M_PI / 18;
    const double beta = M_PI - alpha;

    double angle = _self.getAngleTo(x, y);
    _move.setTurn(angle);

    angle = fabs(angle);

    double realspeed = Pif(_self.getSpeedX(), _self.getSpeedY());
    _move.setSpeedUp(0);

    if (angle <= z) {
        double maxspeed = ctg(conv0(angle, z)) * m / ctg(conv0(alpha, z));
        if (maxspeed > realspeed ) {
            _move.setSpeedUp(1);
        }
    } else {
        double minspeed = ctg(conv1(angle, z)) * n / ctg(conv1(beta, z));
        if (minspeed < n) {
            _move.setSpeedUp(-1);
        }
    }
}

//

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
