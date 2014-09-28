#include <iostream>
#include <math.h>
#include <cassert>

#include "MyStrategy.h"

using namespace std;

MyStrategy::MyStrategy() {
    _first = true;
    _checkInL1 = false;
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

        //

        _attackingPuckArea = Sector(_attackDestX, _attackDestY1, 475, -M_PI, AttackingPuckAreaAngle);

        //

        const double limitY = _world.getOpponentPlayer().getNetTop() + _self.getRadius();
        _attackAreaL1.set( new Sector(_attackingPuckArea),
                           new Rectangle(0, _world.getWidth(), 0, limitY) );

        //

        const double x0 = _attackDestX - (_attackDestY1 - _game.getRinkTop()) * tan(-AttackingPuckAreaAngle - M_PI/2);
        const double x1 = _attackDestX - (_attackDestY1 - limitY) * tan(-AttackingPuckAreaAngle - M_PI/2);

        vector<double> xs;
        xs.push_back(x0);
        xs.push_back(x1);
        xs.push_back(x1 - AttackAreaL0Width);
        xs.push_back(x0 - AttackAreaL0Width);
        vector<double> ys;
        ys.push_back(_game.getRinkTop());
        ys.push_back(limitY);
        ys.push_back(limitY);
        ys.push_back(_game.getRinkTop());

        _attackAreaL0.set(new Sector(_attackingPuckArea), new Polygon(xs, ys));

        //

        _dangerousPuckArea = Sector(_world.getMyPlayer().getNetFront(), _attackDestY1, Inf, -M_PI, DangerousPuckAreaAngle);
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

void MyStrategy::goAngle(double a) {
    const double z = M_PI * 2 / 3;
    const double m = 6;
    const double n = -4;
    const double alpha = 20 * M_PI/180;
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

double MyStrategy::distNextTick(UnitF u1, UnitF u2) {
    const double x = u2.getX() - u1.getX();
    const double y = u2.getY() - u1.getY();

    const double sx = u2.getSpeedX() - u1.getSpeedX();
    const double sy = u2.getSpeedY() - u1.getSpeedY();

    return Pif(x + sx, y + sy);
}

bool MyStrategy::isNearStickNextTick(const HockeyistF& h, const UnitF& u) {
    const double rotationSpeed = 3 * M_PI/180;

    return ( (distNextTick(h, u) <= _game.getStickLength() * ExcessK) &&
             fabs(h.getAngleTo(u)) <= _game.getStickSector() / 2 + rotationSpeed * ExcessK );
}

int MyStrategy::opponentsAttackingHP() {         //Attacking hockeyist with puck
    const double safeDownTicks = 10;

    PuckF puck = _world.getPuck();

    int res = 0;

    const vector<HockeyistF> hockeyists = _world.getHockeyists();
    for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
        const HockeyistF& h = *it;
        if (!h.isTeammate()) {

            if (distNextTick(_self, h) <= 2 * _self.getRadius() * ExcessK) {
                ++res;
            } else if ((h.getRemainingKnockdownTicks() < safeDownTicks) || (h.getRemainingCooldownTicks() < safeDownTicks)) {
                if (isNearStickNextTick(h, _self) || isNearStickNextTick(h, puck)) {
                    ++res;
                }
            }

        }
    }

    return res;
}

//int MyStrategy::opponentsReadyToActHP(const HockeyistF& h, const PuckF& p) {         //Attacking hockeyist with puck
//    int res = 0;

//    const vector<HockeyistF> hockeyists = _world.getHockeyists();
//    for (vector<HockeyistF>::const_iterator it = hockeyists.cbegin(); it != hockeyists.cend(); ++it) {
//        const HockeyistF& opp = *it;

//        if (!opp.isTeammate() && (opp.getRemainingCooldownTicks() <= 1) && (opp.getRemainingKnockdownTicks() <= 1)) {
//            if (isNearStick(opp, h.getX(), h.getY()) || isNearStick(opp, p.getX(), p.getY())) {
//                ++res;
//            }
//        }
//    }

//    return res;
//}

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
//    const int tick = _world.getTick();
//    const int t1 = 1030;
//    const int t2 = 1050;

//    const bool dbg = ((tick >= t1) && (tick <= t2));

//    if (dbg) {
//        cout << "isPuckGoingToMyNet() " << tick << endl;
//    }

    PuckF puck = _world.getPuck();

    if ( (puck.getSpeedX() < 0) && (puck.getSpeedY() >= DangerousPuckSpeedY) ) {
        double insX;
        double insY;
        bool ok = intersection( puck.getX(), puck.getY(), puck.getX() + puck.getSpeedX(), puck.getY() + puck.getSpeedY(),
                                _world.getMyPlayer().getNetFront(), 0, _world.getMyPlayer().getNetFront(), 1,
                                insX, insY );

        if (!ok) {
            cout << __FILE__ << " " <<__LINE__ << endl;
        }

        const double y1 = _world.getMyPlayer().getNetTop() + ApproximateDeadZoneY0;
        const double y2 = _world.getMyPlayer().getNetTop() + ApproximateDeadZoneY1;

//        if (dbg) {
//            cout << insY << endl;
//            cout << y1 << " " << y2 << endl;
//            cout << _fix.invcY(y2) << " " << _fix.invcY(y1) << endl << endl;
//        }

        if ( _dangerousPuckArea.containsU(puck) && (insY >= _fix.invcY(y2)) && (insY <= _fix.invcY(y1)) ) {
            return true;
        }
        if ( ShapeInvY(_dangerousPuckArea, _world.getHeight()).containsU(puck) && (insY >= y1) && (insY <= y2) ) {
            return true;
        }
    }

    return false;
}

