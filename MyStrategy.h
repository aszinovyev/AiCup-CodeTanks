#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

#include "coordfix.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();

    void gotoXY(double x, double y);

    void act();

    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:
    HockeyistF _self;
    WorldF _world;
    GameF _game;
    MoveF _move;
};

#endif
