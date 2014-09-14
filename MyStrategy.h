#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

#include "coordfix.h"
#include "geom.h"
#include "polygon.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:    
    const double StrikeAnglePrecision = M_PI / 180;
    const double StrikeAngleCorrection = M_PI / 360;

    const Circle DefenceArea = Circle(256, 446.5, 50);

    const Circle AttackAreaL0 = Circle(750, 250, 75);
    const Circle AttackAreaL1 = Circle(AttackAreaL0.getX() + AttackAreaL0.getR() / sqrt(2),
                                       AttackAreaL0.getY() + AttackAreaL0.getR() / sqrt(2),
                                       AttackAreaL0.getR() * 1.5);

    const double AttackAreaDestX = AttackAreaL0.getX();
    const double AttackAreaDestY = AttackAreaL0.getY();

    double _attackDestX;
    double _attackDestY0;
    double _attackDestY1;

    bool _first;
    CoordFix _fix;

    HockeyistF _self;
    WorldF _world;
    GameF _game;
    MoveF _move;

    bool _checkInL0;

    double conv0(double x, double z);
    double conv1(double x, double z);
    void goAngle(double a);
    void gotoXY(double x, double y);
    void gotoXY(const UnitF& u);

    bool isNearStick(const HockeyistF& h, double x, double y);
    bool isNearStick(const HockeyistF& h, const UnitF& u);
    bool isNearStick(double x, double y);
    bool isNearStick(const UnitF& u);

    int opponentsAttacking(double x, double y);
    int opponentsAttacking(const UnitF& u);
    int opponentsAttackingHP(const HockeyistF& h, const PuckF& p);     //Attacking hockeyist with puck

    void act();
};

#endif
