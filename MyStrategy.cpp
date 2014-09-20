#include <iostream>
#include <math.h>
#include <cassert>

#include "MyStrategy.h"

using namespace std;

MyStrategy::MyStrategy() {
    _first = true;
    _checkInL0 = false;
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

        _attackDestX = _world.getOpponentPlayer().getNetFront();
        _attackDestY0 = _world.getOpponentPlayer().getNetTop() + _world.getPuck().getRadius();
        _attackDestY1 = _fix.invcY(_attackDestY0);

        const double a1 = -2.7792;
        const double a2 = -2.2689;

        _attackPuckArea = Sector(_attackDestX, _attackDestY1, 500, a1, a2);
        _attackAreaL1.set
                ( new Sector(_attackPuckArea), new Rectangle(0, _world.getWidth(), _attackDestY0, _world.getHeight()) );
        _attackAreaL0.set
                ( new Sector(_attackPuckArea), new Sector(_attackDestX, _attackDestY1, 385, a1, a2) );

        assert(_attackAreaL0.contains(AttackAreaDestX, AttackAreaDestY));
        assert(_attackAreaL1.contains(AttackAreaDestX, AttackAreaDestY));
    }

    _fix.setInvY(_self.getY() > _game.getWorldHeight() / 2);
    act();
}

//

double MyStrategy::conv0(double x, double z) {
    return (x * M_PI / z / 2);
}

double MyStrategy::conv1(double x, double z) {
    return M_PI / 2 * (x + M_PI - 2 * z) / (M_PI - z);
}

void MyStrategy::goAngle(double a) {
    const double z = M_PI * 2 / 3;
    const double m = 6;
    const double n = -4;
    const double alpha = M_PI / 18;
    const double beta = M_PI - alpha;

    _move.setTurn(a);

    a = fabs(a);

    double realspeed = Pif(_self.getSpeedX(), _self.getSpeedY());
    _move.setSpeedUp(0);

    if (a <= z) {
        double maxspeed = ctg(conv0(a, z)) * m / ctg(conv0(alpha, z));
        if (maxspeed > realspeed ) {
            _move.setSpeedUp(1);
        }
    } else {
        double minspeed = ctg(conv1(a, z)) * n / ctg(conv1(beta, z));
        if (minspeed < n) {
            _move.setSpeedUp(-1);
        }
    }
}

void MyStrategy::gotoXY(double x, double y) {
    goAngle(_self.getAngleTo(x, y));
}

void MyStrategy::gotoXY(const UnitF& u) {
    gotoXY(u.getX(), u.getY());
}

//

bool MyStrategy::isNearStick(const HockeyistF& h, double x, double y) {
    return ( h.getDistanceTo(x, y) <= _game.getStickLength() ) &&
           ( fabs(h.getAngleTo(x, y)) <= _game.getStickSector() / 2 );
}

bool MyStrategy::isNearStick(const HockeyistF& h, const UnitF& u) {
    return isNearStick(h, u.getX(), u.getY());
}

bool MyStrategy::isNearStick(double x, double y) {
    return isNearStick(_self, x, y);
}

bool MyStrategy::isNearStick(const UnitF& u) {
    return isNearStick(_self, u);
}

//

int MyStrategy::opponentsAttacking(double x, double y) {
    int res = 0;

    const vector<HockeyistF> hockeyists = _world.getHockeyists();
    for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
        const HockeyistF& h = *it;

        if (!h.isTeammate() && isNearStick(h, x, y) && (h.getRemainingKnockdownTicks() <= 1)) {
            ++res;
        }
    }

    return res;
}

int MyStrategy::opponentsAttacking(const UnitF& u) {
    return opponentsAttacking(u.getX(), u.getY());
}

int MyStrategy::opponentsAttackingHP(const HockeyistF& h, const PuckF& p) {         //Attacking hockeyist with puck
    int res = 0;

    const vector<HockeyistF> hockeyists = _world.getHockeyists();
    for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
        const HockeyistF& opp = *it;

        if (!opp.isTeammate() && (opp.getRemainingKnockdownTicks() <= 1)) {
            if (isNearStick(opp, h.getX(), h.getY()) || isNearStick(opp, p.getX(), p.getY())) {
                ++res;
            }
        }
    }

    return res;
}

int MyStrategy::opponentsReadyToActHP(const HockeyistF& h, const PuckF& p) {         //Attacking hockeyist with puck
    int res = 0;

    const vector<HockeyistF> hockeyists = _world.getHockeyists();
    for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
        const HockeyistF& opp = *it;

        if (!opp.isTeammate() && (opp.getRemainingCooldownTicks() <= 1) && (opp.getRemainingKnockdownTicks() <= 1)) {
            if (isNearStick(opp, h.getX(), h.getY()) || isNearStick(opp, p.getX(), p.getY())) {
                ++res;
            }
        }
    }

    return res;
}

//

void MyStrategy::defend() {
    const double x0 = _world.getMyPlayer().getNetFront();
    const double y0 = ( _world.getMyPlayer().getNetTop() + _world.getMyPlayer().getNetBottom() ) / 2;

    const double puckX = _world.getPuck().getX();
    const double puckY = _world.getPuck().getY();

    double distToPuck = Pif(x0 - puckX, y0 - puckY);

    double x = x0 + (puckX - x0) / distToPuck * DefenceAreaDist;
    double y = y0 + (puckY - y0) / distToPuck * DefenceAreaDist;

    if (Circle(x, y, DefenceAreaR).containsU(_self)) {
        _move.setTurn(_self.getAngleTo(_world.getPuck()));
    } else {
        gotoXY(x, y);
    }
}