bool MyStrategy::canApproximatelyHitOpponentsNet() {
    PuckF puck = _world.getPuck();

    const double speed = 15 + Pif(_self.getSpeedX(), _self.getSpeedY()) *
                              cos( _self.getAngle() - atan2(_self.getSpeedY(), _self.getSpeedX()) );

    const double speedY = speed * sin( atan2(puck.getY() - _self.getY(), puck.getX() - _self.getX()) );

    if ( ShapeInvX(_dangerousPuckArea, _world.getWidth()).containsU(_self) && isNearStick(puck) &&
         (fabs(_self.getAngle()) < M_PI/2) && (speedY >= DangerousPuckSpeedY) )
    {
        double x;
        double y;
        bool ok = intersection(_self.getX(), _self.getY(), puck.getX(), puck.getY(),
                               _world.getOpponentPlayer().getNetFront(), 0, _world.getOpponentPlayer().getNetFront(), 1,
                               x, y);

        if (!ok) {
            cout << __FILE__ << " " <<__LINE__ << endl;
        }

        const double y1 = _world.getOpponentPlayer().getNetBottom() - ApproximateDeadZoneY1;
        const double y2 = _world.getOpponentPlayer().getNetBottom() - ApproximateDeadZoneY0;

        if ( (y >= y1) && (y <= y2) ) {
            return true;
        }
    }

    return false;
}

bool MyStrategy::canApproximatelyHitMyNet() {
    PuckF puck = _world.getPuck();

    const double speed = 15 + Pif(_self.getSpeedX(), _self.getSpeedY()) *
                              cos( _self.getAngle() - atan2(_self.getSpeedY(), _self.getSpeedX()) );

    const double speedY = speed * sin( atan2(puck.getY() - _self.getY(), puck.getX() - _self.getX()) );

    if ( _dangerousPuckArea.containsU(_self) &&
         isNearStick(puck) && (fabs(_self.getAngle()) > M_PI/2) && (speedY >= DangerousPuckSpeedY) )
    {
        double x;
        double y;
        bool ok = intersection(_self.getX(), _self.getY(), puck.getX(), puck.getY(),
                               _world.getMyPlayer().getNetFront(), 0, _world.getMyPlayer().getNetFront(), 1,
                               x, y);

        if (!ok) {
            cout << __FILE__ << " " <<__LINE__ << endl;
        }

        const double y1 = _world.getMyPlayer().getNetBottom() - ApproximateDeadZoneY1;
        const double y2 = _world.getMyPlayer().getNetBottom() - ApproximateDeadZoneY0;

        if ( (y >= y1) && (y <= y2) ) {
            return true;
        }
    }

    return false;
}

//

double MyStrategy::pathSafety(double x1, double y1, double x2, double y2) {
    vector<HockeyistF> hockeyists = _world.getHockeyists();

    int cnt = 0;
    double sum = 0;

    for (unsigned int i = 0; i < hockeyists.size(); ++i) {
        HockeyistF& h = hockeyists[i];

        if (!h.isTeammate() && (h.getType() != GOALIE)) {
            ++cnt;

            bool inside1 = (scalarMul(x2 - x1, y2 - y1, h.getX() - x1, h.getY() - y1) > 0);
            bool inside2 = (scalarMul(x1 - x2, y1 - y2, h.getX() - x2, h.getY() - y2) > 0);

            if (!inside1) {
                sum += sqrt( Pif(h.getX() - x1, h.getY() - y1) );
            } else if (!inside2) {
                sum += sqrt( Pif(h.getX() - x2, h.getY() - y2) );
            } else {
                sum += sqrt( height(h.getX(), h.getY(), x1, y1, x2, y2) );
            }
        }
    }

    return sqr(sum / cnt) / Pif(x1 - x2, y1 - y2);
}

