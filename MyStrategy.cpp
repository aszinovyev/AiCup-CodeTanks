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
        _fix = CoordFix(_game.getWorldWidth(), _game.getWorldHeight(),
                        world.getMyPlayer().getNetBack() < _game.getWorldWidth() / 2);
    }

    _fix.setInvY(false);

    _self = HockeyistF(self, &_fix);
    _world = WorldF(world, &_fix, _game);
    _move = MoveF(&move, &_fix);

    if (_first) {
        _first = false;

        _attackAreaDestX = AttackAreaX0 + self.getRadius();
        _attackAreaDestY = AttackAreaY0 + _game.getPuckBindingRange() + _world.getPuck().getRadius();

        _attackDestX = _world.getOpponentPlayer().getNetFront();
        _attackDestY0 = _world.getOpponentPlayer().getNetTop() + _world.getPuck().getRadius();
        _attackDestY1 = _fix.invcY(_attackDestY0);
    }

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

void MyStrategy::gotoXY(const UnitF& u) {
    gotoXY(u.getX(), u.getY());
}

//

bool MyStrategy::isInAttackArea(double x, double y) {
    return (x > AttackAreaX0 && x < AttackAreaX1 && y > AttackAreaY0 && y < AttackAreaY1);
}

bool MyStrategy::isInAttackArea(const UnitF& u) {
    return isInAttackArea(u.getX(), u.getY());
}

//

void MyStrategy::act() {
    if (_self.getTeammateIndex() == 1) {
        gotoXY(0, 0);
        return;
    }

    _fix.setInvY(_self.getY() > _game.getWorldHeight() / 2);

    if (_world.getPuck().getOwnerHockeyistId() == _self.getId()) {
        if (isInAttackArea(_self)) {
            gotoXY(_attackDestX, _attackDestY1);

            if (fabs(_self.getAngleTo(_attackDestX, _attackDestY1)) < M_PI / 360) {
                _move.setAction(STRIKE);
            }
        } else {
            gotoXY(_attackAreaDestX, _attackAreaDestY);
        }
    } else {
        gotoXY(_world.getPuck());
        _move.setAction(TAKE_PUCK);
    }
}
