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
    const double StrikeAngleCorrection = M_PI / 180 * 0.5;

    const double DefenceAreaR = 70;
    const double DefenceAreaDist = 190;

    const double ApproximateDeadZoneY0 = 0;
    const double ApproximateDeadZoneY1 = 65;
    const double DangerousPuckSpeed = 13;

    const double StrikeAreaAngle = -M_PI / 180 * 130;

    const double StrikeMinSpeedY = 10.5;

    double _attackAreaDestX;
    double _attackAreaDestY;

    double _attackDestX;
    double _attackDestY0;
    double _attackDestY1;

    ShapeCommon _attackingPuckArea;
    Circle _attackArea;

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

    int opponentsAttackingHP(const HockeyistF& h, const PuckF& p);     //Attacking hockeyist with puck
    int opponentsReadyToActHP(const HockeyistF& h, const PuckF& p);     //Attacking hockeyist with puck

    void defend();
    int nearestToPuck();

    bool isOnMyHalf(double x);
    bool isOnMyHalf(const UnitF& u);

    bool isPuckGoingToMyNet();
    bool canApproximatelyHitOpponentsNet();
    bool canApproximatelyHitMyNet();

    double pathSafety(double x1, double y1, double x2, double y2);
    double pathSafetyFromSelf(double x, double y);

    void act();
};

#endif
