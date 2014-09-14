#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

#include "coordfix.h"
#include "geom.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:    
    const double StrikeAnglePrecision = M_PI / 180;
    const double StrikeAngleCorrection = M_PI / 180;

    const double DefencePointX = 256;
    const double DefencePointY = 446.5;

    const double AttackAreaX0 = 550;
    const double AttackAreaX1 = 768;
    const double AttackAreaY0 = 170;
    const double AttackAreaY1 = 346;

    double _attackAreaDestX;
    double _attackAreaDestY;

    double _attackDestX;
    double _attackDestY0;
    double _attackDestY1;

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

    bool isInAttackArea(double x, double y);
    bool isInAttackArea(const UnitF& u);

    bool isNearStick(double x, double y);
    bool isNearStick(const UnitF& u);

    void act();
};

#endif
