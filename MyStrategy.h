#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

#include "coordfix.h"
#include "geom.h"
#include "shape.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:    
    const double ExcessK = 1.2;

    const double StrikeAngleCorrection = M_PI / 180 * 0.45;

    const double DefenceAreaR = 70;
    const double DefenceAreaDist = 190;

    const double AttackingPuckAreaAngle = -M_PI / 180 * 135;
    const double AttackAreaL0Width = 100;

    const double StrikeMinSpeedY = 10.6;

    const double ApproximateDeadZoneY0 = 0;
    const double ApproximateDeadZoneY1 = 80;
    const double DangerousPuckSpeedY = 9.5;
    const double DangerousPuckAreaAngle = -M_PI / 180 * 55;

    double _attackDestX;
    double _attackDestY0;
    double _attackDestY1;

    Sector _attackingPuckArea;
    ShapeCommon _attackAreaL0;
    ShapeCommon _attackAreaL1;

    Sector _dangerousPuckArea;

    bool _checkInL1;

    bool _first;
    CoordFix _fix;

    HockeyistF _self;
    WorldF _world;
    GameF _game;
    MoveF _move;

    double conv0(double x, double z);
    double conv1(double x, double z);
    void goAngle(double a);
    void gotoXY(double x, double y);
    void gotoXY(const UnitF& u);

    bool isNearStick(const HockeyistF& h, double x, double y);
    bool isNearStick(const HockeyistF& h, const UnitF& u);
    bool isNearStick(double x, double y);
    bool isNearStick(const UnitF& u);

    double distNextTick(UnitF u1, UnitF u2);
    bool isNearStickNextTick(const HockeyistF& h, const UnitF& u);

    int opponentsAttackingHP();     //Attacking hockeyist with puck
    //int opponentsReadyToActHP(const HockeyistF& h, const PuckF& p);     //Attacking hockeyist with puck

    void defend();
    int nearestToPuck();

    bool isOnMyHalf(double x);
    bool isOnMyHalf(const UnitF& u);

    bool isPuckGoingToMyNet();
    bool canApproximatelyHitOpponentsNet();
    bool canApproximatelyHitMyNet();

    double pathSafety(double x1, double y1, double x2, double y2);
    double pathSafetyFromSelf(double x, double y);

    void gotoL0();

    void act();
};

#endif