int MyStrategy::nearestToPuck() {
    double len = Inf;
    int res = -1;

    vector<HockeyistF> h = _world.getHockeyists();
    for (unsigned int i = 0; i < h.size(); ++i) {
        if (h[i].isTeammate()) {
            double nwlen = Pif2(_world.getPuck().getX() - h[i].getX(), _world.getPuck().getY() - h[i].getY());

            if (nwlen < len) {
                len = nwlen;
                res = h[i].getId();
            }
        }
    }

    return res;
}

//

bool MyStrategy::isOnMyHalf(double x) {
    return (x < _world.getWidth() / 2);
}

bool MyStrategy::isOnMyHalf(const UnitF& u) {
    return isOnMyHalf(u.getX());
}

//

bool MyStrategy::isPuckGoingToMyNet() {
    PuckF puck = _world.getPuck();

    if ( (puck.getSpeedX() < 0) && (Pif(puck.getSpeedX(), puck.getSpeedY()) >= DangerousPuckSpeed) ) {
        double insX;
        double insY;
        bool ok = intersection( puck.getX(), puck.getY(), puck.getX() + puck.getSpeedX(), puck.getY() + puck.getSpeedY(),
                                _world.getMyPlayer().getNetFront(), 0, _world.getMyPlayer().getNetFront(), 1000,
                                insX, insY );

        if (!ok) {
            cout << __FILE__ << " " <<__LINE__ << endl;
        }

        ShapeInvX dangArea(_attackPuckArea, _world.getWidth());

        double y1 = _world.getMyPlayer().getNetTop() + ApproximateDeadZoneY0;
        double y2 = _world.getMyPlayer().getNetTop() + ApproximateDeadZoneY1;

        if ( dangArea.containsU(puck) && (insY >= _fix.invcY(y2)) && (insY <= _fix.invcY(y1)) ) {
            return true;
        }
        if ( ShapeInvY(dangArea, _world.getHeight()).containsU(puck) && (insY >= y1) && (insY <= y2) ) {
            return true;
        }
    }

    return false;
}

bool MyStrategy::canApproximatelyHitPuckToNet() {

}

//

void MyStrategy::act() {
    defend();

    if (_world.getPuck().getOwnerHockeyistId() == _self.getId()) {
        if (_attackAreaL0.containsU(_self)) {
            _checkInL0 = true;
        }

        if (_checkInL0) {
            double angle = _self.getAngleTo(_attackDestX, _attackDestY1) - StrikeAngleCorrection;
            goAngle(angle - StrikeAnglePrecision / 2);

            if ((angle > 0) && (angle < StrikeAnglePrecision)) {
                //Can strike

                if ((_world.getPuck().getY() >= _attackDestY0) || !_attackAreaL1.containsU(_self)) {
                    _move.setAction(STRIKE);        //Last chance to strike
                } else {
                    if (opponentsReadyToActHP(_self, _world.getPuck()) > 0) {
                        int opps = opponentsAttackingHP(_self, _world.getPuck());

                        if (opps >= 2) {
                            _move.setAction(STRIKE);
                        } else if (opps == 1) {
                            if (!_attackAreaL0.containsU(_self)) {
                                _move.setAction(STRIKE);
                            }
                        }
                    }
                }
            }
        } else {
            gotoXY(AttackAreaDestX, AttackAreaDestY);
        }

        if (!_attackAreaL1.containsU(_self)) {
            _checkInL0 = false;
        }
    } else {
        _checkInL0 = false;

        if (_world.getPuck().getOwnerPlayerId() == _self.getPlayerId()) {
            defend();
        } else {
            if ( (nearestToPuck() == _self.getId()) || isOnMyHalf(_world.getPuck()) ) {
                gotoXY(_world.getPuck());
            } else {
                defend();
            }

            if (isNearStick(_world.getPuck())) {

                if (_world.getPuck().getOwnerPlayerId() == _world.getOpponentPlayer().getId()) {
                    if (isOnMyHalf(_world.getPuck())) {
                        _move.setAction(STRIKE);
                    } else {
                        _move.setAction(TAKE_PUCK);
                    }
                } else {
                    if (isPuckGoingToMyNet()) {
                        _move.setAction(STRIKE);
                    } else {
                        _move.setAction(TAKE_PUCK);
                    }
                }

            }
        }

        if (!isNearStick(_world.getPuck())) {
            bool teammateNear = false;
            bool opponentNear = false;

            const vector<HockeyistF> hockeyists = _world.getHockeyists();
            for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
                const HockeyistF& h = *it;
                if ((h.getRemainingKnockdownTicks() == 0) && (h.getType() != GOALIE) && isNearStick(h)) {
                    if (h.isTeammate()) {
                        teammateNear = true;
                    } else {
                        opponentNear = true;
                    }
                }
            }

            if (opponentNear && !teammateNear) {
                _move.setAction(STRIKE);
            }
        }
    }
}
