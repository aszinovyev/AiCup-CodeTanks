#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"

class MyStrategy : public Strategy {
public:
    MyStrategy();

    void gotoXY(double x, double y);

    void act();

    void move(const model::Hockeyist& self, const model::World& world, const model::Game& game, model::Move& move);

private:
    model::Hockeyist _self;
    model::World _world;
    model::Game _game;
    model::Move* _move;
};

#endif