double MyStrategy::pathSafetyFromSelf(double x, double y) {
    return pathSafety(_self.getX(), _self.getY(), x, y);
}

//

void MyStrategy::gotoL0() {
    const int count = 20;

    const double deltaY = (_world.getOpponentPlayer().getNetTop() - _game.getRinkTop() - _self.getRadius()) / (count - 1);
    const double deltaX = AttackAreaL0Width / (count - 1);

    double maxS = -1;
    double resX = -1;
    double resY = -1;

    double y = _game.getRinkTop() + _self.getRadius();
    for (int i = 0; i < count; ++i) {
        double x = _attackDestX - (_attackDestY1 - y) * tan(-AttackingPuckAreaAngle - M_PI/2) - AttackAreaL0Width;

        for (int j = 0; j < count; ++j) {
            if (_attackAreaL0.contains(x, y)) {
                //cout << x << " " << y << endl;

                double s = pathSafetyFromSelf(x, y);
                if (s > maxS) {
                    maxS = s;
                    resX = x;
                    resY = y;
                }

                s = pathSafetyFromSelf(x, _fix.invcY(y));
                if (s > maxS) {
                    maxS = s;
                    resX = x;
                    resY = _fix.invcY(y);
                }
            }

            x += deltaX;
        }

        y += deltaY;
    }

    gotoXY(resX, resY);
}

//

void MyStrategy::act() {
    _fix.setInvY(_self.getY() > _game.getWorldHeight() / 2);

    if (_world.getPuck().getOwnerHockeyistId() == _self.getId()) {
        if (_attackAreaL1.containsU(_world.getPuck())) {
            _checkInL1 = true;
        }

        const double angle = atan2(_attackDestY1 - _world.getPuck().getY(), _attackDestX - _world.getPuck().getX()) -
                             atan2(_world.getPuck().getY() - _self.getY(), _world.getPuck().getX() - _self.getX()) -
                             StrikeAngleCorrection;

        if (_attackAreaL0.containsU(_world.getPuck())) {
            //cout << "IN" << endl;
            goAngle(-_self.getAngle() + 15 * M_PI/180);
        } else {
            //cout << "OUT" << endl;
            gotoL0();
        }

        if (_checkInL1 && (fabs(angle) <= _game.getPassSector() / 2)) {
            const double speed = 15 + Pif(_self.getSpeedX(), _self.getSpeedY()) *
                                      cos( _self.getAngle() + angle - atan2(_self.getSpeedY(), _self.getSpeedX()) );

            const double speedY = speed * sin(_self.getAngle() + angle);
            cout << speed << " " << (_self.getAngle() + angle) / M_PI * 180 << " " << speedY << endl;

            if (speedY >= StrikeMinSpeedY) {
                //Can strike
                _move.setPassAngle(angle);
                _move.setPassPower(1);

                if ( !_attackAreaL1.containsU(_world.getPuck()) || (opponentsAttackingHP() > 0) ) {
                    _move.setAction(PASS);
                }
            }
        }

        if (!_attackAreaL1.containsU(_world.getPuck())) {
            _checkInL1 = false;
        }
    } else {
        _checkInL1 = false;

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
                        cout << "isPuckGoingToMyNet() #" <<  _self.getTeammateIndex() + 1 << " " <<
                                _world.getTick() << endl;
                        _move.setAction(STRIKE);
                    } else {
                        _move.setAction(TAKE_PUCK);
                    }
                }

                if (canApproximatelyHitOpponentsNet()) {
                    cout << "canApproximatelyHitOpponentsNet() #" << _self.getTeammateIndex() + 1 << " " <<
                            _world.getTick() << endl;
                    _move.setAction(STRIKE);
                }

                if (canApproximatelyHitMyNet()) {
                    cout << "canApproximatelyHitMyNet() #" << _self.getTeammateIndex() + 1 << " " <<
                            _world.getTick() << endl;
                    _move.setAction(TAKE_PUCK);
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
