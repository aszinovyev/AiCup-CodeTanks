#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

#include "coordfix.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:
    bool _first;
    CoordFix _fix;

    HockeyistF _self;
    WorldF _world;
    GameF _game;
    MoveF _move;

    double conv0(double x, double z);
    double conv1(double x, double z);
    void gotoXY(double x, double y);

    void act();
};

#endif
